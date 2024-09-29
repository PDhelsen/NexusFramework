#include "Core/NexusEnginePch.h"
#include "Memory.h"

namespace NxEn
{
	GlobalAllocator* Memory::Global = new GlobalAllocator();
	StackAllocator* Memory::DefaultStack = new StackAllocator(NEXUS_STACK_SIZE);
	HeapAllocator* Memory::DefaultHeap = new HeapAllocator(NEXUS_HEAP_SIZE);
	Stack<Allocator*, 10>* Memory::Allocators = new Stack<Allocator*, 10>();

	void* Memory::Allocate(uint64 Size, Allocator* Allocator, uint64 Alignement)
	{
		if (Allocator != nullptr)
		{
			return Allocator->Allocate(Size, Alignement);
		}
		else
		{
			return Malloc(Size);
		}
	}

	void* Memory::Reallocate(void* Pointer, uint64 Size, Allocator* Allocator, uint64 Alignement)
	{
		if (Allocator != nullptr)
		{
			return Allocator->Reallocate(Pointer, Size, Alignement);
		}
		else
		{
			return Realloc(Pointer, Size);
		}
	}

	void Memory::Free(void* Pointer, Allocator* Allocator)
	{
		if (Allocator != nullptr)
		{
			Allocator->Free(Pointer);
		}
		else
		{
			FreeMemory(Pointer);
		}
	}

	void* Memory::Malloc(uint64 Size)
	{
		NEXUS_ASSERT(Size > 0, "Allocation Size is 0")
		void* Pointer = malloc(Size);
		NEXUS_ASSERT(Pointer != nullptr, "Pointer is null")
		return Pointer;
	}

	void* Memory::Realloc(void* Memory, uint64 Size)
	{
		NEXUS_ASSERT(Size > 0, "Allocation Size is 0")
		void* Pointer = realloc(Memory, Size);
		NEXUS_ASSERT(Pointer != nullptr, "Pointer is null")
		return Pointer;
	}

	void Memory::FreeMemory(void* Memory)
	{
		if (Memory == nullptr)
		{
			return;
		}

		free(Memory);
	}

	uint64 Memory::AlignAddress(uint64 Address, uint64 Alignement)
	{
		uint64 Mask = Alignement - 1;
		NEXUS_ASSERT((Alignement & Mask) == 0, "Alignement should be power of 2")
		return (Address + Mask) & ~Mask;
	}

	void* Memory::AlignPointer(void* Pointer, uint64 Alignement)
	{
		uint64 RawAddress = reinterpret_cast<uint64>(Pointer);

		uint64 AlignedAddress = AlignAddress(RawAddress, Alignement);
		if (RawAddress == AlignedAddress)
		{
			AlignedAddress += Alignement;
		}

		uint64 Shift = AlignedAddress - RawAddress;
		NEXUS_ASSERT(Shift > 0 && Shift <= 256, "Shift is too large")

		uint8* MemoryBlock = reinterpret_cast<uint8*>(AlignedAddress);
		MemoryBlock[-1] = static_cast<uint8>(Shift & 0xFF);

		return reinterpret_cast<void*>(AlignedAddress);
	}

	void* Memory::UnalignPointer(void* Pointer)
	{
		if (Pointer == nullptr)
		{
			return nullptr;
		}

		uint8* AlignedPointer = reinterpret_cast<uint8*>(Pointer);
		
		uint8 Shift = AlignedPointer[-1];
		NEXUS_ASSERT(Shift > 0 && Shift <= 256, "Shift is too large")
	
		uint64 AlignAddress = reinterpret_cast<uint64>(Pointer);
		uint64 RawAddress = AlignAddress - Shift;

		return reinterpret_cast<void*>(RawAddress);
	}
	
	void* Memory::OffsetPointer(void* Pointer, uint64 Offset)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);
		Address += Offset;
		return reinterpret_cast<void*>(Address);
	}

	bool Memory::IsPointerInRange(void* Pointer, void* Position, uint64 Offset)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64 Start = reinterpret_cast<uint64>(Position);
		uint64 End = Start + Offset;
		return Address >= Start && Address < End;
	}

	void Memory::MemSet(void* Memory, uint8 Value, uint64 Size)
	{
		NEXUS_ASSERT(Memory != nullptr, "Trying to set value (%d) to null address", Value)
		NEXUS_ASSERT(Size > 0, "Invalid size (%d)", Size)
		
		memset(Memory, Value, Size);
	}

	void Memory::MemCopy(const void* Source, void* Destination, uint64 Size)
	{
		NEXUS_ASSERT(Source != nullptr, "Trying to copy memory from null address")
		NEXUS_ASSERT(Destination != nullptr, "Trying to copy memory to null address")
		NEXUS_ASSERT(Size > 0, "Invalid size (%d)", Size)

		memcpy(Destination, Source, Size);
	}

	void Memory::MemMove(const void* Source, void* Destination, uint64 Size)
	{
		NEXUS_ASSERT(Source != nullptr, "Trying to move memory from null address")
		NEXUS_ASSERT(Destination != nullptr, "Trying to move memory to null address")
		NEXUS_ASSERT(Size > 0, "Invalid size (%d)", Size)

		memmove(Destination, Source, Size);
	}

	void Memory::PushActiveAllocator(Allocator* Alloc)
	{
		Allocators->Append(Alloc);
	}

	void Memory::PopActiveAllocator()
	{
		Allocators->Remove();
	}

	Allocator* Memory::GetActiveAllocator()
	{
		if (!Allocators)
		{
			return nullptr;
		}

		if (Allocators->GetCount() == 0)
		{
			Allocators->Append(NEXUS_ALLOCATOR_DEFAULT);
		}

		return Allocators->Get();
	}
}
