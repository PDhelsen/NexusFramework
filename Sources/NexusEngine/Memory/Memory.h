#pragma once

#include "Core/NexusEngine.h"
#include "Debug/Assert.h"

namespace NxEn
{
	// Memory
	// Static class that provide global function to handle memory (malloc / free, operator, placement new)

	class Memory
	{
	public:
		template<typename T, typename... Args>
		static T* Allocate(Args&&... args);
		template<typename T>
		static void Deallocate(T* Object);

		NEXUS_ENGINE_API static void* Malloc(uint64 Size);
		NEXUS_ENGINE_API static void* Realloc(void* Memory, uint64 Size);
		NEXUS_ENGINE_API static void Free(void* Memory);

		NEXUS_ENGINE_API static void MemSet(void* Memory, int32 Value, uint64 Size);
		NEXUS_ENGINE_API static void MemCopy(void* Source, void* Destination, uint64 Size);
	};

	template<typename T, typename... Args>
	inline T* Memory::Allocate(Args&&... args)
	{
		void* RawPtr = Malloc(sizeof(T));
		T* TypedPtr = new (RawPtr) T(args...);
		NEXUS_ASSERT(TypedPtr != nullptr, "Pointer is null");
		return TypedPtr;
	}

	template<typename T>
	inline void Memory::Deallocate(T* Object)
	{
		Object->~T();
	}
}

#pragma warning(push)
#pragma warning(disable: 6387)
#pragma warning(disable: 28196)
#pragma warning(disable: 28251)

void* operator new (size_t Size);
void* operator new[](size_t Size);
void operator delete(void* Memory);
void operator delete[](void* Memory);

#pragma warning(pop)