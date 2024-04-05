#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Debug/Assert.h"

#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/StackAllocator.h"
#include "Memory/Allocator/HeapAllocator.h"
#include "Memory/Allocator/PoolAllocator.h"

namespace NxEn
{
	// Memory
	// Static class that provide global function to handle memory (malloc / free, operator, placement new)

#define NEXUS_MEMORY_ALIGN 16

#define NEXUS_ALLOCATOR_DEFAULT NxEn::Memory::GetHeap()
#define NEXUS_STACK_SIZE 1024
#define NEXUS_HEAP_SIZE 1024 * 1024

#define NEXUS_HANDLE_COUNT 1024

	class Memory
	{
	public:
		template<typename T, typename... Args>
		static T* Construct(void* Pointer, Args&&... args);
		template<typename T>
		static void Destruct(T* Object);

		NEXUS_ENGINE_API static void* Allocate(uint64 Size, uint64 Alignement, Allocator* Allocator);
		NEXUS_ENGINE_API static void* Realloc(void* Pointer, uint64 Size, uint64 Alignement, Allocator* Allocator);
		NEXUS_ENGINE_API static void Free(void* Pointer, Allocator* Allocator);

		NEXUS_ENGINE_API static void* Malloc(uint64 Size);
		NEXUS_ENGINE_API static void* Realloc(void* Memory, uint64 Size);
		NEXUS_ENGINE_API static void Free(void* Memory);

		NEXUS_ENGINE_API static uint64 AlignAddress(uint64 Address, uint64 Alignement);
		NEXUS_ENGINE_API static void* AlignPointer(void* Pointer, uint64 Alignement);
		NEXUS_ENGINE_API static void* UnalignPointer(void* Pointer);
		NEXUS_ENGINE_API static void* OffsetPointer(void* Pointer, uint64 Offset);

		NEXUS_ENGINE_API static void MemSet(void* Memory, uint8 Value, uint64 Size);
		NEXUS_ENGINE_API static void MemCopy(void* Source, void* Destination, uint64 Size);

		NEXUS_ENGINE_API static void SetActiveAllocator(Allocator* Allocator) { Active = Allocator; }
		NEXUS_ENGINE_API static Allocator* GetActiveAllocator() { return Active; }

		NEXUS_ENGINE_API static StackAllocator* GetStack() { return Stack; }
		NEXUS_ENGINE_API static HeapAllocator* GetHeap() { return Heap; }

	private:
		static StackAllocator* Stack;
		static HeapAllocator* Heap;

		static Allocator* Active;
	};

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