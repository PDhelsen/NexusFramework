#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"

#define NEXUS_DELETE(Ptr) \
delete Ptr;\
Ptr = nullptr;

#define NEXUS_DELETE_ARRAY(Ptr) \
delete[] Ptr;\
Ptr = nullptr;

namespace NxFr
{
	namespace Memory
	{
		inline constexpr double ByteToKilo = 1024.0;
		inline constexpr double ByteToMega = 1024.0 * 1024.0;
		inline constexpr double ByteToGiga = 1024.0 * 1024.0 * 1024.0;

		inline constexpr double KiloToByte = 1.0 / (1024.0);
		inline constexpr double MegaToByte = 1.0 / (1024.0 * 1024.0);
		inline constexpr double GigaToByte = 1.0 / (1024.0 * 1024.0 * 1024.0);

		inline constexpr uint64 DefaultAlignement = 16;

		NEXUS_FRAMEWORK_API void MemSet(void* Memory, uint8 Value, uint64 Size);
		NEXUS_FRAMEWORK_API void MemCopy(const void* Source, void* Destination, uint64 Size);
		NEXUS_FRAMEWORK_API void MemMove(const void* Source, void* Destination, uint64 Size);
		NEXUS_FRAMEWORK_API bool MemCompare(const void* Source, const void* Destination, uint64 SizeSource, uint64 SizeDestination);
		NEXUS_FRAMEWORK_API bool MemCompare(const void* Source, const void* Destination, uint64 Size);

		NEXUS_FRAMEWORK_API uint64 AlignAddress(uint64 Address, uint64 Alignement);
		NEXUS_FRAMEWORK_API void* AlignPointer(void* Pointer, uint64 Alignement);
		NEXUS_FRAMEWORK_API void* UnalignPointer(void* Pointer);
		NEXUS_FRAMEWORK_API void* OffsetPointer(void* Pointer, uint64 Offset);
		NEXUS_FRAMEWORK_API bool IsPointerInRange(void* Pointer, void* Position, uint64 Offset);

		NEXUS_FRAMEWORK_API void* Allocate(uint64 Size, Allocator* Allocator = Allocator::TryGet(), uint64 Alignement = DefaultAlignement);
		NEXUS_FRAMEWORK_API void* Reallocate(void* Pointer, uint64 Size, Allocator* Allocator = Allocator::TryGet(), uint64 Alignement = DefaultAlignement);
		NEXUS_FRAMEWORK_API void Free(void* Pointer, Allocator* Allocator = Allocator::TryGet());

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
		T* Create(Allocator* Allocator = Allocator::TryGet(), Args&& ...args)
		{
			void* Ptr = Allocate(sizeof(T), Allocator, alignof(T));
			return Construct<T>(Ptr, args...);
		}

		template<typename T>
		void Destroy(T* Pointer, Allocator* Allocator = Allocator::TryGet())
		{
			Destruct(Pointer);
			Free(Pointer, Allocator);
		}
	};
}
