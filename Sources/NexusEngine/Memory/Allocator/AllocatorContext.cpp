#include "Core/NexusEnginePch.h"
#include "AllocatorContext.h"

namespace NxEn
{
	static Stack<Allocator*, 10>& GetAllocators() { static Stack<Allocator*, 10> Allocators(nullptr); return Allocators; }

	AllocatorContext::AllocatorContext(Allocator* Allocator)
	{
		AllocatorContext::Push(Allocator);
	}

	AllocatorContext::~AllocatorContext()
	{
		AllocatorContext::Pop();
	}

	void AllocatorContext::Push(Allocator* Alloc)
	{
		Stack<Allocator*, 10>& Allocators = GetAllocators();
		Allocators.Append(Alloc);
	}

	void AllocatorContext::Pop()
	{
		Stack<Allocator*, 10>& Allocators = GetAllocators();
		Allocators.Remove();
	}

	Allocator* AllocatorContext::Get()
	{
		Stack<Allocator*, 10>& Allocators = GetAllocators();

		if (Allocators.GetCount() == 0)
		{
			return nullptr;
		}

		return Allocators.Get();
	}
}
