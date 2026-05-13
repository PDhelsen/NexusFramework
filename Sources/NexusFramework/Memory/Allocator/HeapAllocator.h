#pragma once

#include "NexusFramework/Memory/Allocator/MemoryAllocator.h"

namespace NxFr
{
	class HandleManager;

    class NEXUS_FRAMEWORK_API HeapAllocator : public MemoryAllocator
    {
    public:
		NEXUS_NOCOPY_NOMOVE(HeapAllocator)
		HeapAllocator(uint64 Size);
        virtual ~HeapAllocator();

		void Clear() override;
		bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		bool BelongToAllocator(void* Pointer) const override;

		void Defragment(HandleManager* Manager);
		void Defragment(HandleManager* Manager, float Time);
		void Defragment(HandleManager* Manager, uint64 Count);

		virtual bool IsEmpty() const override { return UsedAmount() == sizeof(HeapSlot); };

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

    private:
		struct HeapSlot
		{
			HeapSlot* Next;
			bool Free;
		};

        void UpdateHeapSlot(HeapSlot* Slot, uint64 Size);
        void RemoveNextHeapSlot(HeapSlot* Slot);
		HeapSlot* GetHeapSlot(void* Pointer) const;
		HeapSlot* GetHeapSlot(uint64 Size) const;
        void* GetHeapSlotMemory(HeapSlot* Slot) const;
		uint64 GetHeapSlotSize(HeapSlot* Slot) const;
		uint64 GetAlignedSize(uint64 Size) const;
		void Reset();

		void Defragment(HandleManager* Manager, float Time, uint64 Count);

		HeapSlot* Root;
		mutable HeapSlot* Cache;
    };
}

