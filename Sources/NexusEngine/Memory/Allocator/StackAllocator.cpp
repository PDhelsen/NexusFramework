#include "Core/NexusEnginePch.h"
#include "StackAllocator.h"

namespace NxEn
{
	StackAllocator::StackAllocator(uint64 Size)
		: Allocator(Size)
	{
		Marker = GetPointer();
		UpdateAmount();
	}

	StackAllocator::~StackAllocator()
	{
	}
	
	void* StackAllocator::Allocate(uint64 Size, uint64 Alignement)
	{
		NEXUS_ASSERT(CanAllocate(Size, Alignement), "Stack Allocator Overflow");

		void* Pointer = Memory::AlignPointer(Marker, Alignement);
		Marker = Memory::OffsetPointer(Pointer, Size);
		UpdateAmount();
		
		return Pointer;
	}

	void StackAllocator::Free(void* Pointer)
	{
		NEXUS_ASSERT(ValidAddress(Pointer), "Address is outside of the stack");

		Marker = Memory::UnalignPointer(Pointer);
		UpdateAmount();
#if NEXUS_DEBUG
		Memory::MemSet(Marker, 0, FreeAmount());
#endif
	}

	void StackAllocator::Clear()
	{
		Marker = GetPointer();
		UpdateAmount();
#if NEXUS_DEBUG
		Memory::MemSet(Marker, 0, FreeAmount());
#endif
	}

	bool StackAllocator::CanAllocate(uint64 Size, uint64 Alignement) const
	{
		uint64 Current = reinterpret_cast<uint64>(Marker);
		uint64 Aligned = Memory::AlignAddress(Current, Alignement);
		if (Current == Aligned)
		{
			Aligned += Alignement;
		}
		uint64 Address = Aligned + Size;

		uint64 Start = reinterpret_cast<uint64>(GetPointer());
		uint64 End = Start + TotalAmount();

		return Address < End;
	}

	bool StackAllocator::ValidAddress(void* Pointer) const
	{
		NEXUS_ASSERT(Pointer != nullptr, "Pointer is null");

		uint64 Address = reinterpret_cast<uint64>(Pointer);
		uint64 Current = reinterpret_cast<uint64>(Marker);

		uint64 Start = reinterpret_cast<uint64>(GetPointer());
		uint64 End = Start + TotalAmount();

		return Address < Current && Address >= Start && Address < End;
	}

	void StackAllocator::UpdateAmount()
	{
		uint64 Start = reinterpret_cast<uint64>(GetPointer());
		uint64 Current = reinterpret_cast<uint64>(Marker);
		SetAmount(Current - Start);
	}
}