#include "Core/NexusEnginePch.h"
#include "StackAllocator.h"

namespace NxEn
{
	StackAllocator::StackAllocator(uint64 Size)
		: Allocator(Size)
	{
		Clear();
	}

	StackAllocator::~StackAllocator()
	{
	}
	
	void* StackAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		NEXUS_ASSERT(CanAllocate(Size, Alignement), "Not enough space")

		uint64 Before = reinterpret_cast<uint64>(Marker);

		void* Pointer = Memory::AlignPointer(Marker, Alignement);
		Marker = Memory::OffsetPointer(Pointer, Size);
		
		uint64 After = reinterpret_cast<uint64>(Marker);

		UpdateAmount(After - Before, true);
		
		return Pointer;
	}

	void* StackAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		NEXUS_ASSERT(false, "Reallocate from Stack Allocator is not supported")
		return nullptr;
	}

	void StackAllocator::Free(void* Pointer)
	{
		if (!Pointer)
		{
			return;
		}

		NEXUS_ASSERT(IsValidAddress(Pointer), "Address is outside of the stack")

		uint64 Before = reinterpret_cast<uint64>(Marker);

		Marker = Memory::UnalignPointer(Pointer);

		uint64 After = reinterpret_cast<uint64>(Marker);

		UpdateAmount(Before - After, false);
		EraseMemory(Marker, FreeAmount());
	}

	void StackAllocator::Clear()
	{
		WipeoutMemory();
		ResetAmount();

		Marker = GetMemoryBlock();
	}

	bool StackAllocator::CanAllocate(uint64 Size, uint64 Alignement) const
	{
		if (FreeAmount() < Size)
		{
			return false;
		}

		uint64 Current = reinterpret_cast<uint64>(Marker);
		uint64 Aligned = Memory::AlignAddress(Current, Alignement);
		if (Current == Aligned)
		{
			Aligned += Alignement;
		}
		uint64 Address = Aligned + Size;

		uint64 Start = reinterpret_cast<uint64>(GetMemoryBlock());
		uint64 End = Start + TotalAmount();

		return Address < End;
	}

	bool StackAllocator::IsValidAddress(void* Pointer) const
	{
		NEXUS_ASSERT(Pointer != nullptr, "Pointer is null")

		uint64 Address = reinterpret_cast<uint64>(Pointer);
		uint64 Current = reinterpret_cast<uint64>(Marker);

		uint64 Start = reinterpret_cast<uint64>(GetMemoryBlock());
		uint64 End = Start + TotalAmount();

		return Address < Current && Address >= Start && Address < End;
	}
}
