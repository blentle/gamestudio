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

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include "base/assert.h"

namespace mem
{
    inline size_t align(size_t size, size_t boundary = sizeof(intptr_t))
    {
        return (size + boundary - 1) & ~(boundary - 1);
    }

    // Allocate raw memory from the heap using malloc.
    template<size_t InternalAllocSize>
    class HeapAllocator
    {
    public:
        HeapAllocator(std::size_t bytes)
        {
            mMemory = (std::uint8_t*)std::malloc(bytes);
            if (mMemory == nullptr)
                throw std::runtime_error("heap allocator no memory");
        }
        HeapAllocator(const HeapAllocator&) = delete;
       ~HeapAllocator()
        { std::free(mMemory); }

        void* MapMem(size_t offset)
        { return mMemory + offset; }

        HeapAllocator& operator=(const HeapAllocator&) = delete;
    protected:
        // re-using the underlying memory block for internal
        // book keeping allocations.
        inline void* AllocateInternal(size_t index, size_t offset)
        { return mMemory + offset; }
        inline void FreeInternal(size_t index, size_t offset, void* mem)
        { /* intentionally empty */ }
    private:
        std::uint8_t* mMemory = nullptr;
    };

    namespace detail {
        // Some bits for memory management book keeping.
        struct MemoryPoolAllocHeader {
            // todo: flags used for anything if needed
            std::uint32_t flags  : 8;
            // offset into the memory buffer.
            std::uint32_t offset : 24;
        };
        struct MemoryPoolAllocNode {
            MemoryPoolAllocHeader header;
            MemoryPoolAllocNode* next = nullptr;
        };
    } // detail

    // Implement pooled space management algorithm on top
    // of a some allocated/reserved space such as VBO or
    // heap allocated memory block. Fundamentally the
    // underlying space doesn't need to be CPU addressable
    // therefore the regions are managed through offsets
    // into the allocated space. The allocator based object
    // is also used for allocating the internal book keeping
    // nodes for the free list. This allows the allocator
    // to double its allocated memory buffer as the backing
    // store of the free list nodes when applicable.
    template<template<size_t InternalAllocSize> class Allocator, size_t ObjectSize>
    class MemoryPool : public Allocator<sizeof(detail::MemoryPoolAllocNode)>
    {
    public:
        using AllocNode     = detail::MemoryPoolAllocNode;
        using AllocHeader   = detail::MemoryPoolAllocHeader;
        using AllocatorBase = Allocator<sizeof(AllocNode)>;
        // todo: fix this if needed.
        static_assert(ObjectSize >= sizeof(AllocNode));

        // Construct pool with maximum pool size capacity of objects.
        MemoryPool(std::size_t pool_size)
          : AllocatorBase(pool_size * ObjectSize)
          , mPoolSize(pool_size)
        {
            const auto max_offset = pool_size * ObjectSize;
            // only 24 bits are reserved for addressing the memory buffer.
            // so the total space should not exceed that.
            ASSERT(max_offset <= ((1 << 24)-1));

            // Construct the initial free list of allocation nodes.
            for (size_t i=0; i<pool_size; ++i)
            {
                auto* node = NewNode(i, i * ObjectSize);
                node->header.flags  = 0;
                node->header.offset = i * ObjectSize;
                AddListNode(node);
            }
        }
        // Try to allocate a new block of memory (space) in the underlying
        // memory allocator object. Returns true if successful and block
        // contains the memory details where the space has been allocated.
        // If no more space was available returns false.
        bool Allocate(AllocHeader* block)
        {
            auto* next = GetNextNode();
            if (!next)
                return false;
            *block = next->header;
            DeleteNode(next);
            return true;
        }
        // Return a block of space back into the pool.
        void Free(const AllocHeader& block)
        {
            auto* node = NewNode(block.offset / ObjectSize, block.offset);
            node->header.flags  = 0;
            node->header.offset = block.offset;
            AddListNode(node);
        }
    private:
        AllocNode* NewNode(size_t index, size_t offset)
        {
            void* mem = AllocatorBase::AllocateInternal(index, offset);
            return new(mem) AllocNode;
        }
        void DeleteNode(AllocNode* node)
        {
            const auto header = node->header;
            // using the placement new in NewNode means we're just
            // going to call the dtor manually here.
            node->~AllocNode();
            AllocatorBase::FreeInternal(header.offset / ObjectSize, header.offset, (void*)node);
        }

        // Get next allocation node from the free list.
        // Returns nullptr if there are no more blocks.
        AllocNode* GetNextNode()
        {
            if (mFreeList == nullptr)
                return nullptr;
            auto* next = mFreeList;
            mFreeList = mFreeList->next;
            return next;
        }
        // Add a new node to the free list.
        void AddListNode(AllocNode* node)
        {
            if (mFreeList == nullptr) {
                node->next = nullptr;
                mFreeList = node;
            } else {
                node->next = mFreeList;
                mFreeList = node;
            }
        }
    private:
        // current maximum pool size.
        size_t mPoolSize = 0;
        // The free list of allocation nodes each identifying
        // a chunk of free space in the underlying allocator's
        // memory chunk / buffer.
        AllocNode* mFreeList = nullptr;
    };

    // This allocation strategy never de-allocates any individual blocks
    // but only the whole allocation can be freed.
    class BumpAllocator
    {
    public:
        BumpAllocator(size_t bytes)
          : mSize(bytes)
        {}
        bool Allocate(size_t bytes, size_t* offset)
        {
            if (GetFreeBytes() < bytes)
                return false;
            *offset = mOffset;
            mOffset += bytes;
            return true;
        }
        void Reset()
        { mOffset = 0; }
        size_t GetFreeBytes() const
        { return mSize - mOffset; }
        size_t GetCapacity() const
        { return mSize; }
        size_t GetUsedBytes() const
        { return mOffset; }
    private:
        const size_t mSize = 0;
        size_t mOffset = 0;
    };

    class HeapBumpAllocator
    {
    public:
        HeapBumpAllocator(size_t bytes)
            : mAllocator(bytes)
            , mHeap(bytes)
        {}
        void* Allocate(size_t bytes)
        {
            bytes = mem::align(bytes, sizeof(intptr_t));
            size_t offset = 0;
            if (mAllocator.Allocate(bytes, &offset))
                return nullptr;
            void* mem = mHeap.MapMem(offset);
            return mem;
        }
        void Reset()
        { mAllocator.Reset(); }
        size_t GetFreeBytes() const
        { return mAllocator.GetFreeBytes(); }
        size_t GetCapacity() const
        { return mAllocator.GetCapacity(); }
        size_t GetUsedBytes() const
        { return mAllocator.GetUsedBytes(); }
    private:
        BumpAllocator mAllocator;
        HeapAllocator<0> mHeap;
    };

    // Fixed allocator (in terms of allocation size) interface.
    class IFixedAllocator
    {
    public:
        virtual ~IFixedAllocator() = default;
        virtual void* Allocate() noexcept = 0;
        virtual void Free(void* mem) noexcept = 0;
    private:

    };

    // Wrapper for combining heap based memory allocation
    // with pool based memory management strategy.
    template<size_t ObjectSize>
    class HeapMemoryPool : public IFixedAllocator
    {
    public:
        HeapMemoryPool(size_t pool_size)
          : mPoolSize(pool_size)
          , mPool(pool_size)
        {}
        virtual void* Allocate() noexcept override
        {
            detail::MemoryPoolAllocHeader block;
            if (!mPool.Allocate(&block))
                return nullptr;
            void* mem = mPool.MapMem(block.offset);
            // copy the book keeping information into a fixed memory
            // location in the returned mem pointer for convenience.
            std::memcpy(mem, &block, sizeof(block));
            ++mAllocCount;
            return (uint8_t*)mem + sizeof(block);
        }
        virtual void Free(void* mem) noexcept override
        {
            detail::MemoryPoolAllocHeader block;
            std::memcpy(&block, (uint8_t*)mem - sizeof(block), sizeof(block));
            mPool.Free(block);
            --mAllocCount;
        }
        size_t GetAllocCount() const
        { return mAllocCount; }
        size_t GetFreeCount() const
        { return mPoolSize - mAllocCount; }
    private:
        static auto constexpr TotalSize   = ObjectSize + sizeof(detail::MemoryPoolAllocHeader);
        static auto constexpr Padding     = TotalSize % sizeof(intptr_t);
        static auto constexpr AlignedSize = TotalSize + (sizeof(intptr_t) - Padding);

        std::size_t mAllocCount = 0;
        std::size_t mPoolSize   = 0;
        // Use an object space that is larger than the actual
        // object so that the allocation header (which contains
        // the allocation details) can be baked into the actual
        // memory addresses returned by the allocate function.
        MemoryPool<HeapAllocator, AlignedSize> mPool;
    };

} // namespace
