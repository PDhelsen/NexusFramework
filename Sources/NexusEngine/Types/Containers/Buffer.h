#pragma once

#include "Types/Numbers/Integer.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/AllocatorContext.h"

namespace NxEn
{
	template<typename T = Byte>
	class Buffer
	{
	public:
		Buffer(uint64 Size, Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Count(0), Data(nullptr)
		{
			NEXUS_ASSERT(Size > 0, Default, "Buffer has to have size greater than 0");

			Allocate(Size);
		}

		Buffer(const Buffer<T>& Other)
			: Alloc(Other.Alloc), Count(Other.Count), Data(nullptr)
		{
			Allocate(Count);

			Memory::MemCopy(Other.Data, Data, sizeof(T) * Count);
		}

		Buffer(Buffer<T>&& Other) noexcept
			: Alloc(Other.Alloc), Count(Other.Count), Data(Other.Data)
		{
			Other.Data = nullptr;
		}

		~Buffer()
		{
			Free();
		}

		Buffer<T>& operator=(const Buffer<T>& Other)
		{
			if (*this == Other)
			{
				return *this;
			}

			Free();

			Count = Other.Count;

			Allocate(Count);

			Memory::MemCopy(Other.Data, Data, sizeof(T) * Count);

			return *this;
		}

		Buffer<T>& operator=(Buffer<T>&& Other) noexcept
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

		bool operator==(const Buffer<T>& Other) const
		{
			return Count == Other.Count && Data == Other.Data;
		}

		bool operator!=(const Buffer<T>& Other) const
		{
			return !(*this == Other);
		}

		uint64 GetCount() const { return Count; }
		uint64 GetByteSize() const { return Count * sizeof(T); }
		uint64 GetStride() const { return sizeof(T); }
		T* GetPtr() const { return Data; }

	private:
		void Allocate(uint64 Size)
		{
			ValidateCapacity(Size);
			Data = (T*)Memory::Allocate(sizeof(T) * Count, Alloc);
		}

		void Free()
		{
			Memory::Free(Data, Alloc);
		}

		void ValidateCapacity(uint64 Size)
		{
			Count = Size > 1 ? Size : 1;
		}

		Allocator* Alloc;
		uint64 Count;
		T* Data;
	};

	template<typename T = Byte>
	struct BufferView
	{
	public:
		BufferView()
			: Count(0), Data(nullptr)
		{
		}

		BufferView(T* Ptr, uint64 Size)
			: Count(Size), Data(Ptr)
		{
		}

		BufferView(const Buffer<T>& Other)
			: Count(Other.GetCount()), Data(Other.GetPtr())
		{
		}

		uint64 GetCount() const { return Count; }
		uint64 GetByteSize() const { return Count * sizeof(T); }
		uint64 GetStride() const { return sizeof(T); }
		T* GetPtr() const { return Data; }

	private:
		uint64 Count;
		T* Data;
	};
}
