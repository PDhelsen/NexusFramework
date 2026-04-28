#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"

namespace NxFr
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

