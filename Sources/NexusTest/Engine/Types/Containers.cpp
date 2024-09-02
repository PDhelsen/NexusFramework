#include "Core/NexusTestPch.h"

#include "Types/Containers/Array.h"
#include "Types/Containers/List.h"
#include "Types/Containers/Dequeue.h"
#include "Types/Containers/Stack.h"
#include "Types/Containers/Queue.h"
#include "Types/Containers/LinkedList.h"
#include "Types/Containers/Set.h"
#include "Types/Containers/Dictionary.h"
#include "Types/Containers/Tree.h"
#include "Types/Containers/Graph.h"
#include "Types/Containers/Collection.h"
#include "Types/Containers/Pool.h"
#include "Types/Containers/Tuple.h"
#include "Types/Containers/ContainersUtils.h"

namespace NxTs
{
	struct ContainerTest
	{
		uint64 Integer;
		float Float;
		bool Boolean;

		bool Destroyed;

		ContainerTest()
		{
			Integer = 1;
			Float = 1.0f;
			Boolean = true;
			Destroyed = false;
		}

		ContainerTest(uint64 Initialization)
		{
			Integer = Initialization;
			Float = 1.0f;
			Boolean = true;
			Destroyed = false;
		}

		ContainerTest(const ContainerTest& Other)
		{
			Integer = Other.Integer;
			Float = Other.Float;
			Boolean = Other.Boolean;
			Destroyed = Other.Destroyed;
		}

		ContainerTest(ContainerTest&& Other) noexcept
		{
			Integer = Other.Integer;
			Float = Other.Float;
			Boolean = Other.Boolean;
			Destroyed = Other.Destroyed;
		}

		~ContainerTest()
		{
			NEXUS_ASSERT(!Destroyed, "Already Destroyed");

			Integer = 0;
			Float = 0.0f;
			Boolean = false;
			Destroyed = true;
		}

		ContainerTest& operator=(const ContainerTest& Other)
		{
			Integer = Other.Integer;
			Float = Other.Float;
			Boolean = Other.Boolean;
			Destroyed = Other.Destroyed;
			return *this;
		}

		ContainerTest& operator=(ContainerTest&& Other) noexcept
		{
			Integer = Other.Integer;
			Float = Other.Float;
			Boolean = Other.Boolean;
			Destroyed = Other.Destroyed;
			return *this;
		}

		bool operator==(const ContainerTest& Other) const
		{
			return Integer == Other.Integer;
		}

		bool operator!=(const ContainerTest& Other) const
		{
			return Integer != Other.Integer;
		}

		bool operator>(const ContainerTest& Other) const
		{
			return Integer > Other.Integer;
		}

		bool operator<(const ContainerTest& Other) const
		{
			return Integer < Other.Integer;
		}

		bool operator>=(const ContainerTest& Other) const
		{
			return Integer >= Other.Integer;
		}

		bool operator<=(const ContainerTest& Other) const
		{
			return Integer <= Other.Integer;
		}
	};
}

namespace NxEn
{
	template<class H>
	struct Hash<NxTs::ContainerTest, H>
	{
		static H::HashLength HashObject(const NxTs::ContainerTest& Data, H::HashLength Seed = 0)
		{
			H Hashing = H(Seed);
			Hashing.Accumulate(&Data.Integer, sizeof(uint64))
				.Accumulate(&Data.Float, sizeof(float))
				.Accumulate(&Data.Boolean, sizeof(bool));
			return Hashing.Hash();
		}
	};
}

namespace NxTs
{
	TEST(Type_Containers, Array)
	{
		NxEn::Array<ContainerTest> Test = NxEn::Array<ContainerTest>(10, nullptr, 5);
		ASSERT_EQ(Test.GetCount(), 10);
		ASSERT_EQ(Test[0].Integer, 5);
	
		ContainerTest Container1 = ContainerTest(1);
		NxEn::Array<ContainerTest> Range = NxEn::Array<ContainerTest>(5, nullptr, 4);

		Test.Assign(0, Container1);
		Test.Assign(1, ContainerTest(2));
		Test.AssignConstruct(2, 3);
		Test.AssignRange(3, Range);
		ASSERT_EQ(Test[0].Integer, 1);
		ASSERT_EQ(Test[1].Integer, 2);
		ASSERT_EQ(Test[2].Integer, 3);
		ASSERT_EQ(Test[3].Integer, 4);
		ASSERT_EQ(Test[7].Integer, 4);

		ContainerTest AccesCopy = Test[5];
		AccesCopy.Integer = 10;
		ASSERT_NE(Test[5].Integer, 10);

		ContainerTest& AccesRef= Test[5];
		AccesRef.Integer = 10;
		ASSERT_EQ(Test[5].Integer, 10);

		NxEn::Array<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy[0].Integer, Test[0].Integer);

		ASSERT_EQ(Test.Get(0).Integer, Test[0].Integer);
		ASSERT_EQ(Test.First().Integer, Test[0].Integer);
		ASSERT_EQ(Test.Last().Integer, Test[Test.GetCount() - 1].Integer);

		ASSERT_EQ(Test.GetIterator(5)->Integer, Test[5].Integer);
		ASSERT_EQ(Test.GetIterator(5).Id(), 5);
		ASSERT_EQ(Test.Begin()->Integer, Test.First().Integer);
		ASSERT_NE(Test.End()->Integer, Test.Last().Integer);
		ASSERT_EQ(Test.BeginReverse()->Integer, Test.Last().Integer);
		ASSERT_NE(Test.EndReverse()->Integer, Test.First().Integer);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.First().Integer, 0);
		ASSERT_EQ(Test.Last().Integer, Test.GetCount() - 1);

		ASSERT_EQ(Test.IsValidIndex(5), true);
		ASSERT_EQ(Test.IsValidIndex(Test.GetCount()), false);

		Test[3].Integer = 3;
		Test[6].Integer = 6;
		Test.Swap(3, 6);
		ASSERT_EQ(Test[3].Integer, 6);
		ASSERT_EQ(Test[6].Integer, 3);

		ContainerTest First = Test.First();
		ContainerTest Last = Test.Last();
		Test.Reverse();
		ASSERT_EQ(Test.First().Integer, Last.Integer);
		ASSERT_EQ(Test.Last().Integer, First.Integer);

		Test.Sort();
		auto ItFirst = Test.Begin();
		auto& ItSecond = ++Test.Begin();
		while (ItSecond != Test.End())
		{
			ASSERT_EQ(ItFirst->Integer < ItSecond->Integer, true);
			ItFirst++;
			ItSecond++;
		}

		ContainerTest ToFind1 = ContainerTest(5);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.Contains(ToFind1), true);
		ASSERT_EQ(Test.Contains(ToFind2), false);
		ASSERT_EQ(Test.Find(ToFind1)->Integer, ToFind1.Integer);
		ASSERT_EQ(Test.Find(ToFind2), Test.End());
	}

	TEST(Type_Containers, List)
	{
		NxEn::List<ContainerTest> Test = NxEn::List<ContainerTest>(10);
		ASSERT_EQ(Test.GetCapacity(), 10);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxEn::List<ContainerTest> Range = NxEn::List<ContainerTest>();
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);

		Test.Append(Container1);
		Test.Append(ContainerTest(2));
		Test.AppendConstruct(3);
		Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCapacity(), 10);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[0].Integer, 1);
		ASSERT_EQ(Test[1].Integer, 2);
		ASSERT_EQ(Test[2].Integer, 3);
		ASSERT_EQ(Test[3].Integer, 4);
		ASSERT_EQ(Test[7].Integer, 4);

		Test.Assign(7, Container1);
		Test.Assign(6, ContainerTest(2));
		Test.AssignConstruct(5, 3);
		Test.AssignRange(0, Range);
		ASSERT_EQ(Test.GetCapacity(), 10);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false); 
		ASSERT_EQ(Test[7].Integer, 1);
		ASSERT_EQ(Test[6].Integer, 2);
		ASSERT_EQ(Test[5].Integer, 3);
		ASSERT_EQ(Test[4].Integer, 4);
		ASSERT_EQ(Test[0].Integer, 4);

		Test.Insert(3, Container1);
		Test.Insert(4, ContainerTest(2));
		Test.InsertConstruct(5, 3);
		Test.InsertRange(6, Range);
		ASSERT_EQ(Test.GetCapacity() > 10, true);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[3].Integer, 1);
		ASSERT_EQ(Test[4].Integer, 2);
		ASSERT_EQ(Test[5].Integer, 3);
		ASSERT_EQ(Test[6].Integer, 4);
		ASSERT_EQ(Test[11].Integer, 4);

		Test.Remove(3);
		Test.RemoveLast();
		Test.RemoveSwap(3);
		Test.RemoveRange(3, 3);
		ASSERT_EQ(Test.GetCapacity() > 10, true);
		ASSERT_EQ(Test.GetCount(), 10);
		ASSERT_EQ(Test.IsEmpty(), false);

		ContainerTest AccesCopy = Test[5];
		AccesCopy.Integer = 10;
		ASSERT_NE(Test[5].Integer, 10);

		ContainerTest& AccesRef = Test[5];
		AccesRef.Integer = 10;
		ASSERT_EQ(Test[5].Integer, 10);

		NxEn::List<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy[0].Integer, Test[0].Integer);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get(0).Integer, Test[0].Integer);
		ASSERT_EQ(Test.First().Integer, Test[0].Integer);
		ASSERT_EQ(Test.Last().Integer, Test[Test.GetCount() - 1].Integer);

		ASSERT_EQ(Test.GetIterator(5)->Integer, Test[5].Integer);
		ASSERT_EQ(Test.GetIterator(5).Id(), 5);
		ASSERT_EQ(Test.Begin()->Integer, Test.First().Integer);
		ASSERT_NE(Test.End()->Integer, Test.Last().Integer);
		ASSERT_EQ(Test.BeginReverse()->Integer, Test.Last().Integer);
		ASSERT_NE(Test.EndReverse()->Integer, Test.First().Integer);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.First().Integer, 0);
		ASSERT_EQ(Test.Last().Integer, Test.GetCount() - 1);

		ASSERT_EQ(Test.IsValidIndex(5), true);
		ASSERT_EQ(Test.IsValidIndex(Test.GetCount()), false);
		Test.Grow(30);
		ASSERT_EQ(Test.GetCapacity(), 30);
		Test.Shrink();
		ASSERT_EQ(Test.GetCapacity(), 10);

		Test[3].Integer = 3;
		Test[6].Integer = 6;
		Test.Swap(3, 6);
		ASSERT_EQ(Test[3].Integer, 6);
		ASSERT_EQ(Test[6].Integer, 3);

		ContainerTest First = Test.First();
		ContainerTest Last = Test.Last();
		Test.Reverse();
		ASSERT_EQ(Test.First().Integer, Last.Integer);
		ASSERT_EQ(Test.Last().Integer, First.Integer);

		Test.Sort();
		auto ItFirst = Test.Begin();
		auto& ItSecond = ++Test.Begin();
		while (ItSecond != Test.End())
		{
			ASSERT_EQ(ItFirst->Integer < ItSecond->Integer, true);
			ItFirst++;
			ItSecond++;
		}

		ContainerTest ToFind1 = ContainerTest(5);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.Contains(ToFind1), true);
		ASSERT_EQ(Test.Contains(ToFind2), false);
		ASSERT_EQ(Test.Find(ToFind1)->Integer, ToFind1.Integer);
		ASSERT_EQ(Test.Find(ToFind2), Test.End());
	}

	TEST(Type_Containers, Dequeue)
	{
		NxEn::Dequeue<ContainerTest> Test = NxEn::Dequeue<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxEn::Dequeue<ContainerTest> Range = NxEn::Dequeue<ContainerTest>();
		Range.AppendBack(4);
		Range.AppendBack(4);
		Range.AppendBack(4);
		Range.AppendBack(4);
		Range.AppendBack(4);

		Test.AppendBack(Container1);
		Test.AppendBack(ContainerTest(2));
		Test.AppendBackConstruct(3);
		Test.AppendBackRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[0].Integer, 1);
		ASSERT_EQ(Test[1].Integer, 2);
		ASSERT_EQ(Test[2].Integer, 3);
		ASSERT_EQ(Test[3].Integer, 4);
		ASSERT_EQ(Test[7].Integer, 4);

		Test.AppendFront(Container1);
		Test.AppendFront(ContainerTest(2));
		Test.AppendFrontConstruct(3);
		Test.AppendFrontRange(Range);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[7].Integer, 1);
		ASSERT_EQ(Test[6].Integer, 2);
		ASSERT_EQ(Test[5].Integer, 3);
		ASSERT_EQ(Test[4].Integer, 4);
		ASSERT_EQ(Test[0].Integer, 4);

		Test.Assign(3, Container1);
		Test.Assign(4, ContainerTest(2));
		Test.AssignConstruct(5, 3);
		Test.AssignRange(6, Range);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[3].Integer, 1);
		ASSERT_EQ(Test[4].Integer, 2);
		ASSERT_EQ(Test[5].Integer, 3);
		ASSERT_EQ(Test[6].Integer, 4);
		ASSERT_EQ(Test[11].Integer, 4);

		Test.RemoveBack();
		Test.RemoveFront();
		ASSERT_EQ(Test.GetCount(), 14);
		ASSERT_EQ(Test.IsEmpty(), false);

		ContainerTest AccesCopy = Test[5];
		AccesCopy.Integer = 10;
		ASSERT_NE(Test[5].Integer, 10);

		ContainerTest& AccesRef = Test[5];
		AccesRef.Integer = 10;
		ASSERT_EQ(Test[5].Integer, 10);

		NxEn::Dequeue<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy[0].Integer, Test[0].Integer);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get(0).Integer, Test[0].Integer);
		ASSERT_EQ(Test.First().Integer, Test[0].Integer);
		ASSERT_EQ(Test.Last().Integer, Test[Test.GetCount() - 1].Integer);

		ASSERT_EQ(Test.GetIterator(5)->Integer, Test[5].Integer);
		ASSERT_EQ(Test.GetIterator(5).Id(), 5);
		ASSERT_EQ(Test.Begin()->Integer, Test.First().Integer);
		ASSERT_NE(Test.End()->Integer, Test.Last().Integer);
		ASSERT_EQ(Test.BeginReverse()->Integer, Test.Last().Integer);
		ASSERT_NE(Test.EndReverse()->Integer, Test.First().Integer);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.First().Integer, 0);
		ASSERT_EQ(Test.Last().Integer, Test.GetCount() - 1);

		ASSERT_EQ(Test.IsValidIndex(5), true);
		ASSERT_EQ(Test.IsValidIndex(Test.GetCount()), false);

		Test[3].Integer = 3;
		Test[6].Integer = 6;
		Test.Swap(3, 6);
		ASSERT_EQ(Test[3].Integer, 6);
		ASSERT_EQ(Test[6].Integer, 3);

		ContainerTest First = Test.First();
		ContainerTest Last = Test.Last();
		Test.Reverse();
		ASSERT_EQ(Test.First().Integer, Last.Integer);
		ASSERT_EQ(Test.Last().Integer, First.Integer);

		Test.Sort();
		auto ItFirst = Test.Begin();
		auto& ItSecond = ++Test.Begin();
		while (ItSecond != Test.End())
		{
			ASSERT_EQ(ItFirst->Integer < ItSecond->Integer, true);
			ItFirst++;
			ItSecond++;
		}

		ContainerTest ToFind1 = ContainerTest(5);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.Contains(ToFind1), true);
		ASSERT_EQ(Test.Contains(ToFind2), false);
		ASSERT_EQ(Test.Find(ToFind1)->Integer, ToFind1.Integer);
		ASSERT_EQ(Test.Find(ToFind2), Test.End());
	}

	TEST(Type_Containers, Stack)
	{
		NxEn::Stack<ContainerTest> Test = NxEn::Stack<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxEn::Stack<ContainerTest> Range = NxEn::Stack<ContainerTest>();
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);

		ContainerTest& Index1 = Test.Append(Container1);
		ContainerTest& Index2 = Test.Append(ContainerTest(2));
		ContainerTest& Index3 = Test.AppendConstruct(3);
		ContainerTest& Index4 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 1);
		ASSERT_EQ(Index2.Integer, 2);
		ASSERT_EQ(Index3.Integer, 3);
		ASSERT_EQ(Index4.Integer, 4);

		Test.Remove();
		Test.Remove();
		Test.Remove();
		Test.Remove();
		ASSERT_EQ(Test.GetCount(), 4);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxEn::Stack<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy.Get().Integer, Test.Get().Integer);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get().Integer, 4);

		ASSERT_EQ(Test.Begin()->Integer, Index1.Integer);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.Begin()->Integer, 0);

		Test.Reverse();
		auto ItFirst = Test.Begin();
		auto& ItSecond = ++Test.Begin();
		while (ItSecond != Test.End())
		{
			ASSERT_EQ(ItFirst->Integer > ItSecond->Integer, true);
			ItFirst++;
			ItSecond++;
		}

		ContainerTest ToFind1 = ContainerTest(2);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.Contains(ToFind1), true);
		ASSERT_EQ(Test.Contains(ToFind2), false);
		ASSERT_EQ(Test.Find(ToFind1)->Integer, ToFind1.Integer);
		ASSERT_EQ(Test.Find(ToFind2), Test.End());
	}

	TEST(Type_Containers, Queue)
	{
		NxEn::Queue<ContainerTest> Test = NxEn::Queue<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxEn::Queue<ContainerTest> Range = NxEn::Queue<ContainerTest>();
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);

		ContainerTest& Index1 = Test.Append(Container1);
		ContainerTest& Index2 = Test.Append(ContainerTest(2));
		ContainerTest& Index3 = Test.AppendConstruct(3);
		ContainerTest& Index4 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 1);
		ASSERT_EQ(Index2.Integer, 2);
		ASSERT_EQ(Index3.Integer, 3);
		ASSERT_EQ(Index4.Integer, 4);

		Test.Remove();
		Test.Remove();
		ASSERT_EQ(Test.GetCount(), 6);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxEn::Queue<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy.Get().Integer, Test.Get().Integer);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get().Integer, 3);

		ASSERT_EQ(Test.Begin()->Integer, Index3.Integer);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.Begin()->Integer, 0);

		Test.Reverse();
		auto ItFirst = Test.Begin();
		auto& ItSecond = ++Test.Begin();
		while (ItSecond != Test.End())
		{
			ASSERT_EQ(ItFirst->Integer > ItSecond->Integer, true);
			ItFirst++;
			ItSecond++;
		}

		ContainerTest ToFind1 = ContainerTest(4);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.Contains(ToFind1), true);
		ASSERT_EQ(Test.Contains(ToFind2), false);
		ASSERT_EQ(Test.Find(ToFind1)->Integer, ToFind1.Integer);
		ASSERT_EQ(Test.Find(ToFind2), Test.End());
	}

	TEST(Type_Containers, LinkedList)
	{
		NxEn::LinkedList<ContainerTest> Test = NxEn::LinkedList<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxEn::LinkedList<ContainerTest> Range = NxEn::LinkedList<ContainerTest>();
		Range.AppendBack(4);
		Range.AppendBack(4);
		Range.AppendBack(4);
		Range.AppendBack(4);
		Range.AppendBack(4);

		ContainerTest& Index1 = Test.AppendBack(Container1);
		ContainerTest& Index2 = Test.AppendBack(ContainerTest(2));
		ContainerTest& Index3 = Test.AppendBackConstruct(3);
		ContainerTest& Index4 = Test.AppendBackRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 1);
		ASSERT_EQ(Index2.Integer, 2);
		ASSERT_EQ(Index3.Integer, 3);
		ASSERT_EQ(Index4.Integer, 4);

		ContainerTest& Index5 = Test.AppendFront(Container1);
		ContainerTest& Index6 = Test.AppendFront(ContainerTest(2));
		ContainerTest& Index7 = Test.AppendFrontConstruct(3);
		ContainerTest& Index8 = Test.AppendFrontRange(Range);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Integer, 1);
		ASSERT_EQ(Index6.Integer, 2);
		ASSERT_EQ(Index7.Integer, 3);
		ASSERT_EQ(Index8.Integer, 4);

		Index5 = Test.Assign(&Index5, Container1);
		Index6 = Test.Assign(&Index6, ContainerTest(2));
		Index7 = Test.AssignConstruct(&Index7, 3);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Integer, 1);
		ASSERT_EQ(Index6.Integer, 2);
		ASSERT_EQ(Index7.Integer, 3);

		ContainerTest& Index9 = Test.InsertFront(&Index1, Container1);
		ContainerTest& Index10 = Test.InsertFront(&Index2, ContainerTest(2));
		ContainerTest& Index11 = Test.InsertFrontConstruct(&Index3, 3);
		ContainerTest& Index12 = Test.InsertFrontRange(&Index4, Range);
		ASSERT_EQ(Test.GetCount(), 24);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index9.Integer, 1);
		ASSERT_EQ(Index10.Integer, 2);
		ASSERT_EQ(Index11.Integer, 3);
		ASSERT_EQ(Index12.Integer, 4);

		ContainerTest& Index13 = Test.InsertBack(&Index1, Container1);
		ContainerTest& Index14 = Test.InsertBack(&Index2, ContainerTest(2));
		ContainerTest& Index15 = Test.InsertBackConstruct(&Index3, 3);
		ContainerTest& Index16 = Test.InsertBackRange(&Index4, Range);
		ASSERT_EQ(Test.GetCount(), 32);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index13.Integer, 1);
		ASSERT_EQ(Index14.Integer, 2);
		ASSERT_EQ(Index15.Integer, 3);
		ASSERT_EQ(Index16.Integer, 4);

		Test.Remove(&Index1, &Index3);
		Test.Remove(&Index4);
		Test.RemoveBack();
		Test.RemoveFront();
		ASSERT_EQ(Test.GetCount(), 22);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxEn::LinkedList<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy.First().Integer, Test.First().Integer);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get().Integer, Test.First().Integer);
		ASSERT_EQ(Test.TryGetPrev(&Test.First()), nullptr);
		ASSERT_EQ(Test.TryGetNext(&Test.Last()), nullptr);

		ContainerTest& Next = Test.GetNext(&Index5);
		ContainerTest& Prev = Test.GetPrev(&Index5);
		ASSERT_EQ(Test.IsNext(&Index5, &Next), true);
		ASSERT_EQ(Test.IsPrevious(&Index5, &Prev), true);

		ASSERT_EQ(Test.GetIterator(&Index5)->Integer, Index5.Integer);
		ASSERT_EQ(Test.GetIterator(&Index5).Next()->Integer, Test.GetNext(&Index5).Integer);
		ASSERT_EQ(Test.GetIterator(&Index5).Previous()->Integer, Test.GetPrev(&Index5).Integer);
		ASSERT_EQ(Test.Begin()->Integer, Test.First().Integer);
		ASSERT_EQ(Test.End(), nullptr);
		ASSERT_EQ(Test.BeginReverse()->Integer, Test.Last().Integer);
		ASSERT_EQ(Test.EndReverse(), nullptr);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.First().Integer, 0);
		ASSERT_EQ(Test.Last().Integer, Test.GetCount() - 1);

		ContainerTest A = Test.First();
		ContainerTest B = Test.Last();
		Test.Swap(&Test.First(), &Test.Last());
		ASSERT_EQ(Test.First().Integer, B.Integer);
		ASSERT_EQ(Test.Last().Integer, A.Integer);

		ContainerTest First = Test.First();
		ContainerTest Last = Test.Last();
		Test.Reverse();
		ASSERT_EQ(Test.First().Integer, Last.Integer);
		ASSERT_EQ(Test.Last().Integer, First.Integer);

		Test.Sort();
		auto ItFirst = Test.Begin();
		auto& ItSecond = ++Test.Begin();
		while (ItSecond != Test.End())
		{
			ASSERT_EQ(ItFirst->Integer < ItSecond->Integer, true);
			ItFirst++;
			ItSecond++;
		}

		ContainerTest ToFind1 = ContainerTest(5);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.Contains(ToFind1), true);
		ASSERT_EQ(Test.Contains(ToFind2), false);
		ASSERT_EQ(Test.Find(ToFind1)->Integer, ToFind1.Integer);
		ASSERT_EQ(Test.Find(ToFind2), Test.End());
	}

	TEST(Type_Containers, Set)
	{
		NxEn::Set<ContainerTest> Test = NxEn::Set<ContainerTest>();
		ASSERT_EQ(Test.GetCapacity(), 11);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxEn::Set<ContainerTest> Range = NxEn::Set<ContainerTest>();
		Range.Append(6);
		Range.Append(7);
		Range.Append(8);
		Range.Append(9);
		Range.Append(10);

		const ContainerTest& Index1 = Test.Append(Container1);
		const ContainerTest& Index2 = Test.Append(ContainerTest(2));
		const ContainerTest& Index3 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 7);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 1);
		ASSERT_EQ(Index2.Integer, 2);

		Test.Remove(8);
		ASSERT_EQ(Test.GetCount(), 6);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxEn::Set<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.GetIterator(6)->Integer, 6);

		Test.Grow(21);
		ASSERT_EQ(Test.GetCapacity(), 21);

		ContainerTest ToFind1 = ContainerTest(6);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.Contains(ToFind1), true);
		ASSERT_EQ(Test.Contains(ToFind2), false);
		ASSERT_EQ(Test.Find(ToFind1)->Integer, ToFind1.Integer);
		ASSERT_EQ(Test.Find(ToFind2), Test.End());
	}

	TEST(Type_Containers, Dictionary)
	{
		NxEn::Dictionary<ContainerTest, ContainerTest> Test = NxEn::Dictionary<ContainerTest, ContainerTest>();
		ASSERT_EQ(Test.GetCapacity(), 11);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxEn::Dictionary<ContainerTest, ContainerTest> Range = NxEn::Dictionary<ContainerTest, ContainerTest>();
		Range.Append(6, 60);
		Range.Append(7, 70);
		Range.Append(8, 80);
		Range.Append(9, 90);
		Range.Append(10, 100);

		const ContainerTest& Index1 = Test.Append(Container1, 10);
		const ContainerTest& Index2 = Test.Append(ContainerTest(2), 20);
		const ContainerTest& Index3 = Test.AppendConstruct(ContainerTest(3), 30);
		const ContainerTest& Index4 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 10);
		ASSERT_EQ(Index2.Integer, 20);
		ASSERT_EQ(Index3.Integer, 30);

		const ContainerTest& Index5 = Test.Assign(Container1, 100);
		const ContainerTest& Index6 = Test.Assign(ContainerTest(2), 200);
		const ContainerTest& Index7 = Test.AssignConstruct(ContainerTest(3), 300);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Integer, 100);
		ASSERT_EQ(Index6.Integer, 200);
		ASSERT_EQ(Index7.Integer, 300);

		Test.Remove(8);
		ASSERT_EQ(Test.GetCount(), 7);
		ASSERT_EQ(Test.IsEmpty(), false);

		ContainerTest AccesCopy = Test[2];
		AccesCopy.Integer = 2000;
		ASSERT_NE(Test[2].Integer, 2000);

		ContainerTest& AccesRef = Test[2];
		AccesCopy.Integer = 2000;
		ASSERT_NE(Test[2].Integer, 2000);

		NxEn::Dictionary<ContainerTest, ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get(9).Integer, 90);
		ASSERT_EQ(Test.GetIterator(6)->GetKey().Integer, 6);

		for (auto& Kv : Test)
		{
			Kv.GetValue().Integer = Kv.GetKey().Integer * 10;
		}

		Test.Grow(21);
		ASSERT_EQ(Test.GetCapacity(), 21);

		Test.Swap(6, 7);
		ASSERT_EQ(Test[6].Integer, 70);
		ASSERT_EQ(Test[7].Integer, 60);

		ContainerTest ToFind1 = ContainerTest(3);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.ContainsKey(ToFind1), true);
		ASSERT_EQ(Test.ContainsValue(ToFind2), true);
		ASSERT_EQ(Test.FindKey(ToFind1)->GetKey().Integer, ToFind1.Integer);
		ASSERT_EQ(Test.FindValue(ToFind2)->GetValue().Integer, ToFind2.Integer);
	}

	TEST(Type_Containers, Tree)
	{
		NxEn::Tree<ContainerTest> Test = NxEn::Tree<ContainerTest>();
		ContainerTest& Root = Test.Append(nullptr, 0);
		ASSERT_EQ(Test.GetCount(), 1);
		ASSERT_EQ(Test.IsEmpty(), false);

		ContainerTest Container1 = ContainerTest(1);
		NxEn::Tree<ContainerTest> Range = NxEn::Tree<ContainerTest>();
		ContainerTest& RangeRoot = Range.Append(nullptr, 0);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);

		ContainerTest& Index1 = Test.Append(&Root, Container1);
		ContainerTest& Index2 = Test.Append(&Root, ContainerTest(2));
		ContainerTest& Index3 = Test.AppendConstruct(&Root, 3);
		ContainerTest& Index4 = Test.AppendRange(&Root, Range);
		ASSERT_EQ(Test.GetCount(), 10);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 1);
		ASSERT_EQ(Index2.Integer, 2);
		ASSERT_EQ(Index3.Integer, 3);
		ASSERT_EQ(Index4.Integer, 0);

		Index1 = Test.Assign(&Index1, Container1);
		Index2 = Test.Assign(&Index2, ContainerTest(2));
		Index3 = Test.AssignConstruct(&Index3, 3);
		ASSERT_EQ(Test.GetCount(), 10);
		ASSERT_EQ(Test.IsEmpty(), false);

		ContainerTest& Index5 = Test.InsertSibling(&Index1, Container1);
		ContainerTest& Index6 = Test.InsertSibling(&Index2, ContainerTest(5));
		ContainerTest& Index7 = Test.InsertSibling(&Index3, 6);
		ASSERT_EQ(Test.GetCount(), 13);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Integer, 1);
		ASSERT_EQ(Index6.Integer, 5);
		ASSERT_EQ(Index7.Integer, 6);

		ContainerTest& Index8 = Test.InsertChild(&Index1, Container1);
		ContainerTest& Index9 = Test.InsertChild(&Index2, ContainerTest(7));
		ContainerTest& Index10 = Test.InsertChild(&Index3, 8);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index8.Integer, 1);
		ASSERT_EQ(Index9.Integer, 7);
		ASSERT_EQ(Index10.Integer, 8);

		Test.Remove(&Index2);
		Test.RemoveChildren(&Index3);
		ASSERT_EQ(Test.GetCount(), 13);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxEn::Tree<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy.Begin()->Integer, Test.Begin()->Integer);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get().Integer, 0);
		ASSERT_EQ(Test.GetParent(&Index1).Integer, Root.Integer);
		ASSERT_EQ(Test.GetSibling(&Index1).Integer, 1);
		ASSERT_EQ(Test.GetChild(&Index1).Integer, 1);
		ASSERT_EQ(Test.IsParent(&Index1, &Root), true);
		ASSERT_EQ(Test.IsSibling(&Index1, &Index5), true);
		ASSERT_EQ(Test.IsChild(&Index1, &Index8), true);

		ASSERT_EQ(Test.GetIterator(&Index5)->Integer, Index5.Integer);
		ASSERT_EQ(Test.GetIterator(&Index8).Parent()->Integer, Index1.Integer);
		ASSERT_EQ(Test.GetIterator(&Index1).Sibling()->Integer, Index5.Integer);
		ASSERT_EQ(Test.GetIterator(&Index1).Child()->Integer, Index8.Integer);
		ASSERT_EQ(Test.Begin()->Integer, Root.Integer);
		ASSERT_EQ(Test.End(), nullptr);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}

		ContainerTest A = Index8;
		ContainerTest B = Index5;
		Test.Swap(&A, &B);
		ASSERT_EQ(Index8.Integer, B.Integer);
		ASSERT_EQ(Index5.Integer, A.Integer);

		ContainerTest ToFind1 = ContainerTest(5);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.Contains(ToFind1), true);
		ASSERT_EQ(Test.Contains(ToFind2), false);
		ASSERT_EQ(Test.Find(ToFind1)->Integer, ToFind1.Integer);
		ASSERT_EQ(Test.Find(ToFind2), Test.End());
	}

	TEST(Type_Containers, Graph)
	{
		NxEn::Graph<ContainerTest> Test = NxEn::Graph<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxEn::Graph<ContainerTest> Range = NxEn::Graph<ContainerTest>();
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);

		ContainerTest& Index1 = Test.Append(Container1);
		ContainerTest& Index2 = Test.Append(ContainerTest(2));
		ContainerTest& Index3 = Test.AppendConstruct(3);
		ContainerTest& Index4 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 1);
		ASSERT_EQ(Index2.Integer, 2);
		ASSERT_EQ(Index3.Integer, 3);
		ASSERT_EQ(Index4.Integer, 4);

		Index1 = Test.Assign(&Index1, Container1);
		Index2 = Test.Assign(&Index2, ContainerTest(2));
		Index3 = Test.AssignConstruct(&Index3, 3);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);

		Test.Remove(&Index4);
		ASSERT_EQ(Test.GetCount(), 7);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxEn::Graph<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		Test.Connect(&Index1, &Index2);
		Test.Connect(&Index1, &Index3);
		Test.Connect(&Index2, &Index3);
		ASSERT_EQ(Test.GetConnection(&Index1, NxEn::Graph<ContainerTest>::ConnectionType::To, 0).Integer, 2);
		ASSERT_EQ(Test.GetConnection(&Index1, NxEn::Graph<ContainerTest>::ConnectionType::To, 1).Integer, 3);
		ASSERT_EQ(Test.GetConnection(&Index2, NxEn::Graph<ContainerTest>::ConnectionType::To, 0).Integer, 3);
		ASSERT_EQ(Test.IsConnected(&Index1, &Index2, NxEn::Graph<ContainerTest>::ConnectionType::To), true);
		ASSERT_EQ(Test.IsConnected(&Index3, &Index1, NxEn::Graph<ContainerTest>::ConnectionType::From), true);

		Test.Disconnect(&Index1, &Index3);
		ASSERT_EQ(Test.IsConnected(&Index3, &Index1, NxEn::Graph<ContainerTest>::ConnectionType::From), false);

		ASSERT_EQ(Test.GetIterator(&Index1)->Integer, Index1.Integer);
		ASSERT_EQ(Test.GetIterator(&Index1).Connections(NxEn::Graph<ContainerTest>::ConnectionType::To, 0)->Integer, Index2.Integer);
		ASSERT_EQ(Test.Begin()->Integer, Test.Get().Integer);
		ASSERT_EQ(Test.End(), nullptr);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}

		ContainerTest A = Index1;
		ContainerTest B = Index2;
		Test.Swap(&A, &B);
		ASSERT_EQ(Index1.Integer, B.Integer);
		ASSERT_EQ(Index2.Integer, A.Integer);

		ContainerTest ToFind1 = ContainerTest(5);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.Contains(ToFind1), true);
		ASSERT_EQ(Test.Contains(ToFind2), false);
		ASSERT_EQ(Test.Find(ToFind1)->Integer, ToFind1.Integer);
		ASSERT_EQ(Test.Find(ToFind2), Test.End());
	}

	TEST(Type_Containers, Collection)
	{
		NxEn::List<ContainerTest> Data1 = NxEn::List<ContainerTest>(5);
		Data1.Append(1);
		Data1.Append(2);
		Data1.Append(3);
		Data1.Append(4);
		Data1.Append(5);

		NxEn::Queue<ContainerTest> Data2 = NxEn::Queue<ContainerTest>();
		Data2.Append(1);
		Data2.Append(2);
		Data2.Append(3);
		Data2.Append(4);
		Data2.Append(5);

		uint64 Index = 0;
		NxEn::Collection<ContainerTest> Test = Data2;
		for (auto& It = Test.Current(); It != Test.End(); ++It)
		{
			ASSERT_EQ(It->Integer, ++Index);
		}

		auto& It = Test.Reset();
		ASSERT_EQ((*It).Integer, 1);
	}

	TEST(Type_Containers, ContainersUtils)
	{
		NxEn::Set<ContainerTest> Base = NxEn::Set<ContainerTest>();
		Base.Append(1);
		Base.Append(3);
		Base.Append(5);
		Base.Append(7);
		Base.Append(9);

		NxEn::Set<ContainerTest> Other = NxEn::Set<ContainerTest>();
		Other.Append(2);
		Other.Append(4);
		Other.Append(6);
		Other.Append(8);
		Other.Append(10);

		NxEn::ContainersUtils::SetUnion(Base, Other);
		ASSERT_EQ(Base.GetCount(), 10);

		NxEn::ContainersUtils::SetDifference(Base, Other);
		ASSERT_EQ(Base.GetCount(), 5);

		auto It = Other.Begin();
		Base.Append(*It++);
		Base.Append(*It++);
		Base.Append(*It++);

		NxEn::ContainersUtils::SetIntersection(Base, Other);
		ASSERT_EQ(Base.GetCount(), 3);
	}

	TEST(Type_Containers, Pool)
	{
		NxEn::Pool<ContainerTest> Test = NxEn::Pool<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest& Test1 = Test.Acquire();
		ASSERT_EQ(Test1.Integer, 1);
		ASSERT_EQ(Test.GetCount(), 0);

		ContainerTest& Test2 = Test.Acquire();
		ASSERT_EQ(Test2.Integer, 1);
		ASSERT_EQ(Test.GetCount(), 0);

		ContainerTest& Test3 = Test.Acquire();
		ASSERT_EQ(Test3.Integer, 1);
		ASSERT_EQ(Test.GetCount(), 0);

		Test.Recycle(Test1);
		ASSERT_EQ(Test.GetCount(), 1);
		Test.Recycle(Test2);
		ASSERT_EQ(Test.GetCount(), 2);
		Test.Recycle(Test3);
		ASSERT_EQ(Test.GetCount(), 3);

		ContainerTest& Test4 = Test.Acquire();
		ASSERT_EQ(&Test4, &Test3);
		ASSERT_EQ(Test.GetCount(), 2);

		ContainerTest& Test5 = Test.Acquire();
		ASSERT_EQ(&Test5, &Test2);
		ASSERT_EQ(Test.GetCount(), 1);

		Test.Recycle(Test4);
		Test.Recycle(Test5);
		Test.Clear();
		ASSERT_EQ(Test.GetCount(), 0);

		NxEn::Pool<ContainerTest> TestAllocator = NxEn::Pool<ContainerTest>(10);

		ContainerTest& Test6 = TestAllocator.Acquire();
		ASSERT_EQ(Test6.Integer, 1);
		ASSERT_EQ(TestAllocator.GetCount(), 0);

		ContainerTest& Test7 = TestAllocator.Acquire();
		ASSERT_EQ(Test7.Integer, 1);
		ASSERT_EQ(TestAllocator.GetCount(), 0);

		ContainerTest& Test8 = TestAllocator.Acquire();
		ASSERT_EQ(Test8.Integer, 1);
		ASSERT_EQ(TestAllocator.GetCount(), 0);

		TestAllocator.Recycle(Test6);
		ASSERT_EQ(TestAllocator.GetCount(), 1);
		TestAllocator.Recycle(Test7);
		ASSERT_EQ(TestAllocator.GetCount(), 2);
		TestAllocator.Recycle(Test8);
		ASSERT_EQ(TestAllocator.GetCount(), 3);

		ContainerTest& Test9 = TestAllocator.Acquire();
		ASSERT_EQ(&Test9, &Test8);
		ASSERT_EQ(TestAllocator.GetCount(), 2);

		ContainerTest& Test10 = TestAllocator.Acquire();
		ASSERT_EQ(&Test10, &Test7);
		ASSERT_EQ(TestAllocator.GetCount(), 1);

		TestAllocator.Recycle(Test9);
		TestAllocator.Recycle(Test10);
		TestAllocator.Clear();
		ASSERT_EQ(TestAllocator.GetCount(), 0);
	}

	TEST(Type_Containers, Tuple)
	{
		NxEn::Tuple<ContainerTest, ContainerTest> Test = NxEn::Tuple(ContainerTest(5), ContainerTest(10));

		ContainerTest& Test1 = Test.GetFirst();
		ContainerTest& Test2 = Test.GetSecond();
		ASSERT_EQ(Test1.Integer, 5);
		ASSERT_EQ(Test2.Integer, 10);

		Test.SetFirst(ContainerTest(20));
		Test.SetSecond(ContainerTest(25));

		ASSERT_EQ(Test1.Integer, 20);
		ASSERT_EQ(Test2.Integer, 25);

		Test.Swap();
		ASSERT_EQ(Test.GetFirst().Integer, 25);
		ASSERT_EQ(Test.GetSecond().Integer, 20);
	}

	TEST(Type_Containers, NativeType)
	{
		NxEn::List<uint64> TestUint;
		TestUint.Append(18);
		TestUint.Append(24);
		TestUint.Append(36);
		TestUint.Append(45);
		TestUint.Append(58);
		TestUint.Append(64);
		TestUint.Append(72);
		ASSERT_EQ(TestUint.GetCount(), 7);

		ContainerTest Container;
		NxEn::List<ContainerTest*> TestPointer;
		TestPointer.Append(&Container);
		TestPointer.Append(&Container);
		TestPointer.Append(&Container);
		TestPointer.Append(&Container);
		TestPointer.Append(&Container);
		ASSERT_EQ(TestPointer.GetCount(), 5);

		ContainerTest TestPointer21(1);
		ContainerTest TestPointer22(2);
		ContainerTest TestPointer23(3);
		ContainerTest TestPointer24(4);

		NxEn::Dictionary<ContainerTest*, ContainerTest> TestPointer2;
		TestPointer2.Append(&TestPointer21, TestPointer21);
		TestPointer2.Append(&TestPointer22, TestPointer22);
		TestPointer2.Append(&TestPointer23, TestPointer23);
		TestPointer2.Append(&TestPointer24, TestPointer24);
		ASSERT_EQ(TestPointer2.GetCount(), 4);

		ContainerTest TestPointer30(10);
		ContainerTest* TestPointer31 = &TestPointer30;
		ContainerTest* TestPointer32 = &TestPointer30;
		ContainerTest* TestPointer33 = &TestPointer30;
		ContainerTest* TestPointer34 = &TestPointer30;

		NxEn::Dictionary<ContainerTest*, ContainerTest> TestPointer3;
		TestPointer3.Append(TestPointer31, TestPointer30);
		TestPointer3.Append(TestPointer32, TestPointer30);
		TestPointer3.Append(TestPointer33, TestPointer30);
		TestPointer3.Append(TestPointer34, TestPointer30);
		ASSERT_EQ(TestPointer3.GetCount(), 1);
	}

	TEST(Type_Containers, Strings)
	{
		const char* Text = "Hello World";
		NxEn::String Data = Text;

		NxEn::Array<NxEn::String> Array = NxEn::Array<NxEn::String>(10);
		Array.Assign(0, Data);
		Array.Assign(0, Text);
		Array[0] = Data;
		Array[0] = Text;

		NxEn::List<NxEn::String> List = NxEn::List<NxEn::String>();
		List.Append(Data);
		List.Append(Text);
		List.Insert(1, Data);
		List.Insert(1, Text);
		List[1] = Data;
		List[1] = Text;
		List.Remove(1);
		List.Remove(1);

		NxEn::Dequeue<NxEn::String> Dequeue = NxEn::Dequeue<NxEn::String>();
		Dequeue.AppendFront(Data);
		Dequeue.AppendBack(Text);
		List[1] = Data;
		List[1] = Text;
		Dequeue.RemoveFront();
		Dequeue.RemoveBack();

		NxEn::LinkedList<NxEn::String> LinkedList = NxEn::LinkedList<NxEn::String>();
		LinkedList.AppendFront(Data);
		LinkedList.AppendBack(Text);
		LinkedList.RemoveFront();
		LinkedList.RemoveBack();

		NxEn::Stack<NxEn::String> Stack = NxEn::Stack<NxEn::String>();
		Stack.Append(Data);
		Stack.Append(Text);
		Stack.Remove();
		Stack.Remove();

		NxEn::Queue<NxEn::String> Queue = NxEn::Queue<NxEn::String>();
		Queue.Append(Data);
		Queue.Append(Text);
		Queue.Remove();
		Queue.Remove();

		NxEn::Set<NxEn::String> Set = NxEn::Set<NxEn::String>();
		Set.Append(Data);
		Set.Append(Text);
		Set.Remove(Data);

		NxEn::Dictionary<NxEn::String, NxEn::String> Dictionary = NxEn::Dictionary<NxEn::String, NxEn::String>();
		Dictionary.Append("Key 1", Data);
		Dictionary.Append("Key 2", Text);
		Dictionary.AppendConstruct("Key 3", "Hello World");
		Dictionary.Remove("Key 1");

		NxEn::Tree<NxEn::String> Tree = NxEn::Tree<NxEn::String>();
		NxEn::String& Root = Tree.Append(nullptr, Data);
		Tree.Append(&Root, Text);
		Tree.Remove(&Root);

		NxEn::Graph<NxEn::String> Graph = NxEn::Graph<NxEn::String>();
		NxEn::String& A = Graph.Append(Data);
		NxEn::String& B = Graph.Append(Text);
		Graph.Connect(&A, &B);
		Graph.Disconnect(&A, &B);
		Graph.Remove(&A);
		Graph.Remove(&B);

		NxEn::Pool<NxEn::String> Pool = NxEn::Pool<NxEn::String>();
		NxEn::String& C = Pool.Acquire();
		Pool.Recycle(C);

		NxEn::Tuple<NxEn::String, NxEn::String> Tuple = NxEn::Tuple<NxEn::String, NxEn::String>(Data, Text);
		Tuple.SetFirst(Tuple.GetSecond());
	}

	TEST(Type_Containers, Range)
	{
		NxEn::Array<ContainerTest> Container1(10);
		Container1.Assign(0, 18);
		Container1.Assign(1, 24);
		Container1.Assign(2, 36);
		Container1.Assign(3, 45);
		Container1.Assign(4, 58);

		NxEn::List<ContainerTest> Container2;
		Container2.Append(36);
		Container2.Append(58);
		Container2.Append(45);
		Container2.Append(24);
		Container2.Append(18);

		NxEn::Dequeue<ContainerTest> Container3;
		Container3.AppendBack(36);
		Container3.AppendBack(24);
		Container3.AppendBack(18);
		Container3.AppendBack(58);
		Container3.AppendBack(45);

		NxEn::LinkedList<ContainerTest> Container4;
		ContainerTest& Ref4 = Container4.AppendBack(36);
		Container4.AppendBack(24);
		Container4.AppendBack(18);
		Container4.AppendBack(58);
		Container4.AppendBack(45);

		NxEn::Stack<ContainerTest> Container5;
		Container5.Append(36);
		Container5.Append(24);
		Container5.Append(18);
		Container5.Append(58);
		Container5.Append(45);

		NxEn::Queue<ContainerTest> Container6;
		Container6.Append(36);
		Container6.Append(24);
		Container6.Append(18);
		Container6.Append(58);
		Container6.Append(45);

		NxEn::Tree<ContainerTest> Container7;
		ContainerTest& Root = Container7.Append(nullptr, 36);
		Container7.Append(&Root, 24);
		Container7.Append(&Root, 18);
		Container7.Append(&Root, 58);
		Container7.Append(&Root, 45);

		NxEn::Graph<ContainerTest> Container8;
		Container8.Append(36);
		Container8.Append(24);
		Container8.Append(18);
		Container8.Append(58);
		Container8.Append(45);

		Container1.AssignRange(0, Container8);
		Container2.AssignRange(0, Container6);
		Container3.AssignRange(0, Container4);
		Container4.InsertBackRange(&Ref4, Container3);
		Container5.AppendRange(Container1);
		Container6.AppendRange(Container5);
		Container7.AppendRange(&Root, Container2);
		Container8.AppendRange(Container6);

		NxEn::Set<ContainerTest> Container9;
		Container9.AppendRange(Container2);
		Container2.AppendRange(Container9);

		NxEn::List<NxEn::KeyValuePair<ContainerTest, ContainerTest>> Pair;
		Pair.Append({ ContainerTest(1), ContainerTest(10) });
		Pair.Append({ ContainerTest(2), ContainerTest(20) });

		NxEn::Dictionary<ContainerTest, ContainerTest> Container10;
		Container10.AppendRange(Pair);
		Pair.AppendRange(Container10);
	}

	TEST(Type_Containers, Container)
	{
		NxEn::List<ContainerTest> Test;
		Test.Append(1);
		Test.Append(2);
		Test.Append(3);
		Test.Append(4);
		Test.Append(5);

		NxEn::Array<NxEn::List<ContainerTest>> Array = NxEn::Array<NxEn::List<ContainerTest>>(5);
		Array.Assign(0, Test);

		NxEn::List<NxEn::List<ContainerTest>> List;
		List.Append(Test);
		Test.Append(1);
		Test.Append(2);
		Test.Append(3);
		Test.Append(4);
		Test.Append(5);
		List.Assign(0, Test);

		NxEn::LinkedList<NxEn::List<ContainerTest>> LinkedList;
		LinkedList.AppendBack(Test);

		NxEn::Dictionary<ContainerTest, NxEn::List<ContainerTest>> Dictionary;
		Dictionary.Append(ContainerTest(10), Test);
		Test.Append(1);
		Test.Append(2);
		Test.Append(3);
		Test.Append(4);
		Test.Append(5);
		Dictionary.Assign(ContainerTest(10), Test);

		ASSERT_EQ(Test[2].Integer, Array[0][2].Integer);
		ASSERT_EQ(Test[2].Integer, List[0][2].Integer);
		ASSERT_EQ(Test[2].Integer, LinkedList.Get()[2].Integer);
		ASSERT_EQ(Test[2].Integer, Dictionary[ContainerTest(10)][2].Integer);

		Test[2].Integer = 10;

		ASSERT_NE(Test[2].Integer, Array[0][2].Integer);
		ASSERT_NE(Test[2].Integer, List[0][2].Integer);
		ASSERT_NE(Test[2].Integer, LinkedList.Get()[2].Integer);
		ASSERT_NE(Test[2].Integer, Dictionary[ContainerTest(10)][2].Integer);
	}
}
