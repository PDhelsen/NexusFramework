#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Debug/Assert.h"

namespace NxEn
{
#define NEXUS_MEMORY_ALIGN 16

#define NEXUS_ALLOCATOR_DEFAULT NxEn::Memory::GetHeap()
#define NEXUS_STACK_SIZE 1024
#define NEXUS_HEAP_SIZE 1024 * 1024

#define NEXUS_HANDLE_COUNT 1024

	class Allocator;
	class StackAllocator;
	class HeapAllocator;
	class PoolAllocator;

	class Memory
	{
	public:
		template<typename T, typename... Args>
		static T* Create(uint64 Size, Allocator* Allocator = nullptr, uint64 Alignement = NEXUS_MEMORY_ALIGN, Args&&... args);
		template<typename T>
		static T* Move(T* Pointer, uint64 Size, Allocator* Allocator = nullptr, uint64 Alignement = NEXUS_MEMORY_ALIGN);
		template<typename T>
		static void Destroy(T* Pointer, Allocator* Allocator = nullptr);

		template<typename T, typename... Args>
		static T* Construct(void* Pointer, Args&&... args);
		template<typename T>
		static void Destruct(T* Object);

		NEXUS_ENGINE_API static void* Allocate(uint64 Size, Allocator* Allocator = nullptr, uint64 Alignement = NEXUS_MEMORY_ALIGN);
		NEXUS_ENGINE_API static void* Reallocate(void* Pointer, uint64 Size, Allocator* Allocator = nullptr, uint64 Alignement = NEXUS_MEMORY_ALIGN);
		NEXUS_ENGINE_API static void Free(void* Pointer, Allocator* Allocator = nullptr);

		NEXUS_ENGINE_API static uint64 AlignAddress(uint64 Address, uint64 Alignement);
		NEXUS_ENGINE_API static void* AlignPointer(void* Pointer, uint64 Alignement);
		NEXUS_ENGINE_API static void* UnalignPointer(void* Pointer);
		NEXUS_ENGINE_API static void* OffsetPointer(void* Pointer, uint64 Offset);

		NEXUS_ENGINE_API static void MemSet(void* Memory, uint8 Value, uint64 Size);
		NEXUS_ENGINE_API static void MemCopy(const void* Source, void* Destination, uint64 Size);
		NEXUS_ENGINE_API static void MemMove(const void* Source, void* Destination, uint64 Size);

		NEXUS_ENGINE_API static void SetActiveAllocator(Allocator* Allocator) { Active = Allocator; }
		NEXUS_ENGINE_API static Allocator* GetActiveAllocator() { return Active; }

		NEXUS_ENGINE_API static StackAllocator* GetStack() { return Stack; }
		NEXUS_ENGINE_API static HeapAllocator* GetHeap() { return Heap; }

	private:
		static void* Malloc(uint64 Size);
		static void* Realloc(void* Memory, uint64 Size);
		static void FreeMemory(void* Memory);

		static StackAllocator* Stack;
		static HeapAllocator* Heap;

		// TODO: Implementation - Allocators - Store in stack
		static Allocator* Active;
	};

	template<typename T, typename ...Args>
	inline T* Memory::Create(uint64 Size, Allocator* Allocator, uint64 Alignement, Args&& ...args)
	{
		void* Ptr = Allocate(Size, Allocator, Alignement);
		return Construct<T>(Ptr, args...);
	}

	template<typename T>
	inline T* Memory::Move(T* Pointer, uint64 Size, Allocator* Allocator, uint64 Alignement)
	{
		return (T*)Reallocate(Pointer, Size, Allocator, Alignement);
	}

	template<typename T>
	inline void Memory::Destroy(T* Pointer, Allocator* Allocator)
	{
		Destruct(Pointer);
		Free(Pointer, Allocator);
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
