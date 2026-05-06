#pragma once

#include "NexusFramework/Memory/Allocator/Allocator.h"

namespace NxFr
{
	class MemoryAllocator : public Allocator
	{
	public:
		NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, MemoryAllocator)
		NEXUS_FRAMEWORK_API MemoryAllocator(uint64 Size);
		NEXUS_FRAMEWORK_API virtual ~MemoryAllocator();

	protected:
		NEXUS_FRAMEWORK_API void WipeoutMemory();
		NEXUS_FRAMEWORK_API void EraseMemory(void* Pointer, uint64 Size);

		NEXUS_FRAMEWORK_API bool IsPointerInMemoryBlock(void* Pointer) const;
		NEXUS_FRAMEWORK_API void* GetMemoryBlock() const;

	private:
		void* Data;
	};
}
