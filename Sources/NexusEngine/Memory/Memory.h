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
		template<typename T, typename... Args>
		static T* AllocateArray(uint32 Count, Args&&... args);
		template<typename T>
		static void Deallocate(T* Object);
		template<typename T>
		static void DeallocateArray(T* Object, uint32 Count);
		
		template<typename T, typename... Args>
		static T* Construct(void* Pointer, Args&&... args);
		template<typename T>
		static void Destruct(T* Object);

		NEXUS_ENGINE_API static void* Malloc(uint64 Size);
		NEXUS_ENGINE_API static void* Realloc(void* Memory, uint64 Size);
		NEXUS_ENGINE_API static void Free(void* Memory);

		NEXUS_ENGINE_API static void MemSet(void* Memory, int32 Value, uint64 Size);
		NEXUS_ENGINE_API static void MemCopy(void* Source, void* Destination, uint64 Size);
	};

	template<typename T, typename... Args>
	T* Memory::Allocate(Args&&... args)
	{
		T* Pointer = (T*)Malloc(sizeof(T));
		return Construct<T>(Pointer, args...);
	}

	template<typename T, typename... Args>
	T* Memory::AllocateArray(uint32 Count, Args&&... args)
	{
		T* Pointer = (T*)Malloc(sizeof(T) * Count);

		T* Offset = Pointer;
		for (uint32 Index = 0; Index < Count; Index++)
		{
			Construct<T>(Offset++, args...);
		}
		
		return (T*)Pointer;
	}

	template<typename T>
	void Memory::Deallocate(T* Object)
	{
		Destruct<T>(Object);
		Free(Object);
	}

	template<typename T>
	void Memory::DeallocateArray(T* Object, uint32 Count)
	{
		for (uint32 Index = 0; Index < Count; Index++)
		{
			Destruct<T>(&Object[Index]);
		}
		Free(Object);
	}

	template<typename T, typename... Args>
	T* Memory::Construct(void* Pointer, Args&&... args)
	{
		NEXUS_ASSERT(Pointer != nullptr, "Pointer is null")
		T* Object = new (Pointer) T(args...);
		NEXUS_ASSERT(Object != nullptr, "Object is null");
		return Object;
	}

	template<typename T>
	void Memory::Destruct(T* Object)
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