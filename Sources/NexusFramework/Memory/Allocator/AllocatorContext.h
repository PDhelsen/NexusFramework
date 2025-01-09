#pragma once

#include "Core/NexusFrameworkCore.h"
#include "Types/Numbers/Integer.h"
#include "Memory/Allocator/Allocator.h"

namespace NxEn
{
	struct AllocatorContext
	{
	public:
		NEXUS_FRAMEWORK_API static Allocator* Get();
		NEXUS_FRAMEWORK_API static void Push(Allocator* Alloc);
		NEXUS_FRAMEWORK_API static void Pop();

		NEXUS_FRAMEWORK_API AllocatorContext(Allocator* Allocator);
		NEXUS_FRAMEWORK_API ~AllocatorContext();
	};
}

