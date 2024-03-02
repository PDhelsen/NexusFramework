#include "Core/NexusEnginePch.h"
#include "Memory.h"


namespace NxEn
{
	void* Memory::Malloc(uint64 Size)
	{
		NEXUS_ASSERT(Size > 0, "Allocation Size is 0");
		void* Ptr = malloc(Size);
		NEXUS_ASSERT(Ptr != nullptr, "Pointer is null");
		return Ptr;
	}

	void* Memory::Realloc(void* Memory, uint64 Size)
	{
		NEXUS_ASSERT(Size > 0, "Allocation Size is 0");
		void* Ptr = realloc(Memory, Size);
		NEXUS_ASSERT(Ptr != nullptr, "Pointer is null");
		return Ptr;
	}

	void Memory::Free(void* Memory)
	{
		if (Memory == nullptr)
		{
			return;
		}

		free(Memory);
	}

	void Memory::MemSet(void* Memory, int32 Value, uint64 Size)
	{
		NEXUS_ASSERT(Memory != nullptr, "Trying to set value (%d) to null address", Value);
		NEXUS_ASSERT(Size <= 0, "Invalid size (%d)", Size);
		
		memset(Memory, Value, Size);
	}

	void Memory::MemCopy(void* Source, void* Destination, uint64 Size)
	{
		NEXUS_ASSERT(Source != nullptr, "Trying to copy memory from null address");
		NEXUS_ASSERT(Destination != nullptr, "Trying to copy memory to null address");
		NEXUS_ASSERT(Size <= 0, "Invalid size (%d)", Size);

		memcpy(Destination, Source, Size);
	}
}

#pragma warning(push)
#pragma warning(disable: 6387)
#pragma warning(disable: 28196)
#pragma warning(disable: 28251)

void* operator new(size_t Size)
{
	return NxEn::Memory::Malloc(Size);
}

void* operator new[](size_t Size)
{
	return NxEn::Memory::Malloc(Size);
}

void operator delete(void* Memory)
{
	NxEn::Memory::Free(Memory);
}

void operator delete[](void* Memory)
{
	NxEn::Memory::Free(Memory);
}

#pragma warning(pop)