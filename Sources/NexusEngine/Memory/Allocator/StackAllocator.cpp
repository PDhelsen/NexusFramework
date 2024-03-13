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
		void* Pointer = Memory::AlignPointer(Marker, Alignement);
		Marker = Memory::OffsetPointer(Pointer, Size);
		UpdateAmount();
		return Pointer;
	}

	void StackAllocator::Free(void* Pointer)
	{
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

	void StackAllocator::UpdateAmount()
	{
		uint64 Start = reinterpret_cast<uint64>(GetPointer());
		uint64 Current = reinterpret_cast<uint64>(Marker);
		SetAmount(Current - Start);
	}
}