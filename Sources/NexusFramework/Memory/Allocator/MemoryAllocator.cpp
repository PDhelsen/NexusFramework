#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/MemoryAllocator.h"

namespace NxFr
{
	MemoryAllocator::MemoryAllocator(uint64 Size)
		: Allocator(Size), Data(nullptr)
	{
		Data = Memory::Allocate(TotalAmount(), nullptr);
	}

	MemoryAllocator::~MemoryAllocator()
	{
		Memory::Free(Data, nullptr);
	}

	void MemoryAllocator::WipeoutMemory()
	{
		Memory::MemSet(Data, 0, TotalAmount());
	}

	void MemoryAllocator::EraseMemory(void* Pointer, uint64 Size)
	{
#if NEXUS_DEBUG
		Memory::MemSet(Pointer, 0, Size);
#endif
	}

	bool MemoryAllocator::IsPointerInMemoryBlock(void* Pointer) const
	{
		return Memory::IsPointerInRange(Pointer, Data, TotalAmount());
	}

	void* MemoryAllocator::GetMemoryBlock() const
	{
		return Data;
	}
}
