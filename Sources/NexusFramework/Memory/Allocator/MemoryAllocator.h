#pragma once

#include "NexusFramework/Memory/Allocator/Allocator.h"

namespace NxFr
{
	class NEXUS_FRAMEWORK_API MemoryAllocator : public Allocator
	{
	public:
		NEXUS_NOCOPY_NOMOVE(MemoryAllocator)
		MemoryAllocator(uint64 Size);
		virtual ~MemoryAllocator();

	protected:
		void WipeoutMemory();
		void EraseMemory(void* Pointer, uint64 Size);

		bool IsPointerInMemoryBlock(void* Pointer) const;
		void* GetMemoryBlock() const;

	private:
		void* Data;
	};
}
