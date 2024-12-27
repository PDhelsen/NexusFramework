#include "Core/NexusEnginePch.h"
#include "AllocatorContext.h"

namespace NxEn
{
	Stack<Allocator*, 10>* AllocatorContext::Allocators = new Stack<Allocator*, 10>();

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
		Allocators->Append(Alloc);
	}

	void AllocatorContext::Pop()
	{
		Allocators->Remove();
	}

	Allocator* AllocatorContext::Get()
	{
		if (!Allocators || Allocators->GetCount() == 0)
		{
			return nullptr;
		}

		return Allocators->Get();
	}
}
