#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/AllocatorContext.h"

namespace NxFr
{
	static Stack<Allocator*>& GetAllocators() { static thread_local Stack<Allocator*> Allocators(nullptr); return Allocators; }

	Allocator* AllocatorContext::Get()
	{
		Stack<Allocator*>& Allocators = GetAllocators();

		if (Allocators.GetCount() == 0)
		{
			return nullptr;
		}

		return Allocators.Get();
	}

	void AllocatorContext::Push(Allocator* Alloc)
	{
		Stack<Allocator*>& Allocators = GetAllocators();
		Allocators.Append(Alloc);
	}

	void AllocatorContext::Pop()
	{
		Stack<Allocator*>& Allocators = GetAllocators();
		Allocators.Remove();
	}

	AllocatorContext::AllocatorContext(Allocator* Allocator)
	{
		AllocatorContext::Push(Allocator);
	}

	AllocatorContext::~AllocatorContext()
	{
		AllocatorContext::Pop();
	}
}
