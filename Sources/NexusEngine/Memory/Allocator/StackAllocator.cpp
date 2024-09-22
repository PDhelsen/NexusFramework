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
		void* Pointer = Memory::AlignPointer(Marker, Alignement);
		void* Next = Memory::OffsetPointer(Pointer, Size);

		if (!IsPointerInside(Next))
		{
			NEXUS_ASSERT(false, "Allocator is full");
			return nullptr;
		}

		uint64 Before = reinterpret_cast<uint64>(Marker);
		Marker = Next;
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
		if (!IsAllocatedAddress(Pointer))
		{
			return;
		}

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
		void* Pointer = NextPointer(Marker, Size, Alignement);
		return IsPointerInside(Pointer);
	}

	bool StackAllocator::IsAllocatedAddress(void* Pointer) const
	{
		return Pointer != nullptr && IsPointerInside(Pointer) && !IsPointerFree(Pointer);
	}

	void* StackAllocator::NextPointer(void* Pointer, uint64 Size, uint64 Alignement) const
	{
		Pointer = Memory::AlignPointer(Pointer, Alignement);
		return Memory::OffsetPointer(Pointer, Size);
	}

	bool StackAllocator::IsPointerFree(void* Pointer) const
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);
		uint64 Current = reinterpret_cast<uint64>(Marker);
		return Address >= Current;
	}
}
