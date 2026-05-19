#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Memory.h"

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Debug/Profiler/Memory/MemoryTracker.h"

namespace NxFr
{
	void Memory::MemSet(void* Memory, uint8 Value, uint64 Size)
	{
		NX_ASSERT(Memory != nullptr, Default, "Trying to set value (%hhu) to null address", Value);
		NX_ASSERT(Size > 0, Default, "Invalid size (%llu)", Size);

		memset(Memory, Value, Size);
	}

	void Memory::MemCopy(const void* Source, void* Destination, uint64 Size)
	{
		NX_ASSERT(Source != nullptr, Default, "Trying to copy memory from null address");
		NX_ASSERT(Destination != nullptr, Default, "Trying to copy memory to null address");
		NX_ASSERT(Size > 0, Default, "Invalid size (%llu)", Size);

		memcpy(Destination, Source, Size);
	}

	void Memory::MemMove(const void* Source, void* Destination, uint64 Size)
	{
		NX_ASSERT(Source != nullptr, Default, "Trying to move memory from null address");
		NX_ASSERT(Destination != nullptr, Default, "Trying to move memory to null address");
		NX_ASSERT(Size > 0, Default, "Invalid size (%llu)", Size);

		memmove(Destination, Source, Size);
	}

	bool Memory::MemCompare(const void* Source, const void* Destination, uint64 SizeSource, uint64 SizeDestination)
	{
		NX_ASSERT(Source != nullptr, Default, "Trying to compare memory from null address");
		NX_ASSERT(Destination != nullptr, Default, "Trying to compare memory to null address");

		return SizeSource == SizeDestination && MemCompare(Source, Destination, SizeSource);
	}

	bool Memory::MemCompare(const void* Source, const void* Destination, uint64 Size)
	{
		NX_ASSERT(Source != nullptr, Default, "Trying to compare memory from null address");
		NX_ASSERT(Destination != nullptr, Default, "Trying to compare memory to null address");

		return memcmp(Source, Destination, Size) == 0;
	}

	uint64 Memory::AlignAddress(uint64 Address, uint64 Alignement)
	{
		uint64 Mask = Alignement - 1;
		NX_ASSERT((Alignement & Mask) == 0, Default, "Alignement should be power of 2");
		return (Address + Mask) & ~Mask;
	}

	void* Memory::AlignPointer(void* Pointer, uint64 Alignement)
	{
		NX_ASSERT(Pointer, Default, "Invalid Pointer");
		NX_ASSERT(Math::IsPowerOfTwo(Alignement), Default, "Invalid Alignement");

		uint64 RawAddress = reinterpret_cast<uint64>(Pointer);

		uint64 AlignedAddress = AlignAddress(RawAddress, Alignement);
		if (RawAddress == AlignedAddress)
		{
			AlignedAddress += Alignement;
		}

		uint64 Shift = AlignedAddress - RawAddress;
		NX_ASSERT(Shift > 0 && Shift <= 256, Default, "Shift is too large");

		uint8* MemoryBlock = reinterpret_cast<uint8*>(AlignedAddress);
		MemoryBlock[-1] = static_cast<uint8>(Shift & 0xFF);

		return reinterpret_cast<void*>(AlignedAddress);
	}

	void* Memory::UnalignPointer(void* Pointer)
	{
		NX_ASSERT(Pointer, Default, "Invalid Pointer");

		uint8* AlignedPointer = reinterpret_cast<uint8*>(Pointer);

		uint8 Shift = AlignedPointer[-1];
		NX_ASSERT(Shift > 0 && Shift <= 256, Default, "Shift is too large");

		uint64 AlignAddress = reinterpret_cast<uint64>(Pointer);
		uint64 RawAddress = AlignAddress - Shift;

		return reinterpret_cast<void*>(RawAddress);
	}

	void* Memory::OffsetPointer(void* Pointer, uint64 Offset)
	{
		NX_ASSERT(Pointer, Default, "Invalid Pointer");

		uint64 Address = reinterpret_cast<uint64>(Pointer);
		Address += Offset;
		return reinterpret_cast<void*>(Address);
	}

	bool Memory::IsPointerInRange(void* Pointer, void* Position, uint64 Offset)
	{
		NX_ASSERT(Pointer, Default, "Invalid Pointer");

		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64 Start = reinterpret_cast<uint64>(Position);
		uint64 End = Start + Offset;
		return Address >= Start && Address < End;
	}

	void* Memory::Allocate(uint64 Size, Allocator* Allocator, uint64 Alignement)
	{
		NX_ASSERT(Size > 0, Default, "Allocation Size is 0");
		NX_ASSERT(Math::IsPowerOfTwo(Alignement), Default, "Invalid Alignement");
		void* Return = nullptr;

		if (Allocator != nullptr)
		{
			Return = Allocator->Allocate(Size, Alignement);
		}
		else
		{
			Return = malloc(Size);
			NX_TRACKMEMORY_ALLOCATION(Return, Size);
		}

		NX_ASSERT(Return != nullptr, Default, "Pointer is null");
		return Return;
	}

	void* Memory::Reallocate(void* Pointer, uint64 Size, Allocator* Allocator, uint64 Alignement)
	{
		NX_ASSERT(Size > 0, Default, "Allocation Size is 0");
		NX_ASSERT(Math::IsPowerOfTwo(Alignement), Default, "Invalid Alignement");
		void* Return = nullptr;

		if (Allocator != nullptr)
		{
			Return = Allocator->Reallocate(Pointer, Size, Alignement);
		}
		else
		{
			Return = realloc(Pointer, Size);
			NX_TRACKMEMORY_REALLOCATION(Pointer, Return, Size);
		}

		NX_ASSERT(Return != nullptr, Default, "Pointer is null");
		return Return;
	}

	void Memory::Free(void* Pointer, Allocator* Allocator)
	{
		if (Allocator != nullptr)
		{
			Allocator->Free(Pointer);
		}
		else
		{
			NX_TRACKMEMORY_DEALLOCATION(Pointer);
			free(Pointer);
		}
	}
}
