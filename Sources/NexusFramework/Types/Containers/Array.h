#pragma once

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/Misc/Iterator.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	template<typename T, uint64 L = 0>
	class Array
	{
	public:
		using I = Iterator::IteratorBlock<T>;

		Array()
			: Alloc(nullptr), Count(0)
		{
			Allocate(L);
			ConstructRange(0, Count);
		}

		Array(uint64 Size, Allocator* Allctr = Allocator::TryGet())
			: Alloc(Allctr), Count(0)
		{
			NX_ASSERT((L == 0 && Size == 0) || (L == 0 && Size > 0), Default, "The provided size is invalid");

			Allocate(Size);
			ConstructRange(0, Count);
		}

		Array(InitializerList<T> Init, Allocator* Allctr = Allocator::TryGet())
			: Alloc(Allctr), Count(0)
		{
			NX_ASSERT((L == 0 && Init.size() == 0) || (L == 0 && Init.size() > 0), Default, "The provided size is invalid");

			Allocate(Init.size());
			ConstructRange(0, Count);

			uint64 Index = 0;
			for (auto& It : Init)
			{
				Assign(Index++, It);
			}
		}

		Array(const Array<T, L>& Other)
			: Alloc(Other.Alloc), Count(Other.Count)
		{
			Allocate(Count);
			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Construct(Index, Other[Index]);
			}
		}

		Array(Array<T, L>&& Other) noexcept
			: Alloc(Other.Alloc), Count(Other.Count)
		{
			if (!Other.IsStackArray())
			{
				Data.Heap = Other.Data.Heap;
				Other.Data.Heap = nullptr;
				Other.Count = 0;
			}
			else
			{
				for (uint64 Index = 0; Index < Count; ++Index)
				{
					Construct(Index, Other[Index]);
				}
			}
		}

		~Array()
		{
			DestructRange(0, Count);
			Free();
		}

		Array<T, L>& operator=(const Array<T, L>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			DestructRange(0, Count);
			Free();

			Count = Other.Count;

			Allocate(Count);
			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Construct(Index, Other[Index]);
			}

			return *this;
		}

		Array<T, L>& operator=(Array<T, L>&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

			DestructRange(0, Count);
			Free();

			Alloc = Other.Alloc;
			Count = Other.Count;

			if (!Other.IsStackArray())
			{
				Data.Heap = Other.Data.Heap;
				Other.Data.Heap = nullptr;
				Other.Count = 0;
			}
			else
			{
				for (uint64 Index = 0; Index < Count; ++Index)
				{
					Construct(Index, Other[Index]);
				}
			}

			return *this;
		}

		T& operator[](uint64 Index)
		{
			return Get(Index);
		}

		const T& operator[](uint64 Index) const
		{
			return Get(Index);
		}

		bool operator==(const Array<T, L>& Other) const
		{
			return Count == Other.Count && GetData() == Other.GetData();
		}

		bool operator!=(const Array<T, L>& Other) const
		{
			return !(*this == Other);
		}

		bool operator>(const Array<T, L>& Other) const
		{
			return Count > Other.Count;
		}

		bool operator>=(const Array<T, L>&Other) const
		{
			return Count >= Other.Count;
		}

		bool operator<(const Array<T, L>& Other) const
		{
			return Count < Other.Count;
		}

		bool operator<=(const Array<T, L>& Other) const
		{
			return Count <= Other.Count;
		}

		T& Assign(uint64 Index, const T& Value)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			T& Item = GetItem(Index);
			Item = Value;
			return Item;
		}

		T& Assign(uint64 Index, T&& Value)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			T& Item = GetItem(Index);
			Item = Move(Value);
			return Item;
		}

		template<typename... Args>
		T& AssignConstruct(uint64 Index, Args&&... args)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			T& Item = GetItem(Index);
			Destruct(Index);
			Construct(Index, args...);
			return Item;
		}

		template<typename C>
		T& AssignRange(uint64 Index, const C& Value)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");
			NX_ASSERT(IsValidIndex(Index + Value.GetCount()  - 1), Default, "Invalid Index");

			uint64 Offset = 0;
			for (typename C::I It = Value.Begin(); It != Value.End(); ++It, ++Offset)
			{
				T& Item = GetItem(Index + Offset);
				Item = *It;
			}

			return GetItem(Index);
		}

		T& Get(uint64 Index) 
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			return GetItem(Index);
		}

		const T& Get(uint64 Index) const
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			return GetItem(Index);
		}

		T& First() 
		{
			NX_ASSERT(IsValidIndex(0), Default, "Invalid Index");

			return GetItem(0);
		}

		const T& First() const
		{
			NX_ASSERT(IsValidIndex(0), Default, "Invalid Index");

			return GetItem(0);
		}

		T& Last() 
		{
			NX_ASSERT(IsValidIndex(Count - 1), Default, "Invalid Index");

			return GetItem(Count - 1);
		}

		const T& Last() const
		{
			NX_ASSERT(IsValidIndex(Count - 1), Default, "Invalid Index");

			return GetItem(Count - 1);
		}

		I GetIterator(uint64 Index)
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			return GetIt(Index);
		}

		const I GetIterator(uint64 Index) const
		{
			NX_ASSERT(IsValidIndex(Index), Default, "Invalid Index");

			return GetIt(Index);
		}

		I begin() { return Begin(); }
		I Begin() 
		{
			return GetIt(0);
		}

		const I begin() const { return Begin(); }
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

		I end() { return End(); }
		I End() 
		{
			return GetIt(Count);
		}

		const I end() const { return End(); }
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

		bool IsValidIndex(uint64 Index) const
		{
			return Index >= 0 && Index < Count;
		}

		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }

	private:
		union Buffer
		{
			T* Heap;
			T Stack[L > 0 ? L : 1];

			Buffer() : Heap(nullptr) {};
			~Buffer() {};
		};

		void Allocate(uint64 Size)
		{
			ValidateCapacity(Size);
			if (!IsStackArray())
			{
				Data.Heap = (T*)Memory::Allocate(sizeof(T) * Count, Alloc);
			}
		}

		void Free()
		{
			if (!IsStackArray())
			{
				Memory::Free(Data.Heap, Alloc);
				Data.Heap = nullptr;
			}
		}

		template<typename... Args>
		void Construct(uint64 Index, Args&&... args)
		{
			Memory::Construct<T>(&GetData()[Index], args...);
		}

		void ConstructRange(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				Memory::Construct<T>(&GetData()[Index + Offset]);
			}
		}

		void Destruct(uint64 Index)
		{
			Memory::Destruct(&GetData()[Index]);
		}

		void DestructRange(uint64 Index, uint64 Size)
		{
			for (uint64 Offset = 0; Offset < Size; ++Offset)
			{
				Memory::Destruct(&GetData()[Index + Offset]);
			}
		}

		T* GetData()
		{
			return IsStackArray() ? Data.Stack : Data.Heap;
		}

		const T* GetData() const
		{
			return IsStackArray() ? Data.Stack : Data.Heap;
		}

		T& GetItem(uint64 Index)
		{
			return GetData()[Index];
		}

		const T& GetItem(uint64 Index) const
		{
			return GetData()[Index];
		}

		I GetIt(uint64 Index)
		{
			return I(GetData(), Index);
		}

		const I GetIt(uint64 Index) const
		{
			return I(const_cast<T*>(GetData()), Index);
		}

		void ValidateCapacity(uint64 Size)
		{
			Count = Size;
		}

		bool IsStackArray() const
		{
			return L == Count;
		}

		Allocator* Alloc;
		uint64 Count;
		Buffer Data;
	};
}
