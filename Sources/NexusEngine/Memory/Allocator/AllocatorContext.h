#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Memory/Allocator/Allocator.h"

namespace NxEn
{
	template<typename T, uint64> class Stack;

	struct AllocatorContext
	{
	public:
		NEXUS_ENGINE_API AllocatorContext(Allocator* Allocator);
		NEXUS_ENGINE_API ~AllocatorContext();

		NEXUS_ENGINE_API static void Push(Allocator* Alloc);
		NEXUS_ENGINE_API static void Pop();
		NEXUS_ENGINE_API static Allocator* Get();

	private:
		static Stack<Allocator*, 10>* Allocators;
	};
}

