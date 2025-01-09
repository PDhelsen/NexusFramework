#include "Core/NexusFrameworkPch.h"
#include "Allocator.h"

namespace NxEn
{
	Allocator::Allocator(uint64 Size)
		: Capacity(Size), Amount(0), Data(nullptr)
	{
		Data = Memory::Allocate(Capacity, nullptr);
	}

	Allocator::~Allocator()
	{
		Memory::Free(Data, nullptr);
	}

	void Allocator::IncreaseAmount(uint64 Delta)
	{
		NEXUS_ASSERT(Amount + Delta <= Capacity, Default, "Invalid delta");

		Amount += Delta;
	}

	void Allocator::DecreaseAmount(uint64 Delta)
	{
		NEXUS_ASSERT(Amount > Delta, Default, "Invalid delta");

		Amount -= Delta;
	}

	void Allocator::ResetAmount()
	{
		Amount = 0;
	}

	void Allocator::WipeoutMemory()
	{
		Memory::MemSet(Data, 0, Capacity);
	}

	void Allocator::EraseMemory(void* Pointer, uint64 Size)
	{
#if NEXUS_DEBUG
		Memory::MemSet(Pointer, 0, Size);
#endif
	}

	bool Allocator::IsPointerInMemoryBlock(void* Pointer) const
	{
		return Memory::IsPointerInRange(Pointer, Data, Capacity);
	}

	void* Allocator::GetMemoryBlock() const
	{
		return Data;
	}
}
