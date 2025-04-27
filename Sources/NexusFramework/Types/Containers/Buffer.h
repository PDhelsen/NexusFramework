#pragma once

#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Memory/Allocator/AllocatorContext.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	class Buffer
	{
	public:
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

		Buffer& operator=(const Buffer& Other)
		{
			if (*this == Other)
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
			if (*this == Other)
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

		void Set(void* Source, uint64 Size = 0, uint64 OffsetBuffer = 0, uint64 OffsetSource = 0)
		{
			Size = Size != 0 ? Size : Count;
			Copy(Source, Size, OffsetBuffer, OffsetSource);
		}

		void Resize(uint64 Size)
		{
			Reallocate(Size);
		}

		template<typename T>
		T* GetPtr(uint64 Offset = 0) const { return static_cast<T*>(GetPtr(Offset)); }
		void* GetPtr(uint64 Offset = 0) const { return (Byte*)Data + Offset; }
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

		void Copy(void* Source, uint64 Size, uint64 OffsetBuffer, uint64 OffsetSource)
		{
			Memory::MemCopy((Byte*)Source + OffsetSource, (Byte*)Data + OffsetBuffer, Size);
		}

		void ValidateCount(uint64 Size)
		{
			Count = Size > 1 ? Size : 1;
		}

		Allocator* Alloc;
		uint64 Count;
		void* Data;
	};

	struct BufferView
	{
	public:
		BufferView()
			: Data(nullptr), Count(0)
		{
		}

		BufferView(void* Ptr, uint64 Size)
			: Data(Ptr), Count(Size)
		{
		}

		BufferView(const Buffer& Other)
			: Data(Other.GetPtr()), Count(Other.GetCount())
		{
		}

		BufferView(const BufferView& Other)
			: Data(Other.GetPtr()), Count(Other.GetCount())
		{
		}

		template<typename T>
		T* GetPtr(uint64 Offset = 0) const { return static_cast<T*>(GetPtr(Offset)); }
		void* GetPtr(uint64 Offset = 0) const { return (Byte*)Data + Offset; }
		uint64 GetCount() const { return Count; }

	private:
		void* Data;
		uint64 Count;
	};
}
