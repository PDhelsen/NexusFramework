#pragma once

#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/Misc/Iterator.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	namespace Pooling
	{
		template<typename T>
		class PreAllocated
		{
		public:
			using N = Node::NodeSimple<T>;
			using I = Iterator::PreAllocated<T, N>;

			NX_NOCOPY_NOMOVE(PreAllocated)
			PreAllocated(uint64 Size, Allocator* Allctr = Allocator::TryGet())
				: Alloc(Allctr), Capacity(0), Count(0), Data(nullptr), Head(nullptr)
			{
				Allocate(Size);
				ConstructRange(0, Capacity);
				ValidateDefaultState();
			}

			~PreAllocated()
			{
				DestructRange(0, Capacity);
				Free();
			}

			bool operator==(const PreAllocated<T>& Other) const
			{
				return Count == Other.Count && Data == Other.Data;
			}

			bool operator!=(const PreAllocated<T>& Other) const
			{
				return !(*this == Other);
			}

			bool operator>(const PreAllocated<T>& Other) const
			{
				return Count > Other.Count;
			}

			bool operator>=(const PreAllocated<T>& Other) const
			{
				return Count >= Other.Count;
			}

			bool operator<(const PreAllocated<T>& Other) const
			{
				return Count < Other.Count;
			}

			bool operator<=(const PreAllocated<T>& Other) const
			{
				return Count <= Other.Count;
			}

			T* Acquire()
			{
				N* Instance = nullptr;

				if (Count == Capacity)
				{
					NX_ASSERT(false, Default, "Pool is full");
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
					NX_ASSERT(false, Default, "Trying to recycle invalid address");
					return;
				}

				if (Count == 0)
				{
					NX_ASSERT(false, Default, "Pool is full");
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
				return GetIt(0);
			}

			const I begin() const { return Begin(); }
			const I Begin() const
			{
				return GetIt(0);
			}

			I end() { return End(); }
			I End() 
			{
				return GetIt(Capacity);
			}

			const I end() const { return End(); }
			const I End() const
			{
				return GetIt(Capacity);
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

			I GetIt(uint64 Index) 
			{
				return I(Data, Index, Capacity);
			}

			const I GetIt(uint64 Index) const
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
			using I = Iterator::NodeSimple<T, N>;

			NX_NOCOPY_NOMOVE(OnDemand)
			OnDemand(Allocator* Allctr = Allocator::TryGet())
				: Alloc(Allctr), Count(0), Unsused(0), Data(nullptr), Head(nullptr)
			{
			}

			~OnDemand()
			{
				Clear();
				Free();
			}

			bool operator==(const OnDemand<T>& Other) const
			{
				return Count == Other.Count && Unsused == Other.Unsused && Data == Other.Data;
			}

			bool operator!=(const OnDemand<T>& Other) const
			{
				return !(*this == Other);
			}

			bool operator>(const OnDemand<T>& Other) const
			{
				return Count > Other.Count;
			}

			bool operator>=(const OnDemand<T>& Other) const
			{
				return Count >= Other.Count;
			}

			bool operator<(const OnDemand<T>& Other) const
			{
				return Count < Other.Count;
			}

			bool operator<=(const OnDemand<T>& Other) const
			{
				return Count <= Other.Count;
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
					NX_ASSERT(false, Default, "Trying to recycle invalid address");
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
				return GetIt(Data);
			}

			const I begin() const { return Begin(); }
			const I Begin() const
			{
				return GetIt(Data);
			}

			I end() { return End(); }
			I End()
			{
				return GetIt(nullptr);
			}

			const I end() const { return End(); }
			const I End() const
			{
				return GetIt(nullptr);
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

			I GetIt(N* Instance)
			{
				return I(Instance);
			}

			const I GetIt(const N* Instance) const
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

		NX_NOCOPY_NOMOVE(Pool)
		Pool(Allocator* Allctr = Allocator::TryGet())
			: Data(Allctr)
		{
		}

		Pool(uint64 Size, Allocator* Allctr = Allocator::TryGet())
			: Data(Size, Allctr)
		{
		}

		~Pool()
		{
		}

		bool operator==(const Pool<T, P>& Other) const
		{
			return Data == Other.Data;
		}

		bool operator!=(const Pool<T, P>& Other) const
		{
			return !(*this == Other);
		}

		bool operator>(const Pool<T, P>& Other) const
		{
			return Data > Other.Data;
		}

		bool operator>=(const Pool<T, P>& Other) const
		{
			return Data >= Other.Data;
		}

		bool operator<(const Pool<T, P>& Other) const
		{
			return Data < Other.Data;
		}

		bool operator<=(const Pool<T, P>& Other) const
		{
			return Data <= Other.Data;
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

		uint64 GetCapacity() const { return Data.GetCapacity(); }
		uint64 GetCount() const { return Data.GetCount(); }
		uint64 GetUnused() const { return Data.GetUnused(); }

	private:
		P Data;
	};
}
