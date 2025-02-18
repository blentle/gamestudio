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

#include "warnpush.h"
#  include <neargye/magic_enum.hpp>
#include "warnpop.h"

#include <cmath>

#include "base/logging.h"
#include "base/assert.h"
#include "base/hash.h"
#include "base/utility.h"
#include "data/writer.h"
#include "data/reader.h"
#include "game/animation.h"
#include "game/entity.h"

namespace game
{

std::size_t SetFlagActuatorClass::GetHash() const
{
    std::size_t hash = 0;
    hash = base::hash_combine(hash, mId);
    hash = base::hash_combine(hash, mName);
    hash = base::hash_combine(hash, mNodeId);
    hash = base::hash_combine(hash, mFlagName);
    hash = base::hash_combine(hash, mStartTime);
    hash = base::hash_combine(hash, mDuration);
    hash = base::hash_combine(hash, mFlagAction);
    hash = base::hash_combine(hash, mFlags);
    return hash;
}

void SetFlagActuatorClass::IntoJson(data::Writer& data) const
{
    data.Write("id",        mId);
    data.Write("name",      mName);
    data.Write("node",      mNodeId);
    data.Write("flag",      mFlagName);
    data.Write("starttime", mStartTime);
    data.Write("duration",  mDuration);
    data.Write("action",    mFlagAction);
    data.Write("flags",     mFlags);
}

bool SetFlagActuatorClass::FromJson(const data::Reader& data)
{
    data.Read("id",        &mId);
    data.Read("name",      &mName);
    data.Read("node",      &mNodeId);
    data.Read("flag",      &mFlagName);
    data.Read("starttime", &mStartTime);
    data.Read("duration",  &mDuration);
    data.Read("action",    &mFlagAction);
    data.Read("flags",     &mFlags);
    return true;
}

std::size_t KinematicActuatorClass::GetHash() const
{
    std::size_t hash = 0;
    hash = base::hash_combine(hash, mId);
    hash = base::hash_combine(hash, mName);
    hash = base::hash_combine(hash, mNodeId);
    hash = base::hash_combine(hash, mInterpolation);
    hash = base::hash_combine(hash, mStartTime);
    hash = base::hash_combine(hash, mDuration);
    hash = base::hash_combine(hash, mEndLinearVelocity);
    hash = base::hash_combine(hash, mEndAngularVelocity);
    hash = base::hash_combine(hash, mFlags);
    return hash;
}

void KinematicActuatorClass::IntoJson(data::Writer& data) const
{
    data.Write("id",               mId);
    data.Write("name",             mName);
    data.Write("node",             mNodeId);
    data.Write("method",           mInterpolation);
    data.Write("starttime",        mStartTime);
    data.Write("duration",         mDuration);
    data.Write("linear_velocity",  mEndLinearVelocity);
    data.Write("angular_velocity", mEndAngularVelocity);
    data.Write("flags",            mFlags);
}

bool KinematicActuatorClass::FromJson(const data::Reader& data)
{
    data.Read("id",               &mId);
    data.Read("name",             &mName);
    data.Read("node",             &mNodeId);
    data.Read("method",           &mInterpolation);
    data.Read("starttime",        &mStartTime);
    data.Read("duration",         &mDuration);
    data.Read("linear_velocity",  &mEndLinearVelocity);
    data.Read("angular_velocity", &mEndAngularVelocity);
    data.Read("flags",            &mFlags);
    return true;
}

size_t SetValueActuatorClass::GetHash() const
{
    std::size_t hash = 0;
    hash = base::hash_combine(hash, mId);
    hash = base::hash_combine(hash, mName);
    hash = base::hash_combine(hash, mNodeId);
    hash = base::hash_combine(hash, mInterpolation);
    hash = base::hash_combine(hash, mParamName);
    hash = base::hash_combine(hash, mStartTime);
    hash = base::hash_combine(hash, mDuration);
    hash = base::hash_combine(hash, mEndValue);
    hash = base::hash_combine(hash, mFlags);
    return hash;
}

void SetValueActuatorClass::IntoJson(data::Writer& data) const
{
    data.Write("id",        mId);
    data.Write("cname",     mName);
    data.Write("node",      mNodeId);
    data.Write("method",    mInterpolation);
    data.Write("name",      mParamName);
    data.Write("starttime", mStartTime);
    data.Write("duration",  mDuration);
    data.Write("value",     mEndValue);
    data.Write("flags",     mFlags);
}

bool SetValueActuatorClass::FromJson(const data::Reader& data)
{
    data.Read("id",        &mId);
    data.Read("cname",     &mName);
    data.Read("node",      &mNodeId);
    data.Read("method",    &mInterpolation);
    data.Read("name",      &mParamName);
    data.Read("starttime", &mStartTime);
    data.Read("duration",  &mDuration);
    data.Read("value",     &mEndValue);
    data.Read("flags",     &mFlags);
    return true;
}

void TransformActuatorClass::IntoJson(data::Writer& data) const
{
    data.Write("id",        mId);
    data.Write("name",      mName);
    data.Write("node",      mNodeId);
    data.Write("method",    mInterpolation);
    data.Write("starttime", mStartTime);
    data.Write("duration",  mDuration);
    data.Write("position",  mEndPosition);
    data.Write("size",      mEndSize);
    data.Write("scale",     mEndScale);
    data.Write("rotation",  mEndRotation);
    data.Write("flags",     mFlags);
}

bool TransformActuatorClass::FromJson(const data::Reader& data)
{
    data.Read("id",        &mId);
    data.Read("name",      &mName);
    data.Read("node",      &mNodeId);
    data.Read("starttime", &mStartTime);
    data.Read("duration",  &mDuration);
    data.Read("position",  &mEndPosition);
    data.Read("size",      &mEndSize);
    data.Read("scale",     &mEndScale);
    data.Read("rotation",  &mEndRotation);
    data.Read("method",    &mInterpolation);
    data.Read("flags",     &mFlags);
    return true;
}

std::size_t TransformActuatorClass::GetHash() const
{
    std::size_t hash = 0;
    hash = base::hash_combine(hash, mId);
    hash = base::hash_combine(hash, mName);
    hash = base::hash_combine(hash, mNodeId);
    hash = base::hash_combine(hash, mInterpolation);
    hash = base::hash_combine(hash, mStartTime);
    hash = base::hash_combine(hash, mDuration);
    hash = base::hash_combine(hash, mEndPosition);
    hash = base::hash_combine(hash, mEndSize);
    hash = base::hash_combine(hash, mEndScale);
    hash = base::hash_combine(hash, mEndRotation);
    hash = base::hash_combine(hash, mFlags);
    return hash;
}

void MaterialActuatorClass::IntoJson(data::Writer& data) const
{
    data.Write("id",       mId);
    data.Write("cname",    mName);
    data.Write("node",     mNodeId);
    data.Write("method",   mInterpolation);
    data.Write("start",    mStartTime);
    data.Write("duration", mDuration);
    data.Write("name",     mParamName);
    data.Write("value",    mParamValue);
    data.Write("flags",    mFlags);
}
bool MaterialActuatorClass::FromJson(const data::Reader& data)
{
    data.Read("id",       &mId);
    data.Read("cname",    &mName);
    data.Read("node",     &mNodeId);
    data.Read("method",   &mInterpolation);
    data.Read("start",    &mStartTime);
    data.Read("duration", &mDuration);
    data.Read("name",     &mParamName);
    data.Read("value",    &mParamValue);
    data.Read("flags",   &mFlags);
    return true;
}
std::size_t MaterialActuatorClass::GetHash() const
{
    size_t hash = 0;
    hash = base::hash_combine(hash, mId);
    hash = base::hash_combine(hash, mName);
    hash = base::hash_combine(hash, mNodeId);
    hash = base::hash_combine(hash, mInterpolation);
    hash = base::hash_combine(hash, mStartTime);
    hash = base::hash_combine(hash, mDuration);
    hash = base::hash_combine(hash, mParamName);
    hash = base::hash_combine(hash, mParamValue);
    hash = base::hash_combine(hash, mFlags);
    return hash;
}

void KinematicActuator::Start(EntityNode& node)
{
    if (const auto* body = node.GetRigidBody())
    {
        mStartLinearVelocity  = body->GetLinearVelocity();
        mStartAngularVelocity = body->GetAngularVelocity();
        if (body->GetSimulation() == RigidBodyItemClass::Simulation::Static)
        {
            WARN("EntityNode '%1' is not dynamically or kinematically simulated.", node.GetName());
            WARN("Kinematic actuator will have no effect.");
        }
    }
    else
    {
        WARN("EntityNode '%1' doesn't have a rigid body item.", node.GetName());
        WARN("Kinematic actuator will have no effect.");
    }
}
void KinematicActuator::Apply(EntityNode& node, float t)
{
    if (auto* body = node.GetRigidBody())
    {
        const auto method = mClass->GetInterpolation();
        const auto linear_velocity = math::interpolate(mStartLinearVelocity, mClass->GetEndLinearVelocity(), t, method);
        const auto angular_velocity = math::interpolate(mStartAngularVelocity, mClass->GetEndAngularVelocity(), t, method);
        body->AdjustLinearVelocity(linear_velocity);
        body->AdjustAngularVelocity(angular_velocity);
    }
}

void KinematicActuator::Finish(EntityNode& node)
{
    if (auto* body = node.GetRigidBody())
    {
        body->AdjustLinearVelocity(mClass->GetEndLinearVelocity());
        body->AdjustAngularVelocity(mClass->GetEndAngularVelocity());
    }
}

void SetFlagActuator::Start(EntityNode& node)
{
    if (!CanApply(node, true /*verbose*/))
        return;

    const auto* draw = node.GetDrawable();
    const auto* body = node.GetRigidBody();
    const auto* text = node.GetTextItem();

    using FlagName = SetFlagActuatorClass::FlagName;
    const auto flag = mClass->GetFlagName();

    if (flag == FlagName::Drawable_VisibleInGame)
        mStartState = draw->TestFlag(DrawableItem::Flags::VisibleInGame);
    else if (flag == FlagName::Drawable_UpdateMaterial)
        mStartState = draw->TestFlag(DrawableItem::Flags::UpdateMaterial);
    else if (flag == FlagName::Drawable_UpdateDrawable)
        mStartState = draw->TestFlag(DrawableItem::Flags::UpdateDrawable);
    else if (flag == FlagName::Drawable_Restart)
        mStartState = draw->TestFlag(DrawableItem::Flags::RestartDrawable);
    else if (flag == FlagName::Drawable_FlipHorizontally)
        mStartState = draw->TestFlag(DrawableItem::Flags::FlipHorizontally);
    else if (flag == FlagName::RigidBody_Bullet)
        mStartState = body->TestFlag(RigidBodyItem::Flags::Bullet);
    else if (flag == FlagName::RigidBody_Sensor)
        mStartState = body->TestFlag(RigidBodyItem::Flags::Sensor);
    else if (flag == FlagName::RigidBody_Enabled)
        mStartState = body->TestFlag(RigidBodyItem::Flags::Enabled);
    else if (flag == FlagName::RigidBody_CanSleep)
        mStartState = body->TestFlag(RigidBodyItem::Flags::CanSleep);
    else if (flag == FlagName::RigidBody_DiscardRotation)
        mStartState = body->TestFlag(RigidBodyItem::Flags::DiscardRotation);
    else if (flag == FlagName::TextItem_VisibleInGame)
        mStartState = text->TestFlag(TextItem::Flags::VisibleInGame);
    else if (flag == FlagName::TextItem_Blink)
        mStartState = text->TestFlag(TextItem::Flags::BlinkText);
    else if (flag == FlagName::TextItem_Underline)
        mStartState = text->TestFlag(TextItem::Flags::UnderlineText);
    else BUG("Unhandled flag in set flag actuator.");
}
void SetFlagActuator::Apply(EntityNode& node, float t)
{
    // no op.
}
void SetFlagActuator::Finish(EntityNode& node)
{
    if (!CanApply(node, false))
        return;

    bool next_value = false;
    const auto action = mClass->GetFlagAction();
    if (action == FlagAction::Toggle)
        next_value = !mStartState;
    else if (action == FlagAction::On)
        next_value = true;
    else if (action == FlagAction::Off)
        next_value = false;

    auto* draw = node.GetDrawable();
    auto* body = node.GetRigidBody();
    auto* text = node.GetTextItem();

    using FlagName = SetFlagActuatorClass::FlagName;
    const auto flag = mClass->GetFlagName();

    if (flag == FlagName::Drawable_VisibleInGame)
        draw->SetFlag(DrawableItem::Flags::VisibleInGame, next_value);
    else if (flag == FlagName::Drawable_UpdateMaterial)
        draw->SetFlag(DrawableItem::Flags::UpdateMaterial, next_value);
    else if (flag == FlagName::Drawable_UpdateDrawable)
        draw->SetFlag(DrawableItem::Flags::UpdateDrawable, next_value);
    else if (flag == FlagName::Drawable_Restart)
        draw->SetFlag(DrawableItem::Flags::RestartDrawable, next_value);
    else if (flag == FlagName::Drawable_FlipHorizontally)
        draw->SetFlag(DrawableItem::Flags::FlipHorizontally, next_value);
    else if (flag == FlagName::RigidBody_Bullet)
        body->SetFlag(RigidBodyItem::Flags::Bullet, next_value);
    else if (flag == FlagName::RigidBody_Sensor)
        body->SetFlag(RigidBodyItem::Flags::Sensor, next_value);
    else if (flag == FlagName::RigidBody_Enabled)
        body->SetFlag(RigidBodyItem::Flags::Enabled, next_value);
    else if (flag == FlagName::RigidBody_CanSleep)
        body->SetFlag(RigidBodyItem::Flags::CanSleep, next_value);
    else if (flag == FlagName::RigidBody_DiscardRotation)
        body->SetFlag(RigidBodyItem::Flags::DiscardRotation, next_value);
    else if (flag == FlagName::TextItem_VisibleInGame)
        text->SetFlag(TextItem::Flags::VisibleInGame, next_value);
    else if (flag == FlagName::TextItem_Blink)
        text->SetFlag(TextItem::Flags::BlinkText, next_value);
    else if (flag == FlagName::TextItem_Underline)
        text->SetFlag(TextItem::Flags::UnderlineText, next_value);
    else BUG("Unhandled flag in set flag actuator.");

    // spams the log
    // DEBUG("Set EntityNode '%1' flag '%2' to '%3'.", node.GetName(), flag, next_value ? "On" : "Off");
}

bool SetFlagActuator::CanApply(EntityNode& node, bool verbose) const
{
    auto* draw = node.GetDrawable();
    auto* body = node.GetRigidBody();
    auto* text = node.GetTextItem();

    using FlagName = SetFlagActuatorClass::FlagName;
    const auto flag = mClass->GetFlagName();

    if (flag == FlagName::Drawable_VisibleInGame ||
        flag == FlagName::Drawable_UpdateMaterial ||
        flag == FlagName::Drawable_UpdateDrawable ||
        flag == FlagName::Drawable_Restart ||
        flag == FlagName::Drawable_FlipHorizontally)
    {
        if (!draw && verbose)
        {
            WARN("EntityNode '%1' doesn't have a drawable item.", node.GetName());
            WARN("Setting a drawable flag '%1' will have no effect.", flag);
        }
        return draw != nullptr;
    }
    else if (flag == FlagName::RigidBody_Bullet ||
             flag == FlagName::RigidBody_Sensor ||
             flag == FlagName::RigidBody_Enabled ||
             flag == FlagName::RigidBody_CanSleep ||
             flag == FlagName::RigidBody_DiscardRotation)
    {
        if (!body && verbose)
        {
            WARN("EntityNode '%1' doesn't have a rigid body.", node.GetName());
            WARN("Setting a rigid body flag '%1' will have no effect.", flag);
        }
        return body != nullptr;
    }
    else if (flag == FlagName::TextItem_VisibleInGame ||
             flag == FlagName::TextItem_Underline ||
             flag == FlagName::TextItem_Blink)
    {
        if (!text && verbose)
        {
            WARN("EntityNode '%1' doesn't have a text item.", node.GetName());
            WARN("Setting a text item flag '%1' will have no effect.", flag);
        }
        return text != nullptr;
    }
    else BUG("Unhandled flag in set flag actuator.");
    return true;
}

void SetValueActuator::Start(EntityNode& node)
{
    if (!CanApply(node, true /*verbose */))
        return;

    const auto param = mClass->GetParamName();
    const auto* draw = node.GetDrawable();
    const auto* body = node.GetRigidBody();
    const auto* text = node.GetTextItem();

    if (param == ParamName::DrawableTimeScale)
        mStartValue = draw->GetTimeScale();
    else if (param == ParamName::AngularVelocity)
        mStartValue = body->GetAngularVelocity();
    else if (param == ParamName::LinearVelocityX)
        mStartValue = body->GetLinearVelocity().x;
    else if (param == ParamName::LinearVelocityY)
        mStartValue = body->GetLinearVelocity().y;
    else if (param == ParamName::LinearVelocity)
        mStartValue = body->GetLinearVelocity();
    else if (param == ParamName::TextItemText)
        mStartValue = text->GetText();
    else if (param == ParamName::TextItemColor)
        mStartValue = text->GetTextColor();
    else BUG("Unhandled node item value.");
}

void SetValueActuator::Apply(EntityNode& node, float t)
{
    if (!CanApply(node, false /*verbose*/))
        return;

    const auto method = mClass->GetInterpolation();
    const auto param  = mClass->GetParamName();
    const auto end    = mClass->GetEndValue();
    const auto start  = mStartValue;

    auto* draw = node.GetDrawable();
    auto* body = node.GetRigidBody();
    auto* text = node.GetTextItem();

    if (param == ParamName::DrawableTimeScale)
        draw->SetTimeScale(Interpolate<float>(t));
    else if (param == ParamName::AngularVelocity)
        body->AdjustAngularVelocity(Interpolate<float>(t));
    else if (param == ParamName::LinearVelocityX)
    {
        auto velocity = body->GetLinearVelocity();
        velocity.x = Interpolate<float>(t);
        body->AdjustLinearVelocity(velocity);
    }
    else if (param == ParamName::LinearVelocityY)
    {
        auto velocity = body->GetLinearVelocity();
        velocity.y = Interpolate<float>(t);
        body->AdjustLinearVelocity(velocity);
    }
    else if (param == ParamName::LinearVelocity)
        body->AdjustLinearVelocity(Interpolate<glm::vec2>(t));
    else if (param == ParamName::TextItemColor)
        text->SetTextColor(Interpolate<Color4f>(t));
    else if (param == ParamName::TextItemText) {
        // intentionally empty, can't interpolate
    } else BUG("Unhandled value actuator param type.");
}

void SetValueActuator::Finish(EntityNode& node)
{
    if (!CanApply(node, false))
        return;

    auto* draw = node.GetDrawable();
    auto* body = node.GetRigidBody();
    auto* text = node.GetTextItem();

    const auto param = mClass->GetParamName();
    const auto end   = mClass->GetEndValue();

    if (param == ParamName::DrawableTimeScale)
        draw->SetTimeScale(std::get<float>(end));
    else if (param == ParamName::AngularVelocity)
        body->AdjustAngularVelocity(std::get<float>(end));
    else if (param == ParamName::LinearVelocityX)
    {
        auto velocity = body->GetLinearVelocity();
        velocity.x = std::get<float>(end);
        body->AdjustLinearVelocity(velocity);
    }
    else if (param == ParamName::LinearVelocityY)
    {
        auto velocity = body->GetLinearVelocity();
        velocity.y = std::get<float>(end);
        body->AdjustLinearVelocity(velocity);
    }
    else if (param == ParamName::LinearVelocity)
        body->AdjustLinearVelocity(std::get<glm::vec2>(end));
    else if (param == ParamName::TextItemColor)
        text->SetTextColor(std::get<Color4f>(end));
    else if (param == ParamName::TextItemText) {
        text->SetText(std::get<std::string>(mClass->GetEndValue()));
    } else BUG("Unhandled value actuator param type.");
}

bool SetValueActuator::CanApply(EntityNode& node, bool verbose) const
{
    const auto param = mClass->GetParamName();
    const auto* draw = node.GetDrawable();
    const auto* body = node.GetRigidBody();
    const auto* text = node.GetTextItem();

    if ((param == ParamName::DrawableTimeScale))
    {
        if (!draw && verbose)
        {
            WARN("EntityNode '%1' doesn't have a drawable item.", node.GetName());
            WARN("Setting drawable item value '%1' will have no effect.", param);
        }
        return draw != nullptr;
    }
    else if ((param == ParamName::LinearVelocityY ||
              param == ParamName::LinearVelocityX ||
              param == ParamName::LinearVelocity  ||
              param == ParamName::AngularVelocity))
    {
        if (!body && verbose)
        {
            WARN("EntityNode '%1' doesn't have a rigid body.", node.GetName());
            WARN("Setting rigid body value '%1' will have no effect.", param);
        }
        return body != nullptr;
    }
    else if ((param == ParamName::TextItemText ||
              param == ParamName::TextItemColor))
    {
        if (!text && verbose)
        {
            WARN("EntityNode '%1' doesn't have a text item.", node.GetName());
            WARN("Setting text item value '%1' will have no effect.", param);
        }
        return text != nullptr;
    } else BUG("Unhandled value actuator param type.");
    return false;
}

TransformActuator::TransformActuator(const std::shared_ptr<const TransformActuatorClass>& klass)
    : mClass(klass)
{
    if (!mClass->TestFlag(Flags::StaticInstance))
    {
        Instance instance;
        instance.end_position = mClass->GetEndPosition();
        instance.end_size     = mClass->GetEndSize();
        instance.end_scale    = mClass->GetEndScale();
        instance.end_rotation = mClass->GetEndRotation();
        mDynamicInstance      = instance;
    }
}

void TransformActuator::Start(EntityNode& node)
{
    mStartPosition = node.GetTranslation();
    mStartSize     = node.GetSize();
    mStartScale    = node.GetScale();
    mStartRotation = node.GetRotation();
}
void TransformActuator::Apply(EntityNode& node, float t)
{
    const auto& inst = GetInstance();

    // apply interpolated state on the node.
    const auto method = mClass->GetInterpolation();
    const auto& p = math::interpolate(mStartPosition, inst.end_position, t, method);
    const auto& s = math::interpolate(mStartSize,     inst.end_size,     t, method);
    const auto& r = math::interpolate(mStartRotation, inst.end_rotation, t, method);
    const auto& f = math::interpolate(mStartScale,    inst.end_scale,    t, method);
    node.SetTranslation(p);
    node.SetSize(s);
    node.SetRotation(r);
    node.SetScale(f);
}
void TransformActuator::Finish(EntityNode& node)
{
    const auto& inst = GetInstance();

    node.SetTranslation(inst.end_position);
    node.SetRotation(inst.end_rotation);
    node.SetSize(inst.end_size);
    node.SetScale(inst.end_scale);
}

void TransformActuator::SetEndPosition(const glm::vec2& pos)
{
    if (mClass->TestFlag(Flags::StaticInstance))
    {
        WARN("Ignoring transform actuator position set on static actuator instance. [name=%1]", mClass->GetName());
        return;
    }
    mDynamicInstance.value().end_position = pos;
}
void TransformActuator::SetEndScale(const glm::vec2& scale)
{
    if (mClass->TestFlag(Flags::StaticInstance))
    {
        WARN("Ignoring transform actuator scale set on static actuator instance. [name=%1]", mClass->GetName());
        return;
    }
    mDynamicInstance.value().end_position = scale;
}
void TransformActuator::SetEndSize(const glm::vec2& size)
{
    if (mClass->TestFlag(Flags::StaticInstance))
    {
        WARN("Ignoring transform actuator size set on static actuator instance. [name=%1]", mClass->GetName());
        return;
    }
    mDynamicInstance.value().end_position = size;
}
void TransformActuator::SetEndRotation(float angle)
{
    if (mClass->TestFlag(Flags::StaticInstance))
    {
        WARN("Ignoring transform actuator rotation set on static actuator instance. [name=%1]", mClass->GetName());
        return;
    }
    mDynamicInstance.value().end_rotation = angle;
}

TransformActuator::Instance TransformActuator::GetInstance() const
{
    if (mDynamicInstance.has_value())
        return mDynamicInstance.value();
    Instance inst;
    inst.end_size     = mClass->GetEndSize();
    inst.end_scale    = mClass->GetEndScale();
    inst.end_rotation = mClass->GetEndRotation();
    inst.end_position = mClass->GetEndPosition();
    return inst;
}

void MaterialActuator::Start(EntityNode& node)
{
    const auto& name = mClass->GetParamName();
    const auto* draw = node.GetDrawable();
    if (draw == nullptr)
    {
        WARN("EntityNode '%1' doesn't have a drawable item.", node.GetName());
        WARN("Setting a material parameter '%1' will have no effect.", name);
        return;
    }
    if (const auto* p = draw->FindMaterialParam(name))
        mStartValue = *p;
    else WARN("EntityNode '%1' drawable doesn't have such material param '%2'.", node.GetName(), name);
}
void MaterialActuator::Apply(EntityNode& node, float t)
{
    if (auto* draw = node.GetDrawable())
    {
        const auto& name = mClass->GetParamName();
        const auto& end  = mClass->GetParamValue();
        if (std::holds_alternative<int>(end))
            draw->SetMaterialParam(name, Interpolate<int>(t));
        else if (std::holds_alternative<float>(end))
            draw->SetMaterialParam(name, Interpolate<float>(t));
        else if (std::holds_alternative<glm::vec2>(end))
            draw->SetMaterialParam(name, Interpolate<glm::vec2>(t));
        else if (std::holds_alternative<glm::vec3>(end))
            draw->SetMaterialParam(name, Interpolate<glm::vec3>(t));
        else if (std::holds_alternative<glm::vec4>(end))
            draw->SetMaterialParam(name, Interpolate<glm::vec4>(t));
        else if (std::holds_alternative<Color4f>(end))
            draw->SetMaterialParam(name, Interpolate<Color4f>(t));
        else BUG("Unhandled material parameter type.");
    }
}
void MaterialActuator::Finish(EntityNode& node)
{
    if (auto* draw = node.GetDrawable())
    {
        const auto& name = mClass->GetParamName();
        const auto& end  = mClass->GetParamValue();
        draw->SetMaterialParam(name, end);
    }
}

AnimationClass::AnimationClass()
{
    mId = base::RandomString(10);
}

AnimationClass::AnimationClass(const AnimationClass& other)
{
    for (const auto& a : other.mActuators)
    {
        mActuators.push_back(a->Copy());
    }
    mId       = other.mId;
    mName     = other.mName;
    mDuration = other.mDuration;
    mLooping  = other.mLooping;
    mDelay    = other.mDelay;
}
AnimationClass::AnimationClass(AnimationClass&& other)
{
    mId        = std::move(other.mId);
    mActuators = std::move(other.mActuators);
    mName      = std::move(other.mName);
    mDuration  = other.mDuration;
    mLooping   = other.mLooping;
    mDelay     = other.mDelay;
}

void AnimationClass::DeleteActuator(size_t index)
{
    ASSERT(index < mActuators.size());
    auto it = mActuators.begin();
    std::advance(it, index);
    mActuators.erase(it);
}

bool AnimationClass::DeleteActuatorById(const std::string& id)
{
    for (auto it = mActuators.begin(); it != mActuators.end(); ++it)
    {
        if ((*it)->GetId() == id) {
            mActuators.erase(it);
            return true;
        }
    }
    return false;
}
ActuatorClass* AnimationClass::FindActuatorById(const std::string& id)
{
    for (auto& actuator : mActuators) {
        if (actuator->GetId() == id)
            return actuator.get();
    }
    return nullptr;
}
const ActuatorClass* AnimationClass::FindActuatorById(const std::string& id) const
{
    for (auto& actuator : mActuators) {
        if (actuator->GetId() == id)
            return actuator.get();
    }
    return nullptr;
}

std::unique_ptr<Actuator> AnimationClass::CreateActuatorInstance(size_t i) const
{
    const auto& klass = mActuators[i];
    if (klass->GetType() == ActuatorClass::Type::Transform)
        return std::make_unique<TransformActuator>(std::static_pointer_cast<TransformActuatorClass>(klass));
    else if (klass->GetType() == ActuatorClass::Type::SetValue)
        return std::make_unique<SetValueActuator>(std::static_pointer_cast<SetValueActuatorClass>(klass));
    else if (klass->GetType() == ActuatorClass::Type::Kinematic)
        return std::make_unique<KinematicActuator>(std::static_pointer_cast<KinematicActuatorClass>(klass));
    else if (klass->GetType() == ActuatorClass::Type::SetFlag)
        return std::make_unique<SetFlagActuator>(std::static_pointer_cast<SetFlagActuatorClass>(klass));
    else if (klass->GetType() == ActuatorClass::Type::Material)
        return std::make_unique<MaterialActuator>(std::static_pointer_cast<MaterialActuatorClass>(klass));
    else BUG("Unknown actuator type");
    return {};
}

std::size_t AnimationClass::GetHash() const
{
    std::size_t hash = 0;
    hash = base::hash_combine(hash, mId);
    hash = base::hash_combine(hash, mName);
    hash = base::hash_combine(hash, mDuration);
    hash = base::hash_combine(hash, mLooping);
    hash = base::hash_combine(hash, mDelay);
    for (const auto& actuator : mActuators)
        hash = base::hash_combine(hash, actuator->GetHash());
    return hash;
}

void AnimationClass::IntoJson(data::Writer& data) const
{
    data.Write("id", mId);
    data.Write("name", mName);
    data.Write("duration", mDuration);
    data.Write("delay", mDelay);
    data.Write("looping", mLooping);
    for (const auto &actuator : mActuators)
    {
        auto meta = data.NewWriteChunk();
        auto act  = data.NewWriteChunk();
        actuator->IntoJson(*act);
        meta->Write("type", actuator->GetType());
        meta->Write("actuator", std::move(act));
        data.AppendChunk("actuators", std::move(meta));
    }
}

// static
std::optional<AnimationClass> AnimationClass::FromJson(const data::Reader& data)
{
    AnimationClass ret;
    if (!data.Read("id", &ret.mId) ||
        !data.Read("name", &ret.mName) ||
        !data.Read("duration", &ret.mDuration) ||
        !data.Read("delay", &ret.mDelay) ||
        !data.Read("looping", &ret.mLooping))
        return std::nullopt;

    for (unsigned i=0; i<data.GetNumChunks("actuators"); ++i)
    {
        const auto& meta = data.GetReadChunk("actuators", i);
        ActuatorClass::Type type;
        if (!meta->Read("type", &type))
            return std::nullopt;
        std::shared_ptr<ActuatorClass> actuator;
        if (type == ActuatorClass::Type::Transform)
            actuator = std::make_shared<TransformActuatorClass>();
        else if (type == ActuatorClass::Type::SetValue)
            actuator = std::make_shared<SetValueActuatorClass>();
        else if (type == ActuatorClass::Type::Kinematic)
            actuator = std::make_shared<KinematicActuatorClass>();
        else if (type == ActuatorClass::Type::SetFlag)
            actuator = std::make_shared<SetFlagActuatorClass>();
        else if (type == ActuatorClass::Type::Material)
            actuator = std::make_shared<MaterialActuatorClass>();
        else BUG("Unknown actuator type.");

        const auto& act = meta->GetReadChunk("actuator");
        if (!act || !actuator->FromJson(*act))
            return std::nullopt;
        ret.mActuators.push_back(actuator);
    }
    return ret;
}

AnimationClass AnimationClass::Clone() const
{
    AnimationClass ret;
    ret.mName     = mName;
    ret.mDuration = mDuration;
    ret.mLooping  = mLooping;
    ret.mDelay    = mDelay;
    for (const auto& klass : mActuators)
        ret.mActuators.push_back(klass->Clone());
    return ret;
}

AnimationClass& AnimationClass::operator=(const AnimationClass& other)
{
    if (this == &other)
        return *this;
    AnimationClass copy(other);
    std::swap(mId, copy.mId);
    std::swap(mActuators, copy.mActuators);
    std::swap(mName, copy.mName);
    std::swap(mDuration, copy.mDuration);
    std::swap(mLooping, copy.mLooping);
    std::swap(mDelay, copy.mDelay);
    return *this;
}

Animation::Animation(const std::shared_ptr<const AnimationClass>& klass)
    : mClass(klass)
{
    for (size_t i=0; i<mClass->GetNumActuators(); ++i)
    {
        NodeTrack track;
        track.actuator = mClass->CreateActuatorInstance(i);
        track.node     = track.actuator->GetNodeId();
        track.ended    = false;
        track.started  = false;
        mTracks.push_back(std::move(track));
    }
    mDelay = klass->GetDelay();
    // start at negative delay time, then the actual animation playback
    // starts after the current time reaches 0 and all of the delay
    // has been "consumed".
    mCurrentTime = -mDelay;
}
Animation::Animation(const AnimationClass& klass)
    : Animation(std::make_shared<AnimationClass>(klass))
    {}

Animation::Animation(const Animation& other) : mClass(other.mClass)
{
    for (size_t i=0; i<other.mTracks.size(); ++i)
    {
        NodeTrack track;
        track.node     = other.mTracks[i].node;
        track.actuator = other.mTracks[i].actuator->Copy();
        track.ended    = other.mTracks[i].ended;
        track.started  = other.mTracks[i].started;
        mTracks.push_back(std::move(track));
    }
    mCurrentTime = other.mCurrentTime;
    mDelay       = other.mDelay;
}
    // Move ctor.
Animation::Animation(Animation&& other)
{
    mClass       = other.mClass;
    mCurrentTime = other.mCurrentTime;
    mDelay       = other.mDelay;
    mTracks      = std::move(other.mTracks);
}

void Animation::Update(float dt)
{
    const auto duration = mClass->GetDuration();

    mCurrentTime = math::clamp(-mDelay, duration, mCurrentTime + dt);
}

void Animation::Apply(EntityNode& node) const
{
    // if we're delaying then skip until delay is consumed.
    if (mCurrentTime < 0)
        return;
    const auto duration = mClass->GetDuration();
    const auto pos = mCurrentTime / duration;

    // todo: keep the tracks in some smarter data structure or perhaps
    // in a sorted vector and then binary search.
    for (auto& track : mTracks)
    {
        if (track.node != node.GetClassId())
            continue;

        const auto start = track.actuator->GetStartTime();
        const auto len   = track.actuator->GetDuration();
        const auto end   = math::clamp(0.0f, 1.0f, start + len);
        if (pos < start)
            continue;
        else if (pos >= end)
        {
            if (!track.ended)
            {
                track.actuator->Finish(node);
                track.ended = true;
            }
            continue;
        }
        if (!track.started)
        {
            track.actuator->Start(node);
            track.started = true;
        }
        const auto t = math::clamp(0.0f, 1.0f, (pos - start) / len);
        track.actuator->Apply(node, t);
    }
}

void Animation::Restart()
{
    for (auto& track : mTracks)
    {
        ASSERT(track.started);
        ASSERT(track.ended);
        track.started = false;
        track.ended   = false;
    }
    mCurrentTime = -mDelay;
}

bool Animation::IsComplete() const
{
    for (const auto& track : mTracks)
    {
        if (!track.ended)
            return false;
    }
    if (mCurrentTime >= mClass->GetDuration())
        return true;
    return false;
}

Actuator* Animation::FindActuatorById(const std::string& id)
{
    for (auto& item : mTracks)
    {
        if (item.actuator->GetClassId() == id)
            return item.actuator.get();
    }
    return nullptr;
}
Actuator* Animation::FindActuatorByName(const std::string& name)
{
    for (auto& item : mTracks)
    {
        if (item.actuator->GetClassName() == name)
            return item.actuator.get();
    }
    return nullptr;
}

const Actuator* Animation::FindActuatorById(const std::string& id) const
{
    for (auto& item : mTracks)
    {
        if (item.actuator->GetClassId() == id)
            return item.actuator.get();
    }
    return nullptr;
}
const Actuator* Animation::FindActuatorByName(const std::string& name) const
{
    for (auto& item : mTracks)
    {
        if (item.actuator->GetClassName() == name)
            return item.actuator.get();
    }
    return nullptr;
}

std::unique_ptr<Animation> CreateAnimationInstance(std::shared_ptr<const AnimationClass> klass)
{
    return std::make_unique<Animation>(klass);
}

} // namespace
