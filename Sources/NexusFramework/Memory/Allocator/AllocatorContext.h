#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"

namespace NxFr
{
	struct NEXUS_FRAMEWORK_API AllocatorContext
	{
	public:
		static Allocator* Get();
		static void Push(Allocator* Alloc);
		static void Pop();
		static void Reset();

		AllocatorContext(Allocator* Allocator);
		~AllocatorContext();
	};
}

