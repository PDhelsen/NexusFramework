#pragma once

#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Memory/Allocator/AllocatorContext.h"
#include "NexusFramework/Debug/Logger/Log.h"
#include "NexusFramework/Types/Containers/Iterator.h"

namespace NxFr
{
	struct BufferView
	{
		friend class ContainersUtils;

	public:
		using I = Iterator::IteratorPointer;

		BufferView()
			: Data(nullptr), Count(0)
		{
		}

		BufferView(const void* Ptr, uint64 Size)
			: Data(Ptr), Count(Size)
		{
		}

		BufferView(const BufferView& Other)
			: Data(Other.GetPtr()), Count(Other.GetCount())
		{
		}

		const I GetIterator(uint64 Offset) const
		{
			return GetIt(Offset);
		}

		const I begin() const { return Begin(); }
		const I Begin() const
		{
			return GetIt(0);
		}

		const I BeginReverse() const
		{
			I It = End();
			--It;
			return It;
		}

		const I end() const { return End(); }
		const I End() const
		{
			return GetIt(Count);
		}

		const I EndReverse() const
		{
			I It = Begin();
			--It;
			return It;
		}

		template<typename T>
		const T* GetPtr(uint64 Offset = 0) const
		{
			return static_cast<const T*>(GetPtr(Offset));
		}

		const void* GetPtr(uint64 Offset = 0) const
		{
			return (Byte*)Data + Offset;
		}

		uint64 GetCount() const { return Count; }

	private:
		const I GetIt(uint64 Offset) const
		{
			return I(const_cast<void*>(Data), Offset);
		}

		const void* Data;
		uint64 Count;
	};

	class Buffer
	{
		friend class ContainersUtils;

	public:
		using I = Iterator::IteratorPointer;

		Buffer(uint64 Size = 0, Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Count(0), Data(nullptr)
		{
			Allocate(Size);
		}

		Buffer(const Buffer& Other)
			: Alloc(Other.Alloc), Count(Other.Count), Data(nullptr)
		{
			Allocate(Count);
			Set(Other.Data);
		}

		Buffer(Buffer&& Other) noexcept
			: Alloc(Other.Alloc), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Buffer()
		{
			Free();
		}

		operator BufferView() const
		{
			return Get(Count);
		}

		Buffer& operator=(const Buffer& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			Free();

			Count = Other.Count;

			Allocate(Count);
			Set(Other.Data);

			return *this;
		}

		Buffer& operator=(Buffer&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

			Free();

			Alloc = Other.Alloc;
			Count = Other.Count;
			Data = Other.Data;

			Other.Count = 0;
			Other.Data = nullptr;

			return *this;
		}

		bool operator==(const Buffer& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Buffer& Other) const
		{
			return !(*this == Other);
		}

		bool operator>(const Buffer& Other) const
		{
			return Count > Other.Count;
		}

		bool operator>=(const Buffer& Other) const
		{
			return Count >= Other.Count;
		}

		bool operator<(const Buffer& Other) const
		{
			return Count < Other.Count;
		}

		bool operator<=(const Buffer& Other) const
		{
			return Count <= Other.Count;
		}

		void Clear()
		{
			Clear(Count, 0);
		}

		BufferView Get(uint64 Size, uint64 Offset = 0) const
		{
			NEXUS_ASSERT(Size > 0, Default, "Size is 0");
			NEXUS_ASSERT(Size <= Count, Default, "Size is greater than the size of the Buffer");
			NEXUS_ASSERT(Offset + Size <= Count, Default, "Get will overflow");

			return BufferView((Byte*)GetPtr() + Offset, Size);
		}

		void Set(const void* Source, uint64 Size = 0, uint64 OffsetBuffer = 0, uint64 OffsetSource = 0)
		{
			NEXUS_ASSERT(Source, Default, "Source is null");
			NEXUS_ASSERT(Size <= Count, Default, "Size is greater than the size of the Buffer");
			NEXUS_ASSERT(OffsetBuffer + Size <= Count, Default, "Set will overflow");

			Size = Size != 0 ? Size : Count;
			Copy(Source, Size, OffsetBuffer, OffsetSource);
		}

		void Grow(uint64 Size)
		{
			Reallocate(Count + Size);
		}

		void Resize(uint64 Size)
		{
			Reallocate(Size);
		}

		I GetIterator(uint64 Offset)
		{
			return GetIt(Offset);
		}

		const I GetIterator(uint64 Offset) const
		{
			return GetIt(Offset);
		}

		I begin()
		{
			return Begin();
		}

		I Begin()
		{
			return GetIt(0);
		}

		const I begin() const
		{
			return Begin();
		}

		const I Begin() const
		{
			return GetIt(0);
		}

		I BeginReverse()
		{
			I It = End();
			--It;
			return It;
		}

		const I BeginReverse() const
		{
			I It = End();
			--It;
			return It;
		}

		I end()
		{
			return End();
		}

		I End()
		{
			return GetIt(Count);
		}

		const I end() const
		{
			return End();
		}

		const I End() const
		{
			return GetIt(Count);
		}

		I EndReverse()
		{
			I It = Begin();
			--It;
			return It;
		}

		const I EndReverse() const
		{
			I It = Begin();
			--It;
			return It;
		}

		template<typename T>
		T* GetPtr(uint64 Offset = 0)
		{
			return static_cast<T*>(GetPtr(Offset));
		}

		template<typename T>
		const T* GetPtr(uint64 Offset = 0) const
		{
			return static_cast<T*>(GetPtr(Offset));
		}

		void* GetPtr(uint64 Offset = 0)
		{
			return (Byte*)Data + Offset;
		}

		const void* GetPtr(uint64 Offset = 0) const
		{
			return (Byte*)Data + Offset;
		}

		uint64 GetCount() const { return Count; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateCount(Size);
			Data = Memory::Allocate(Count, Alloc);
		}

		void Reallocate(uint64 Size)
		{
			ValidateCount(Size);
			Data = Memory::Reallocate(Data, Count, Alloc);
		}

		void Free()
		{
			Memory::Free(Data, Alloc);
		}

		void Clear(uint64 Size, uint64 Offset)
		{
			Memory::MemSet((Byte*)Data + Offset, 0, Size);
		}

		void Copy(const void* Source, uint64 Size, uint64 OffsetBuffer, uint64 OffsetSource)
		{
			Memory::MemCopy((Byte*)Source + OffsetSource, (Byte*)Data + OffsetBuffer, Size);
		}

		I GetIt(uint64 Offset)
		{
			return I(Data, Offset);
		}

		const I GetIt(uint64 Offset) const
		{
			return I(Data, Offset);
		}

		void ValidateCount(uint64 Size)
		{
			Count = Size > 1 ? Size : 1;
		}

		Allocator* Alloc;
		uint64 Count;
		void* Data;
	};	
}
