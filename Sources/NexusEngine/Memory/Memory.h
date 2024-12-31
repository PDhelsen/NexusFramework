#pragma once

#include "Core/NexusEngineCore.h"
#include "External/StandardLibrary.h"
#include "Types/Numbers/Integer.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/AllocatorContext.h"

namespace NxEn
{
	namespace Memory
	{
		constexpr double ByteToKilo = 1024.0;
		constexpr double ByteToMega = 1024.0 * 1024.0;
		constexpr double ByteToGiga = 1024.0 * 1024.0 * 1024.0;
		
		constexpr double KiloToByte = 1.0 / (1024.0);
		constexpr double MegaToByte = 1.0 / (1024.0 * 1024.0);
		constexpr double GigaToByte = 1.0 / (1024.0 * 1024.0 * 1024.0);

		constexpr uint64 DefaultAlignement = 16;

		NEXUS_ENGINE_API void MemSet(void* Memory, uint8 Value, uint64 Size);
		NEXUS_ENGINE_API void MemCopy(const void* Source, void* Destination, uint64 Size);
		NEXUS_ENGINE_API void MemMove(const void* Source, void* Destination, uint64 Size);
		NEXUS_ENGINE_API bool MemCompare(const void* Source, const void* Destination, uint64 SizeSource, uint64 SizeDestination);
		NEXUS_ENGINE_API bool MemCompare(const void* Source, const void* Destination, uint64 Size);

		NEXUS_ENGINE_API uint64 AlignAddress(uint64 Address, uint64 Alignement);
		NEXUS_ENGINE_API void* AlignPointer(void* Pointer, uint64 Alignement);
		NEXUS_ENGINE_API void* UnalignPointer(void* Pointer);
		NEXUS_ENGINE_API void* OffsetPointer(void* Pointer, uint64 Offset);
		NEXUS_ENGINE_API bool IsPointerInRange(void* Pointer, void* Position, uint64 Offset);

		NEXUS_ENGINE_API void* Allocate(uint64 Size, Allocator* Allocator = AllocatorContext::Get(), uint64 Alignement = DefaultAlignement);
		NEXUS_ENGINE_API void* Reallocate(void* Pointer, uint64 Size, Allocator* Allocator = AllocatorContext::Get(), uint64 Alignement = DefaultAlignement);
		NEXUS_ENGINE_API void Free(void* Pointer, Allocator* Allocator = AllocatorContext::Get());

		template<typename T, typename... Args>
		T* Construct(void* Pointer, Args&&... args)
		{
			return new (Pointer) T(args...);
		}

		template<typename T>
		void Destruct(T* Object)
		{
			Object->~T();
		}

		template<typename T, typename ...Args>
		T* Create(uint64 Size, Allocator* Allocator = AllocatorContext::Get(), Args&& ...args)
		{
			void* Ptr = Allocate(Size, Allocator, alignof(T));
			return Construct<T>(Ptr, args...);
		}

		template<typename T>
		void Destroy(T* Pointer, Allocator* Allocator = AllocatorContext::Get())
		{
			Destruct(Pointer);
			Free(Pointer, Allocator);
		}
	};
}
