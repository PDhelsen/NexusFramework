#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/StackAllocator.h"

namespace NxFr
{
	StackAllocator::StackAllocator(uint64 Size)
		: MemoryAllocator(Size), Marker(nullptr)
	{
		Clear();
	}

	StackAllocator::~StackAllocator()
	{
	}

	void StackAllocator::Clear()
	{
		WipeoutMemory();
		Marker = Data;
		Amount = 0;
	}

	bool StackAllocator::CanAllocate(uint64 Size, uint64 Alignement) const
	{
		void* Pointer = Memory::AlignPointer(Marker, Alignement);
		Pointer = Memory::OffsetPointer(Pointer, Size);
		return IsPointerInMemoryBlock(Pointer);
	}

	bool StackAllocator::BelongToAllocator(void* Pointer) const
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);
		uint64 Current = reinterpret_cast<uint64>(Marker);
		return Pointer && IsPointerInMemoryBlock(Pointer) && Address < Current;
	}

	void* StackAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		void* Pointer = Memory::AlignPointer(Marker, Alignement);
		void* NextPointer = Memory::OffsetPointer(Pointer, Size);

		NX_ASSERT(IsPointerInMemoryBlock(NextPointer), Default, "Allocator is full");

		uint64 Before = reinterpret_cast<uint64>(Marker);
		Marker = NextPointer;
		uint64 After = reinterpret_cast<uint64>(Marker);
		Amount += After - Before;

		return Pointer;
	}

	void* StackAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		// Stack allocator can only grow as free will deallocate everything past this object.
		return Allocate(Size, Alignement);
	}

	void StackAllocator::Free(void* Pointer)
	{
		if (!Pointer)
		{
			return;
		}

		NX_ASSERT(IsPointerInMemoryBlock(Pointer), Default, "Invalid pointer");

		uint64 Address = reinterpret_cast<uint64>(Pointer);
		uint64 Current = reinterpret_cast<uint64>(Marker);

		if (Address >= Current)
		{
			return;
		}

		uint64 Before = reinterpret_cast<uint64>(Marker);
		Marker = Memory::UnalignPointer(Pointer);
		uint64 After = reinterpret_cast<uint64>(Marker);
		Amount -= Before - After;

		EraseMemory(Marker, FreeAmount());
	}
}
