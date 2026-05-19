#pragma once

#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Dequeue.h"
#include "NexusFramework/Types/Containers/Stack.h"
#include "NexusFramework/Types/Containers/Queue.h"
#include "NexusFramework/Types/Containers/LinkedList.h"
#include "NexusFramework/Types/Containers/Set.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Containers/Tree.h"
#include "NexusFramework/Types/Containers/Graph.h"
#include "NexusFramework/Types/Containers/Collection.h"
#include "NexusFramework/Types/Containers/Buffer.h"
#include "NexusFramework/Types/Containers/Pool.h"
#include "NexusFramework/Types/Containers/Tuple.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Misc/Sort/Sort.h"

namespace NxFr
{
	template<typename T>
	using Predicate = Delegate<bool(const T&)>;

	namespace ContainerUtility
	{
		template<typename T, typename C>
		void Fill(C& Container, const T& Value)
		{
			for (auto& It : Container)
			{
				It = Value;
			}
		}
		template<typename K, typename T>
		void Fill(Dictionary<K, T>& Container, const T& Value)
		{
			for (auto& It : Container)
			{
				It.Value = Value;
			}
		}

		template<typename T>
		void Resize(Array<T>& Container, uint64 Size)
		{
			uint64 Count = Math::Min(Container.GetCount(), Size);
			Array<T> Result = Array<T>(Size);

			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Result[Index] = Container[Index];
			}

			Container = Move(Result);
		}
		template<typename T>
		void Resize(List<T>& Container, uint64 Size)
		{
			if (Size > Container.GetCount())
			{
				uint64 Delta = Size - Container.GetCount();
				while (Delta-- > 0)
				{
					Container.AppendConstruct();
				}
			}
			else if (Size < Container.GetCount())
			{
				uint64 Delta = Container.GetCount() - Size;
				Container.RemoveRange(Size, Delta);
			}
		}

		template<typename T, typename C>
		Array<T> ToArray(const C& Container)
		{
			uint64 Index = 0;
			Array<T> Result = Array<T>(Container.GetCount());

			for (const auto& It : Container)
			{
				Result.AssignConstruct(Index++, It);
			}

			return Result;
		}
		template<typename K, typename T>
		Array<KeyValuePair<K, T>> ToArray(const Dictionary<K, T>& Container)
		{
			uint64 Index = 0;
			Array<KeyValuePair<K, T>> Result = Array<KeyValuePair<K, T>>(Container.GetCount());

			for (const auto& It : Container)
			{
				Result.AssignConstruct(Index++, It.Key, It.Value);
			}

			return Result;
		}
		template<typename K, typename T>
		Array<K> ToArrayKeys(const Dictionary<K, T>& Container)
		{
			uint64 Index = 0;
			Array<K> Result = Array<K>(Container.GetCount());

			for (const auto& It : Container)
			{
				Result.AssignConstruct(Index++, It.Key);
			}

			return Result;
		}
		template<typename K, typename T>
		Array<T> ToArrayValues(const Dictionary<K, T>& Container)
		{
			uint64 Index = 0;
			Array<T> Result = Array<T>(Container.GetCount());

			for (const auto& It : Container)
			{
				Result.AssignConstruct(Index++, It.Value);
			}

			return Result;
		}

		template<typename T, typename C>
		void Swap(C& Container, uint64 A, uint64 B)
		{
			T Temp = Container[A];
			Container[A] = Move(Container[B]);
			Container[B] = Move(Temp);
		}
		template<typename T, typename C>
		void Swap(C& Container, T* A, T* B)
		{
			T Temp = *A;
			*A = Move(*B);
			*B = Move(Temp);
		}
		template<typename T1, typename T2>
		void Swap(Tuple<T1, T2>& Container)
		{
			constexpr bool SameType = IsSameType<T1, T2>::Value;
			NX_ASSERT_STATIC(SameType, "Cannot swap if First and Second are not of the same type");

			T1 Temp = Container.GetSecond();
			Container.SetSecond(Move(Container.GetFirst()));
			Container.SetFirst(Move(Temp));
		}

		template<typename T, typename C, typename S = Sorting::DefaultIndexed<T>>
		void Sort(C& Container, Sorting::CompareFunction<T> Function = nullptr)
		{
			Sort::SortCollection<T, C, S>(Container, Container.GetCount(), Function);
		}
		template<typename T, typename S = Sorting::DefaultLinked<T, typename LinkedList<T>::N>>
		void Sort(LinkedList<T>& Container, Sorting::CompareFunction<T> Function = nullptr)
		{
			typename LinkedList<T>::N* Start = Node::GetNode<T, typename LinkedList<T>::N>(&Container.First());
			typename LinkedList<T>::N* End = Node::GetNode<T, typename LinkedList<T>::N>(&Container.Last());

			Sort::SortNodes<T, typename LinkedList<T>::N, S>(
				&Start,
				[](typename LinkedList<T>::N* Node, typename LinkedList<T>::N* Value) { Node->Next = Value; },
				[](typename LinkedList<T>::N* Node) { return Node->Next; },
				[](typename LinkedList<T>::N* Node) { return Node->Value; },
				Function
			);
			End = Node::RelinkBackward<typename LinkedList<T>::N>(Start);

			Container.Reorder(&Start->Value, &End->Value);
		}

		template<typename T, typename C>
		void Reverse(C& Container)
		{
			uint64 Half = Container.GetCount() / 2;
			for (uint64 Front = 0, Back = Container.GetCount() - 1; Front < Half; ++Front, --Back)
			{
				Swap<T, C>(Container, Front, Back);
			}
		}
		template<typename T>
		void Reverse(LinkedList<T>& Container)
		{
			typename LinkedList<T>::N* Start = Node::GetNode<T, typename LinkedList<T>::N>(&Container.First());
			typename LinkedList<T>::N* End = Node::GetNode<T, typename LinkedList<T>::N>(&Container.Last());

			typename LinkedList<T>::N* Current = Start;
			typename LinkedList<T>::N* Next = Current->Next;
			Current->Next = nullptr;
			Current->Prev = Next;

			while (Next)
			{
				typename LinkedList<T>::N* SecondNext = Next->Next;
				Next->Next = Current;
				Next->Prev = SecondNext;

				Current = Next;
				Next = SecondNext;
			}

			Container.Reorder(&End->Value, &Start->Value);
		}

		template<typename T, typename C>
		typename C::I Where(const C& Container, const Predicate<T>& Predicate)
		{
			for (typename C::I It = Container.Begin(); It != Container.End(); ++It)
			{
				if (Predicate(*It))
				{
					return It;
				}
			}

			return Container.End();
		}
		template<typename K, typename T>
		typename Dictionary<K, T>::I WhereKey(const Dictionary<K, T>& Container, const Predicate<K>& Predicate)
		{
			for (typename Dictionary<K, T>::I It = Container.Begin(); It != Container.End(); ++It)
			{
				if (Predicate(It->Key))
				{
					return It;
				}
			}

			return Container.End();
		}
		template<typename K, typename T>
		typename Dictionary<K, T>::I WhereValue(const Dictionary<K, T>& Container, const Predicate<T>& Predicate)
		{
			for (typename Dictionary<K, T>::I It = Container.Begin(); It != Container.End(); ++It)
			{
				if (Predicate(It->Value))
				{
					return It;
				}
			}

			return Container.End();
		}
		template<typename T, typename C>
		typename C::I Find(const C& Container, const T& Other) { return Where<T, C>(Container, [&](const T& Element) { return Element == Other; }); }
		template<typename K, typename T>
		typename Dictionary<K, T>::I FindKey(const Dictionary<K, T>& Container, const K& Other) { return WhereKey<K, T>(Container, [&](const K& Element) { return Element == Other; }); }
		template<typename K, typename T>
		typename Dictionary<K, T>::I FindValue(const Dictionary<K, T>& Container, const T& Other) { return WhereValue<K, T>(Container, [&](const T& Element) { return Element == Other; }); }

		template<typename T, typename C>
		bool Any(const C& Container, const Predicate<T>& Predicate)
		{
			return Where(Container, Predicate) != Container.End();
		}
		template<typename K, typename T>
		bool AnyKey(const Dictionary<K, T>& Container, const Predicate<K>& Predicate)
		{
			return WhereKey(Container, Predicate) != Container.End();
		}
		template<typename K, typename T>
		bool AnyValue(const Dictionary<K, T>& Container, const Predicate<T>& Predicate)
		{
			return WhereValue(Container, Predicate) != Container.End();
		}
		template<typename T, typename C>
		bool Contains(const C& Container, const T& Other) { return Any<T, C>(Container, [&](const T& Element) { return Element == Other; }); }
		template<typename K, typename T>
		bool ContainsKey(const Dictionary<K, T>& Container, const K& Other) { return AnyKey<K, T>(Container, [&](const K& Element) { return Element == Other; }); }
		template<typename K, typename T>
		bool ContainsValue(const Dictionary<K, T>& Container, const T& Other) { return AnyValue<K, T>(Container, [&](const T& Element) { return Element == Other; }); }
		
		template<typename T, typename C, class H = Hashing::Default>
		void Intersection(Set<T, H>& Base, const C& Other)
		{
			for (typename Set<T, H>::I It = Base.Begin(); It != Base.End(); ++It)
			{
				if (!Contains(Other, *It))
				{
					Base.Remove(*It);
				}
			}
		}
		template<typename T, class H = Hashing::Default>
		void Intersection(Set<T, H>& Base, const Set<T, H>& Other)
		{
			for (typename Set<T, H>::I It = Base.Begin(); It != Base.End(); ++It)
			{
				if (Other.TryGet(*It) == nullptr)
				{
					Base.Remove(*It);
				}
			}
		}
	};
}
