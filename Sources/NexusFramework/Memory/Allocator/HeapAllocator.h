#pragma once

#include "NexusFramework/Memory/Allocator/Allocator.h"

namespace NxFr
{
	class HandleManager;

    class HeapAllocator : public Allocator
    {
	private:
		struct HeapSlot
		{
			HeapSlot* Next;
			bool Free;
		};

    public:
        NEXUS_FRAMEWORK_API HeapAllocator(uint64 Size);
		NEXUS_FRAMEWORK_API HeapAllocator(const HeapAllocator& Other) = delete;
		NEXUS_FRAMEWORK_API HeapAllocator(HeapAllocator&& Other) noexcept = delete;
        NEXUS_FRAMEWORK_API virtual ~HeapAllocator();

		NEXUS_FRAMEWORK_API HeapAllocator& operator=(const HeapAllocator& Other) = delete;
		NEXUS_FRAMEWORK_API HeapAllocator& operator=(HeapAllocator&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API void Clear() override;
		NEXUS_FRAMEWORK_API bool CanAllocate(uint64 Size, uint64 Alignement) const override;
		NEXUS_FRAMEWORK_API bool BelongToAllocator(void* Pointer) const override;

		NEXUS_FRAMEWORK_API void Defragment(HandleManager* Manager);
		NEXUS_FRAMEWORK_API void Defragment(HandleManager* Manager, float Time);
		NEXUS_FRAMEWORK_API void Defragment(HandleManager* Manager, uint64 Count);

		NEXUS_FRAMEWORK_API virtual bool IsEmpty() const override { return UsedAmount() == sizeof(HeapSlot); };

	protected:
		void* Allocate(uint64 Size, uint64 Alignement) override;
		void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) override;
		void Free(void* Pointer) override;

    private:
        void UpdateHeapSlot(HeapSlot* Slot, uint64 Size);
        void RemoveNextHeapSlot(HeapSlot* Slot);
		HeapSlot* GetHeapSlot(void* Pointer) const;
		HeapSlot* GetHeapSlot(uint64 Size) const;
        void* GetHeapSlotMemory(HeapSlot* Slot) const;
		uint64 GetHeapSlotSize(HeapSlot* Slot) const;
		uint64 GetAlignedSize(uint64 Size) const;
		void Reset();

		void Defragment(HandleManager* Manager, float Time, uint64 Count);

	private:
		HeapSlot* Root;
		mutable HeapSlot* Cache;
    };
}

