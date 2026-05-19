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
		ResetAmount();

		Reset();
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
		Next(NextPointer);
		uint64 After = reinterpret_cast<uint64>(Marker);

		IncreaseAmount(After - Before);

		return Pointer;
	}

	void* StackAllocator::Reallocate(void* Pointer, uint64 Size, uint64 Alignement)
	{
		NX_ASSERT(false, Default, "Reallocate from Stack Allocator is not supported");
		return nullptr;
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
		Previous(Pointer);
		uint64 After = reinterpret_cast<uint64>(Marker);

		EraseMemory(Marker, FreeAmount());

		DecreaseAmount(Before - After);
	}

	void StackAllocator::Next(void* Pointer)
	{
		Marker = Pointer;
	}

	void StackAllocator::Previous(void* Pointer)
	{
		Marker = Memory::UnalignPointer(Pointer);
	}

	void StackAllocator::Reset()
	{
		Marker = GetMemoryBlock();
	}
}
