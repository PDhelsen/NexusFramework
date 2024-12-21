#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"

namespace NxEn
{
#define NEXUS_MEMORY_ALIGN 16

#define NEXUS_ALLOCATOR_DEFAULT NxEn::Memory::GetHeap()
#define NEXUS_STACK_SIZE (uint64)(1 * Memory::ByteToKilo)
#define NEXUS_HEAP_SIZE (uint64)(1 * Memory::ByteToMega)

	class Allocator;
	class GlobalAllocator;
	class NexusAllocator;
	class StackAllocator;
	class HeapAllocator;
	class PoolAllocator;
	template<typename T, uint64> class Stack;

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
		NEXUS_ENGINE_API static bool IsPointerInRange(void* Pointer, void* Position, uint64 Offset);

		NEXUS_ENGINE_API static void MemSet(void* Memory, uint8 Value, uint64 Size);
		NEXUS_ENGINE_API static void MemCopy(const void* Source, void* Destination, uint64 Size);
		NEXUS_ENGINE_API static void MemMove(const void* Source, void* Destination, uint64 Size);
		NEXUS_ENGINE_API static bool MemCompare(const void* Source, const void* Destination, uint64 SizeSource, uint64 SizeDestination);
		NEXUS_ENGINE_API static bool MemCompare(const void* Source, const void* Destination, uint64 Size);

		NEXUS_ENGINE_API static void PushActiveAllocator(Allocator* Alloc);
		NEXUS_ENGINE_API static void PopActiveAllocator();
		NEXUS_ENGINE_API static Allocator* GetActiveAllocator();

		NEXUS_ENGINE_API static GlobalAllocator* GetGlobal() { return Global; }
		NEXUS_ENGINE_API static StackAllocator* GetStack() { return DefaultStack; }
		NEXUS_ENGINE_API static HeapAllocator* GetHeap() { return DefaultHeap; }

		inline static const double ByteToKilo = 1024.0;
		inline static const double ByteToMega = 1024.0 * 1024.0;
		inline static const double ByteToGiga = 1024.0 * 1024.0 * 1024.0;

		inline static const double KiloToByte = 1.0 / (1024.0);
		inline static const double MegaToByte = 1.0 / (1024.0 * 1024.0);
		inline static const double GigaToByte = 1.0 / (1024.0 * 1024.0 * 1024.0);

	private:
		static void* Malloc(uint64 Size);
		static void* Realloc(void* Memory, uint64 Size);
		static void FreeMemory(void* Memory);

		static GlobalAllocator* Global;
		static StackAllocator* DefaultStack;
		static HeapAllocator* DefaultHeap;
		static Stack<Allocator*, 10>* Allocators;
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
		return new (Pointer) T(args...);
	}

	template<typename T>
	void Memory::Destruct(T* Object)
	{
		Object->~T();
	}
}
