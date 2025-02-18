// Copyright (C) 2020-2021 Sami Väisänen
// Copyright (C) 2020-2021 Ensisoft http://www.ensisoft.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "config.h"

#ifdef AUDIO_USE_PULSEAUDIO
  #include <pulse/pulseaudio.h>
#endif

#include <cassert>
#include <cstring>

#include "base/assert.h"
#include "base/logging.h"
#include "audio/source.h"
#include "audio/stream.h"
#include "audio/device.h"

#ifdef AUDIO_USE_PULSEAUDIO
namespace {
void ThrowPaException(pa_context* context, const char* what)
{
    const auto err = pa_context_errno(context);
    ASSERT(err != PA_OK);
    const auto* str = pa_strerror(err);
    if (str && std::strlen(str))
        throw std::runtime_error(str);
    throw std::runtime_error(what);
}
}  // namespace
#endif

namespace audio
{

#ifdef AUDIO_USE_PULSEAUDIO

// AudioDevice implementation for PulseAudio
class PulseAudio : public Device
{
public:
    PulseAudio(const char* appname)
    {
        loop_ = pa_mainloop_new();
        main_ = pa_mainloop_get_api(loop_);
        context_ = pa_context_new(main_, appname);

        pa_context_set_state_callback(context_, state_callback, this);
        pa_context_connect(context_, nullptr, PA_CONTEXT_NOAUTOSPAWN, nullptr);

        // this is kinda ugly...
        while (state_ == Device::State::None)
        {
            pa_mainloop_iterate(loop_, 0, nullptr);
        }

        if (state_ == Device::State::Error)
            throw std::runtime_error("pulseaudio connect error");
    }

   ~PulseAudio()
    {
        pa_context_set_state_callback(context_, nullptr, nullptr);
        pa_context_disconnect(context_);
        pa_context_unref(context_);
        pa_mainloop_free(loop_);
    }

    virtual std::shared_ptr<Stream> Prepare(std::unique_ptr<Source> source) override
    {
        const auto name = source->GetName();
        try
        {
            auto stream = std::make_shared<PlaybackStream>(std::move(source), context_, buffer_size_);

            // todo: fix this silly looping
            while (stream->GetState() == Stream::State::None)
            {
                pa_mainloop_iterate(loop_, 0, nullptr);
            }
            if (stream->GetState() == Stream::State::Ready)
                return stream;

            ERROR("PulseAudio audio source failed to prepare. [name='%1']", name);
        }
        catch (const std::exception& e)
        { ERROR("PulseAudio audio source failed to prepare. [name='%1', error='%2'].",  name, e.what()); }
        return nullptr;
    }

    virtual void Poll() override
    {
        pa_mainloop_iterate(loop_, 0, nullptr);
    }

    virtual void Init() override
    { }

    virtual State GetState() const override
    {
        return state_;
    }

    virtual void SetBufferSize(unsigned milliseconds) override
    {
        buffer_size_ = milliseconds;
    }

    PulseAudio(const PulseAudio&) = delete;
    PulseAudio& operator=(const PulseAudio&) = delete;
private:
    class PlaybackStream  : public Stream
    {
    public:
        PlaybackStream(std::unique_ptr<Source> source, pa_context* context, unsigned buffer_size_ms)
            : source_(std::move(source))
        {
            DEBUG("Creating new PulseAudio playback stream. [name='%1', channels=%2, rate=%3, format=%4]",
                  source_->GetName(), source_->GetNumChannels(),
                  source_->GetRateHz(), source_->GetFormat());
            const auto& name  = source_->GetName();
            pa_sample_spec spec;
            spec.channels = source_->GetNumChannels();
            spec.rate     = source_->GetRateHz();
            if (source_->GetFormat() == Source::Format::Float32)
                spec.format = PA_SAMPLE_FLOAT32NE;
            else if (source_->GetFormat() == Source::Format::Int16)
                spec.format = PA_SAMPLE_S16NE;
            else if (source_->GetFormat() == Source::Format::Int32)
                spec.format = PA_SAMPLE_S32NE;
            else BUG("Unsupported playback format.");

            stream_ = pa_stream_new(context, name.c_str(), &spec, nullptr);
            if (!stream_)
                ThrowPaException(context, "create stream failed.");

            pa_stream_set_state_callback(stream_, state_callback, this);
            pa_stream_set_write_callback(stream_, write_callback, this);
            pa_stream_set_underflow_callback(stream_, underflow_callback, this);

            const auto sample_size    = Source::ByteSize(source_->GetFormat());
            const auto samples_per_ms = source_->GetRateHz() / 1000u;
            const auto bytes_per_ms   = source_->GetNumChannels() * sample_size * samples_per_ms;

            pa_buffer_attr buffering;
            buffering.maxlength = bytes_per_ms * buffer_size_ms;
            buffering.prebuf = -1;
            buffering.minreq = -1;
            buffering.tlength = -1;

            if (pa_stream_connect_playback(stream_,
                nullptr, // device
                &buffering,
                PA_STREAM_START_CORKED, // stream flags 0 for default
                nullptr,  // volume
                nullptr)) // sync stream
                ThrowPaException(context, "stream playback failed.");
        }

       ~PlaybackStream()
        {
            if (stream_)
            {
                Cancel();
            }
        }

        virtual Stream::State GetState() const override
        {  return state_; }
        
        virtual std::unique_ptr<Source> GetFinishedSource() override
        {
            std::unique_ptr<Source> ret;
            if (state_ == State::Complete || state_ == State::Error)
                ret = std::move(source_);
            return ret;
        }

        virtual std::string GetName() const override
        { return source_->GetName(); }
        virtual std::uint64_t GetStreamTime() const override
        { return milliseconds_; }
        virtual std::uint64_t GetStreamBytes() const override
        { return num_pcm_bytes_; }

        virtual void Play() override
        {
            pa_stream_cork(stream_, 0, nullptr, nullptr);
            DEBUG("PulseAudio stream play. [name='%1']", source_->GetName());
        }

        virtual void Pause() override
        {
            pa_stream_cork(stream_, 1, nullptr, nullptr);
            DEBUG("PulseAudio stream pause. [name='%1']", source_->GetName());
        }

        virtual void Resume() override
        {
            pa_stream_cork(stream_, 0, nullptr, nullptr);
            DEBUG("PulseAudio stream resume. [name='%1']", source_->GetName());
        }

        virtual void Cancel() override
        {
            pa_stream_set_state_callback(stream_, nullptr, nullptr);
            pa_stream_set_write_callback(stream_, nullptr, nullptr);
            pa_stream_set_underflow_callback(stream_, nullptr, nullptr);
            pa_stream_disconnect(stream_);
            pa_stream_unref(stream_);
            stream_ = nullptr;
            if (source_)
            {
                source_->Shutdown();
                DEBUG("PulseAudio stream cancel. [name='%1']", source_->GetName());
            }
        }

        virtual void SendCommand(std::unique_ptr<Command> cmd) override
        {
            source_->RecvCommand(std::move(cmd));
        }

        virtual std::unique_ptr<Event> GetEvent() override
        {
            return source_->GetEvent();
        }
    private:
        static void underflow_callback(pa_stream* stream, void* user)
        {
            auto* this_ = static_cast<PlaybackStream*>(user);
            if (!this_->source_)
                return;
            WARN("PulseAudio stream underflow callback. [name='%1']", this_->source_->GetName());
        }

        static void drain_callback(pa_stream* stream, int success, void* user)
        {
            auto* this_ = static_cast<PlaybackStream*>(user);

            DEBUG("PulseAudio stream drain callback. [name='%1']", this_->source_->GetName());

            this_->state_ = Stream::State::Complete;
        }
        static void write_callback(pa_stream* stream, size_t length, void* user)
        {
            auto* this_ = static_cast<PlaybackStream*>(user);
            auto& source = this_->source_;

            if (this_->state_ != State::Ready)
                return;

            // we should always still have the source object as long
            // as the state is ready and playing and we're expected
            // to write more data into the stream!
            ASSERT(source);

            // callback while the stream is being drained. weird..
            if (!source->HasMore(this_->num_pcm_bytes_))
                return;

            // don't let exceptions propagate into the C library...
            try
            {
                // try to see if we have a happy case and can get a buffer
                // with a matching size from the server. however it's possible
                // that the returned buffer doesn't have enough capacity
                // in which case it cannot be used. :(
                void* buffer = nullptr;
                size_t buffer_size = -1;
                if (pa_stream_begin_write(this_->stream_, &buffer, &buffer_size))
                    throw std::runtime_error("pa_stream_begin_write failed.");

                std::vector<std::uint8_t> backup_buff;
                if (buffer_size < length)
                {
                    // important: cancel the pa_stream_begin_write!
                    pa_stream_cancel_write(this_->stream_);

                    backup_buff.resize(length);
                    buffer = &backup_buff[0];
                }

                // may throw.
                const auto bytes = this_->source_->FillBuffer(buffer, length);

                // it seems that if the pa_stream_write doesn't write exactly
                // as many bytes as requested the playback stops and the
                // callback is no longer invoked.
                // as of July-2021 there's an open bug about this.
                // https://gitlab.freedesktop.org/pulseaudio/pulseaudio/-/issues/1132
                if (pa_stream_write(this_->stream_, buffer, bytes, nullptr, 0, PA_SEEK_RELATIVE))
                    throw std::runtime_error("pa_stream_write failed.");

                const auto sample_size    = Source::ByteSize(source->GetFormat());
                const auto samples_per_ms = source->GetRateHz() / 1000u;
                const auto bytes_per_ms   = source->GetNumChannels() * sample_size * samples_per_ms;
                const auto milliseconds   = bytes / bytes_per_ms;
                this_->milliseconds_  += milliseconds;
                this_->num_pcm_bytes_ += bytes;

                // reaching the end of the stream, i.e. we're providing the last
                // write of data. schedule the drain operation callback on the stream.
                if (!source->HasMore(this_->num_pcm_bytes_))
                    pa_operation_unref(pa_stream_drain(this_->stream_, drain_callback, this_));
                else if (bytes < length * 0.8)
                    WARN("PulseAudio stream write possibly insufficient. [requested=%1 b, wrote=%2 b].", length, bytes);
            }
            catch (const std::exception& exception)
            {
                ERROR("PulseAudio stream error. [name='%1', error='%2']", source->GetName(), exception.what());
                this_->state_ = State::Error;
            }
        }

        static void state_callback(pa_stream* stream, void* user)
        {
            auto* this_ = static_cast<PlaybackStream*>(user);
            const auto state = pa_stream_get_state(stream);
            switch (state)
            {
                case PA_STREAM_CREATING:
                    break;
                case PA_STREAM_UNCONNECTED:
                    break;

                // stream finished cleanly, but this state transition
                // only is dispatched when the pa_stream_disconnect is connected.
                case PA_STREAM_TERMINATED:
                    //this_->state_ = AudioStream::State::complete;
                    break;

                case PA_STREAM_FAILED:
                   this_->state_ = Stream::State::Error;
                   break;

                case PA_STREAM_READY:
                    this_->state_ = Stream::State::Ready;
                    break;
            }
            DEBUG("Pulseaudio stream state callback. [name='%1', state=%2]", this_->source_->GetName(),
                  ToString(state));
        }
        static const char* ToString(pa_stream_state pa_state)
        {
            #define CASE(x) case x: return #x
            switch (pa_state)
            {
                CASE(PA_STREAM_CREATING);
                CASE(PA_STREAM_UNCONNECTED);
                CASE(PA_STREAM_TERMINATED);
                CASE(PA_STREAM_FAILED);
                CASE(PA_STREAM_READY);
            }
            #undef CASE
            return "???";
        }

    private:
        std::unique_ptr<Source> source_;
        pa_stream*  stream_  = nullptr;
        Stream::State state_ = Stream::State::None;
        std::uint64_t num_pcm_bytes_ = 0;
        std::uint64_t milliseconds_ = 0;
    private:
    };
private:
    static void state_callback(pa_context* context, void* user)
    {
        auto* this_ = static_cast<PulseAudio*>(user);
        switch (pa_context_get_state(context))
        {
            case PA_CONTEXT_CONNECTING:
                DEBUG("PA_CONTEXT_CONNECTING");
                break;
            case PA_CONTEXT_AUTHORIZING:
                DEBUG("PA_CONTEXT_AUTHORIZING");
                break;
            case PA_CONTEXT_SETTING_NAME:
                DEBUG("PA_CONTEXT_SETTING_NAME");
                break;
            case PA_CONTEXT_UNCONNECTED:
                DEBUG("PA_CONTEXT_UNCONNECTED");
                break;
            case PA_CONTEXT_TERMINATED:
                DEBUG("PA_CONTEXT_TERMINATED");
                break;
            case PA_CONTEXT_READY:
                DEBUG("PA_CONTEXT_READY");
                this_->state_ = State::Ready;
                break;
            case PA_CONTEXT_FAILED:
                DEBUG("PA_CONTEXT_FAILED");
                this_->state_ = State::Error;
                break;
        }
    }
private:
    pa_mainloop* loop_     = nullptr;
    pa_mainloop_api* main_ = nullptr;
    pa_context* context_   = nullptr;
private:
    State state_ = Device::State::None;
    unsigned buffer_size_ = 20;
};

// static
std::unique_ptr<Device> Device::Create(const char* appname)
{
    return std::make_unique<PulseAudio>(appname);
}

#endif // AUDIO_USE_PULSEAUDIO

} // namespace
