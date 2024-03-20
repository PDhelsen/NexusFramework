#include "Core/NexusEnginePch.h"
#include "Memory.h"

namespace NxEn
{
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

	void Memory::Free(void* Memory)
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

	void Memory::MemSet(void* Memory, uint8 Value, uint64 Size)
	{
		NEXUS_ASSERT(Memory != nullptr, "Trying to set value (%d) to null address", Value)
		NEXUS_ASSERT(Size > 0, "Invalid size (%d)", Size)
		
		memset(Memory, Value, Size);
	}

	void Memory::MemCopy(void* Source, void* Destination, uint64 Size)
	{
		NEXUS_ASSERT(Source != nullptr, "Trying to copy memory from null address")
		NEXUS_ASSERT(Destination != nullptr, "Trying to copy memory to null address")
		NEXUS_ASSERT(Size > 0, "Invalid size (%d)", Size)

		memcpy(Destination, Source, Size);
	}
}