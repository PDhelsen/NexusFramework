#pragma once

#include "Types/Numbers/Integer.h"
#include "Memory/Memory.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/AllocatorContext.h"
#include "Misc/References.h"
#include "Types/Containers/Node.h"
#include "Debug/Logger/Assert.h"

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

			PreAllocated(uint64 Size, Allocator* Allctr = AllocatorContext::Get())
				: Alloc(Allctr), Capacity(0), Count(0), Data(nullptr), Head(nullptr)
			{
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
					NEXUS_ASSERT(false, Default, "Pool is full");
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
					NEXUS_ASSERT(false, Default, "Trying to recycle invalid address");
					return;
				}

				if (Count == 0)
				{
					NEXUS_ASSERT(false, Default, "Pool is full");
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

			I begin() { return Begin(); }
			I Begin() 
			{
				return GetIteratorIndex(0);
			}

			const I begin() const { return Begin(); }
			const I Begin() const
			{
				return GetIteratorIndex(0);
			}

			I end() { return End(); }
			I End() 
			{
				return GetIteratorIndex(Capacity);
			}

			const I end() const { return End(); }
			const I End() const
			{
				return GetIteratorIndex(Capacity);
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

			I GetIteratorIndex(uint64 Index) const
			{
				return I(Data, Index, Capacity);
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
			using N = Node::NodeDouble<T>;
			using I = Iterator::IteratorNodeSimple<T, N>;

			OnDemand(Allocator* Allctr = AllocatorContext::Get())
				: Alloc(Allctr), Count(0), Unsused(0), Data(nullptr), Head(nullptr)
			{
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
					if (Head)
					{
						Head->Prev = nullptr;
					}
					Unsused--;
				}

				Instance->Prev = nullptr;
				Instance->Next = nullptr;

				if (Data)
				{
					Instance->Next = Data;
					Data->Prev = Instance;
				}
				Data = Instance;

				Count++;
				return &Instance->Value;
			}

			void Recycle(T* Value)
			{
				N* Instance = GetNode(Value);

				if (Instance == nullptr)
				{
					NEXUS_ASSERT(false, Default, "Trying to recycle invalid address");
					return;
				}

				if (Data == Instance)
				{
					Data = Instance->Next;
					if (Data)
					{
						Data->Prev = nullptr;
					}
				}
				else
				{
					if (Instance->Prev)
					{
						Instance->Prev->Next = Instance->Next;
					}
					if (Instance->Next)
					{
						Instance->Next->Prev = Instance->Prev;
					}
				}

				Instance->Next = Head;
				Instance->Prev = nullptr;
				if (Head)
				{
					Head->Prev = Instance;
				}
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
						Instance->Prev = nullptr;
						Head->Prev = Instance;
						Head = Instance;
					}
				}

				Data = nullptr;

				Unsused += Count;
				Count = 0;
			}

			I begin() { return Begin(); }
			I Begin()
			{
				return GetIteratorNode(Data);
			}

			const I begin() const { return Begin(); }
			const I Begin() const
			{
				return GetIteratorNode(Data);
			}

			I end() { return End(); }
			I End()
			{
				return GetIteratorNode(nullptr);
			}

			const I end() const { return End(); }
			const I End() const
			{
				return GetIteratorNode(nullptr);
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
			}

			void Destruct(N* Instance)
			{
				Memory::Destruct<T>(&Instance->Value);
			}

			N* GetNode(T* Position)
			{
				return Node::GetNode<T, N>(Position);
			}

			const N* GetNode(const T* Position) const
			{
				return Node::GetNode<T, N>(Position);
			}

			I GetIteratorNode(N* Instance)
			{
				return I(Instance);
			}

			const I GetIteratorNode(const N* Instance) const
			{
				return I(const_cast<N*>(Instance));
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

		Pool(Allocator* Allctr = AllocatorContext::Get())
			: Data(Allctr)
		{
		}

		Pool(uint64 Size, Allocator* Allctr = AllocatorContext::Get())
			: Data(Size, Allctr)
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

		I begin() { return Begin(); }
		I Begin() 
		{
			return Data.Begin();
		}

		const I begin() const { return Begin(); }
		const I Begin() const
		{
			return Data.Begin();
		}

		I end() { return End(); }
		I End() 
		{
			return Data.End();
		}

		const I end() const { return End(); }
		const I End() const
		{
			return Data.End();
		}

		bool Contains(const T& Other) const
		{
			return Find(Other) != End();
		}

		I Find(const T& Other)
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

		const I Find(const T& Other) const
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
