#pragma once

#include "Types/Integer.h"
#include "Types/Containers/Node.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/GlobalAllocator.h"
#include "Memory/Allocator/PoolAllocator.h"
#include "Debug/Assert.h"
#include "Misc/References.h"

namespace NxEn
{
	namespace Pooling
	{
		template<typename T>
		class PreAllocated
		{
		public:
			using N = Node::NodeSimple<T>;
			using I = Iterator::IteratorPreAllocated<T, N>;

			PreAllocated(uint64 Size, Allocator* Allctr = nullptr)
				: Alloc(nullptr), Capacity(0), Count(0), Data(nullptr), Head(nullptr)
			{
				ValidateAllocator(Allctr);
				Allocate(Size);
				ConstructRange(0, Capacity);
				ValidateDefaultState();
			}

			PreAllocated(const PreAllocated<T>& Other) = delete;
			PreAllocated(PreAllocated<T>&& Other) noexcept = delete;

			~PreAllocated()
			{
				DestructRange(0, Capacity);
				Free();
			}

			PreAllocated<T>& operator=(const PreAllocated<T>& Other) = delete;
			PreAllocated<T>& operator=(PreAllocated<T>&& Other) noexcept = delete;

			bool operator==(const PreAllocated<T>& Other) const
			{
				return Count == Other.Count && Data == Other.Data;
			}

			bool operator!=(const PreAllocated<T>& Other) const
			{
				return !(*this == Other);
			}

			T* Acquire()
			{
				N* Instance = nullptr;

				if (Count == Capacity)
				{
					NEXUS_ASSERT(false, "Pool is full");
					return nullptr;
				}

				Instance = Head;
				Head = Head->Next;
				Count++;

				Instance->Next = nullptr;
				return &Instance->Value;
			}

			void Recycle(T* Value)
			{
				N* Instance = Node::GetNode<T, N>(Value);

				if (Instance == nullptr || !Memory::IsPointerInRange(Instance, Data, sizeof(N) * Capacity))
				{
					NEXUS_ASSERT(false, "Trying to recycle invalid address");
					return;
				}

				if (Count == 0)
				{
					NEXUS_ASSERT(false, "Pool is full");
					return;
				}

				Instance->Next = Head;
				Head = Instance;
				Count--;
			}

			void Clear()
			{
				for (int I = 0; I < Capacity; I++)
				{
					N& Instance = Data[I];
					Instance.Next = ValidateNext(&Instance);
				}

				ValidateDefaultState();
			}

			I begin() const { return Begin(); }
			I Begin() const
			{
				return I(Data, 0, Capacity);
			}

			I end() const { return End(); }
			I End() const
			{
				return I(Data, Capacity, Capacity);
			}

			uint64 GetCapacity() const { return Capacity; }
			uint64 GetCount() const { return Count; }
			uint64 GetUnused() const { return Capacity - Count; }

		private:
			void Allocate(uint64 Size)
			{
				ValidateCapacity(Size);
				Data = (N*)Memory::Allocate(sizeof(N) * Capacity, Alloc);
			}

			void Free()
			{
				Memory::Free(Data, Alloc);
				Data = nullptr;
			}

			void ConstructRange(uint64 Index, uint64 Size)
			{
				for (uint64 Offset = 0; Offset < Size; ++Offset)
				{
					N& Instance = Data[Index + Offset];
					Memory::Construct<T>(&Instance.Value);
					Instance.Next = ValidateNext(&Instance);
				}
			}

			void DestructRange(uint64 Index, uint64 Size)
			{
				for (uint64 Offset = 0; Offset < Size; ++Offset)
				{
					N& Instance = Data[Index + Offset];
					Memory::Destruct<T>(&Instance.Value);
					Instance.Next = nullptr;
				}
			}

			void ValidateAllocator(Allocator* Allctr)
			{
				Alloc = Allctr != nullptr ? Allctr : Memory::GetGlobal();
			}

			void ValidateCapacity(uint64 Size)
			{
				Capacity = Size > 1 ? Size : 1;
			}

			void ValidateDefaultState()
			{
				Head = Data;
				Count = 0;
			}

			N* ValidateNext(N* Instance)
			{
				return ++Instance;
			}

			Allocator* Alloc;
			uint64 Capacity;
			uint64 Count;
			N* Data;
			N* Head;
		};

		template<typename T>
		class OnDemand
		{
		public:
			using N = Node::NodeSimple<T>;
			using I = Iterator::IteratorNodeSimple<T, N>;

			OnDemand(Allocator* Allctr = nullptr)
				: Alloc(nullptr), Count(0), Unsused(0), Data(nullptr), Head(nullptr)
			{
				ValidateAllocator(Allctr);
			}

			OnDemand(const OnDemand<T>& Other) = delete;
			OnDemand(OnDemand<T>&& Other) noexcept = delete;

			~OnDemand()
			{
				Clear();
				Free();
			}

			OnDemand<T>& operator=(const OnDemand<T>& Other) = delete;
			OnDemand<T>& operator=(OnDemand<T>&& Other) noexcept = delete;

			bool operator==(const OnDemand<T>& Other) const
			{
				return Count == Other.Count && Unsused == Other.Unsused && Data == Other.Data;
			}

			bool operator!=(const OnDemand<T>& Other) const
			{
				return !(*this == Other);
			}

			T* Acquire()
			{
				N* Instance = nullptr;

				if (Unsused == 0)
				{
					Instance = Allocate();
					Construct(Instance);
				}
				else
				{
					Instance = Head;
					Head = Head->Next;
					Unsused--;
				}

				if (Data)
				{
					Instance->Next = Data;
				}
				Data = Instance;

				Count++;
				return &Instance->Value;
			}

			void Recycle(T* Value)
			{
				N* Instance = Node::GetNode<T, N>(Value);

				if (Instance == nullptr)
				{
					NEXUS_ASSERT(false, "Trying to recycle invalid address");
					return;
				}

				if (Data == Instance)
				{
					Data = Instance->Next;
				}
				else
				{
					N* Current = Data;
					while (Current->Next != Instance)
					{
						Current = Current->Next;
					}
					Current->Next = Instance->Next;
				}

				Instance->Next = Head;
				Head = Instance;
				Count--;
				Unsused++;
			}

			void Clear()
			{
				if (Head == nullptr)
				{
					Head = Data;
				}
				else
				{
					N* Current = Data;
					while (Current)
					{
						N* Instance = Current;
						Current = Current->Next;
						Instance->Next = Head;
						Head = Instance;
					}
				}

				Data = nullptr;

				Unsused += Count;
				Count = 0;
			}

			I begin() const { return Begin(); }
			I Begin() const
			{
				return I(Data);
			}

			I end() const { return End(); }
			I End() const
			{
				return I(nullptr);
			}

			uint64 GetCapacity() const { return Count + Unsused; }
			uint64 GetCount() const { return Count; }
			uint64 GetUnused() const { return Unsused; }

		private:
			N* Allocate()
			{
				return (N*)Memory::Allocate(sizeof(N), Alloc);
			}

			void Free()
			{
				while (Head)
				{
					N* Instance = Head;
					Head = Head->Next;
					Free(Instance);
				}
			}

			void Free(N* Instance)
			{
				Memory::Free(Instance, Alloc);
			}

			void Construct(N* Instance)
			{
				Memory::Construct<T>(&Instance->Value);
				Instance->Next = nullptr;
			}

			void Destruct(N* Instance)
			{
				Memory::Destruct<T>(&Instance->Value);
			}

			void ValidateAllocator(Allocator* Allctr)
			{
				Alloc = Allctr != nullptr ? Allctr : Memory::GetGlobal();
			}

			Allocator* Alloc;
			uint64 Count;
			uint64 Unsused;
			N* Data;
			N* Head;
		};
	}

	template<typename T, typename P = Pooling::OnDemand<T>>
	class Pool
	{
	public:
		using I = typename P::I;

		Pool(Allocator* Alloc = nullptr)
			: Data(Alloc)
		{
		}

		Pool(uint64 Size, Allocator* Alloc = nullptr)
			: Data(Size, Alloc)
		{
		}

		Pool(const Pool<T, P>& Other) = delete;
		Pool(Pool<T, P>&& Other) noexcept = delete;

		~Pool()
		{
		}

		Pool<T, P>& operator=(const Pool<T, P>& Other) = delete;
		Pool<T, P>& operator=(Pool<T, P>&& Other) noexcept = delete;

		bool operator==(const Pool<T, P>& Other) const
		{
			return Data == Other.Data;
		}

		bool operator!=(const Pool<T, P>& Other) const
		{
			return !(*this == Other);
		}

		T& Acquire()
		{
			return *Data.Acquire();
		}

		void Recycle(T& Value)
		{
			Data.Recycle(&Value);
		}

		void Clear()
		{
			Data.Clear();
		}

		I begin() const { return Begin(); }
		I Begin() const
		{
			return Data.Begin();
		}

		I end() const { return End(); }
		I End() const
		{
			return Data.End();
		}

		bool Contains(const T& Other) const
		{
			return Find(Other) != End();
		}

		I Find(const T& Other) const
		{
			for (I It = Begin(); It != End(); ++It)
			{
				if (*It == Other)
				{
					return It;
				}
			}

			return End();
		}

		uint64 GetCapacity() const { return Data.GetCapacity(); }
		uint64 GetCount() const { return Data.GetCount(); }
		uint64 GetUnused() const { return Data.GetUnused(); }

	private:
		P Data;
	};
}
