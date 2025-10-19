#include "NexusTests/Core/NexusTests.h"

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
			NEXUS_ASSERT(!Destroyed, Default, "Already Destroyed");

			Integer = 0;
			Float = 0.0f;
			Boolean = false;
			Destroyed = true;
		}

		ContainerTest& operator=(const ContainerTest& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			Integer = Other.Integer;
			Float = Other.Float;
			Boolean = Other.Boolean;
			Destroyed = Other.Destroyed;
			return *this;
		}

		ContainerTest& operator=(ContainerTest&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

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

namespace NxFr
{
	namespace Hashing
	{
		template<typename H>
		class HashProcess<NxTs::ContainerTest, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const NxTs::ContainerTest& Data)
			{
				State.Accumulate(&Data.Integer, sizeof(uint64))
					.Accumulate(&Data.Float, sizeof(float))
					.Accumulate(&Data.Boolean, sizeof(bool));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const NxTs::ContainerTest& Data)
			{
				HashProcess<NxTs::ContainerTest, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};
	}

	template<>
	struct StringConverter<NxTs::ContainerTest>
	{
		static void ToString(const NxTs::ContainerTest& Data, String& Result, StringView Format = "")
		{
			Result.Format("Container: %d %.2f %s", Data.Integer, Data.Float, Data.Boolean ? "true" : "false");
		}
	};
}

namespace NxTs
{
	TEST(Containers, Array)
	{
		NxFr::Array<ContainerTest, 10> Test = NxFr::Array<ContainerTest, 10>();
		NxFr::ContainersUtils::Fill<ContainerTest>(Test, 5);
		ASSERT_EQ(Test.GetCount(), 10);

		ContainerTest Container1 = ContainerTest(1);
		NxFr::Array<ContainerTest> Range = NxFr::Array<ContainerTest>(5);
		NxFr::ContainersUtils::Fill<ContainerTest>(Range, 4);

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

		NxFr::Array<ContainerTest, 10> Copy = Test;
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

		Test.Sort<NxFr::Sorting::MergeSort>();
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

	TEST(Containers, List)
	{
		NxFr::List<ContainerTest> Test = NxFr::List<ContainerTest>(10);
		ASSERT_EQ(Test.GetCapacity(), 10);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxFr::List<ContainerTest> Range = NxFr::List<ContainerTest>();
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

		NxFr::List<ContainerTest> Copy = Test;
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
		Test.Reserve(30);
		ASSERT_EQ(Test.GetCapacity(), 30);
		Test.Reserve(0);
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

		Test.Sort<NxFr::Sorting::QuickSort>();
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

	TEST(Containers, Dequeue)
	{
		NxFr::Dequeue<ContainerTest> Test = NxFr::Dequeue<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxFr::Dequeue<ContainerTest> Range = NxFr::Dequeue<ContainerTest>();
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

		NxFr::Dequeue<ContainerTest> Copy = Test;
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

		Test.Sort<NxFr::Sorting::HeapSort>();
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

	TEST(Containers, Stack)
	{
		NxFr::Stack<ContainerTest> Test = NxFr::Stack<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxFr::Stack<ContainerTest> Range = NxFr::Stack<ContainerTest>();
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

		NxFr::Stack<ContainerTest> Copy = Test;
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

	TEST(Containers, Queue)
	{
		NxFr::Queue<ContainerTest> Test = NxFr::Queue<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxFr::Queue<ContainerTest> Range = NxFr::Queue<ContainerTest>();
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

		NxFr::Queue<ContainerTest> Copy = Test;
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

	TEST(Containers, LinkedList)
	{
		NxFr::LinkedList<ContainerTest> Test = NxFr::LinkedList<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxFr::LinkedList<ContainerTest> Range = NxFr::LinkedList<ContainerTest>();
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

		NxFr::LinkedList<ContainerTest> Copy = Test;
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

		Test.Swap(&Test.First(), &Test.Last());
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

	TEST(Containers, Set)
	{
		NxFr::Set<ContainerTest> Test = NxFr::Set<ContainerTest>();
		ASSERT_EQ(Test.GetCapacity(), 11);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxFr::Set<ContainerTest> Range = NxFr::Set<ContainerTest>();
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

		NxFr::Set<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.GetIterator(6)->Integer, 6);

		Test.Reserve(21);
		ASSERT_EQ(Test.GetCapacity(), 23);

		ContainerTest ToFind1 = ContainerTest(6);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.Contains(ToFind1), true);
		ASSERT_EQ(Test.Contains(ToFind2), false);
		ASSERT_EQ(Test.Find(ToFind1)->Integer, ToFind1.Integer);
		ASSERT_EQ(Test.Find(ToFind2), Test.End());
	}

	TEST(Containers, Dictionary)
	{
		NxFr::Dictionary<ContainerTest, ContainerTest> Test = NxFr::Dictionary<ContainerTest, ContainerTest>();
		ASSERT_EQ(Test.GetCapacity(), 11);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxFr::Dictionary<ContainerTest, ContainerTest> Range = NxFr::Dictionary<ContainerTest, ContainerTest>();
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

		NxFr::Dictionary<ContainerTest, ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get(9).Integer, 90);
		ASSERT_EQ(Test.GetIterator(6)->Key.Integer, 6);

		for (auto& [Key, Value] : Test)
		{
			Value.Integer = Key.Integer * 10;
		}

		Test.Reserve(21);
		ASSERT_EQ(Test.GetCapacity(), 23);

		Test.Swap(6, 7);
		ASSERT_EQ(Test[6].Integer, 70);
		ASSERT_EQ(Test[7].Integer, 60);

		ContainerTest ToFind1 = ContainerTest(3);
		ContainerTest ToFind2 = ContainerTest(100);
		ASSERT_EQ(Test.ContainsKey(ToFind1), true);
		ASSERT_EQ(Test.ContainsValue(ToFind2), true);
		ASSERT_EQ(Test.FindKey(ToFind1)->Key.Integer, ToFind1.Integer);
		ASSERT_EQ(Test.FindValue(ToFind2)->Value.Integer, ToFind2.Integer);

		NxFr::ContainersUtils::Fill<ContainerTest, ContainerTest>(Range, 4);
	}

	TEST(Containers, Tree)
	{
		NxFr::Tree<ContainerTest> Test = NxFr::Tree<ContainerTest>();
		ContainerTest& Root = Test.Append(nullptr, 0);
		ASSERT_EQ(Test.GetCount(), 1);
		ASSERT_EQ(Test.IsEmpty(), false);

		ContainerTest Container1 = ContainerTest(1);
		NxFr::Tree<ContainerTest> Range = NxFr::Tree<ContainerTest>();
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

		NxFr::Tree<ContainerTest> Copy = Test;
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

	TEST(Containers, Graph)
	{
		NxFr::Graph<ContainerTest> Test = NxFr::Graph<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest Container1 = ContainerTest(1);
		NxFr::Graph<ContainerTest> Range = NxFr::Graph<ContainerTest>();
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

		NxFr::Graph<ContainerTest> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		Test.Connect(&Index1, &Index2);
		Test.Connect(&Index1, &Index3);
		Test.Connect(&Index2, &Index3);
		ASSERT_EQ(Test.GetConnection(&Index1, NxFr::Graph<ContainerTest>::CT::To, 0).Integer, 2);
		ASSERT_EQ(Test.GetConnection(&Index1, NxFr::Graph<ContainerTest>::CT::To, 1).Integer, 3);
		ASSERT_EQ(Test.GetConnection(&Index2, NxFr::Graph<ContainerTest>::CT::To, 0).Integer, 3);
		ASSERT_EQ(Test.IsConnected(&Index1, &Index2, NxFr::Graph<ContainerTest>::CT::To), true);
		ASSERT_EQ(Test.IsConnected(&Index3, &Index1, NxFr::Graph<ContainerTest>::CT::From), true);

		Test.Disconnect(&Index1, &Index3);
		ASSERT_EQ(Test.IsConnected(&Index3, &Index1, NxFr::Graph<ContainerTest>::CT::From), false);

		ASSERT_EQ(Test.GetIterator(&Index1)->Integer, Index1.Integer);
		ASSERT_EQ(Test.GetIterator(&Index1).Connections(NxFr::Graph<ContainerTest>::CT::To, 0)->Integer, Index2.Integer);
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

	TEST(Containers, Collection)
	{
		NxFr::List<ContainerTest> Data1 = NxFr::List<ContainerTest>(5);
		Data1.Append(1);
		Data1.Append(2);
		Data1.Append(3);
		Data1.Append(4);
		Data1.Append(5);

		NxFr::Queue<ContainerTest> Data2 = NxFr::Queue<ContainerTest>();
		Data2.Append(1);
		Data2.Append(2);
		Data2.Append(3);
		Data2.Append(4);
		Data2.Append(5);

		uint64 Index = 0;
		NxFr::Collection<ContainerTest> Test = Data2;
		for (auto& It = Test.Current(); It != Test.End(); ++It)
		{
			ASSERT_EQ(It->Integer, ++Index);
		}

		auto& It = Test.Reset();
		ASSERT_EQ((*It).Integer, 1);
	}

	TEST(Containers, ContainersUtils)
	{
		NxFr::Set<ContainerTest> Base = NxFr::Set<ContainerTest>();
		Base.Append(1);
		Base.Append(3);
		Base.Append(5);
		Base.Append(7);
		Base.Append(9);

		NxFr::Set<ContainerTest> Other = NxFr::Set<ContainerTest>();
		Other.Append(2);
		Other.Append(4);
		Other.Append(6);
		Other.Append(8);
		Other.Append(10);

		NxFr::ContainersUtils::SetUnion(Base, Other);
		ASSERT_EQ(Base.GetCount(), 10);

		NxFr::ContainersUtils::SetDifference(Base, Other);
		ASSERT_EQ(Base.GetCount(), 5);

		auto It = Other.Begin();
		Base.Append(*It++);
		Base.Append(*It++);
		Base.Append(*It++);

		NxFr::ContainersUtils::SetIntersection(Base, Other);
		ASSERT_EQ(Base.GetCount(), 3);
	}

	TEST(Containers, Buffer)
	{
		ContainerTest Raw[5];
		Raw[0].Integer = 0;
		Raw[1].Integer = 1;
		Raw[2].Integer = 2;
		Raw[3].Integer = 3;
		Raw[4].Integer = 4;

		NxFr::BufferView View = NxFr::BufferView(Raw, 5 * sizeof(ContainerTest));
		ASSERT_EQ(View.GetPtr<ContainerTest>(0 * sizeof(ContainerTest))->Integer, 0);
		ASSERT_EQ(View.GetPtr<ContainerTest>(1 * sizeof(ContainerTest))->Integer, 1);
		ASSERT_EQ(View.GetPtr<ContainerTest>(2 * sizeof(ContainerTest))->Integer, 2);
		ASSERT_EQ(View.GetPtr<ContainerTest>(3 * sizeof(ContainerTest))->Integer, 3);
		ASSERT_EQ(View.GetPtr<ContainerTest>(4 * sizeof(ContainerTest))->Integer, 4);

		NxFr::Buffer Buffer(3 * sizeof(ContainerTest));
		Buffer.Set(&Raw[0], sizeof(ContainerTest), 0 * sizeof(ContainerTest));
		Buffer.Set(&Raw[2], sizeof(ContainerTest), 2 * sizeof(ContainerTest));
		Buffer.Set(&Raw[1], sizeof(ContainerTest), 1 * sizeof(ContainerTest));
		ASSERT_EQ(View.GetPtr<ContainerTest>(0 * sizeof(ContainerTest))->Integer, 0);
		ASSERT_EQ(View.GetPtr<ContainerTest>(1 * sizeof(ContainerTest))->Integer, 1);
		ASSERT_EQ(View.GetPtr<ContainerTest>(2 * sizeof(ContainerTest))->Integer, 2);

		Buffer.Resize(5 * sizeof(ContainerTest));
		Buffer.Set(&Raw[4], sizeof(ContainerTest), 4 * sizeof(ContainerTest));
		Buffer.Set(&Raw[3], sizeof(ContainerTest), 3 * sizeof(ContainerTest));
		ASSERT_EQ(View.GetPtr<ContainerTest>(3 * sizeof(ContainerTest))->Integer, 3);
		ASSERT_EQ(View.GetPtr<ContainerTest>(4 * sizeof(ContainerTest))->Integer, 4);
	}

	TEST(Containers, Pool)
	{
		NxFr::Pool<ContainerTest, NxFr::Pooling::PreAllocated<ContainerTest>> TestPreAllocated = NxFr::Pool<ContainerTest, NxFr::Pooling::PreAllocated<ContainerTest>>(10);
		ASSERT_EQ(TestPreAllocated.GetCapacity(), 10);
		ASSERT_EQ(TestPreAllocated.GetCount(), 0);
		ASSERT_EQ(TestPreAllocated.GetUnused(), 10);

		ContainerTest& Test1 = TestPreAllocated.Acquire();
		ASSERT_EQ(TestPreAllocated.GetCount(), 1);
		ASSERT_EQ(TestPreAllocated.GetUnused(), 9);

		ContainerTest& Test2 = TestPreAllocated.Acquire();
		ASSERT_EQ(TestPreAllocated.GetCount(), 2);
		ASSERT_EQ(TestPreAllocated.GetUnused(), 8);

		Test1.Integer = 10;

		TestPreAllocated.Recycle(Test1);
		ASSERT_EQ(TestPreAllocated.GetCount(), 1);
		ASSERT_EQ(TestPreAllocated.GetUnused(), 9);

		ContainerTest& Test3 = TestPreAllocated.Acquire();
		ContainerTest& Test4 = TestPreAllocated.Acquire();
		ContainerTest& Test5 = TestPreAllocated.Acquire();

		Test4.Integer = 5;
		auto ItPreAllocated = TestPreAllocated.Find(5);
		ASSERT_EQ(ItPreAllocated->Integer, 5);

		TestPreAllocated.Recycle(Test4);
		TestPreAllocated.Recycle(Test3);
		TestPreAllocated.Recycle(Test5);

		ContainerTest& Test6 = TestPreAllocated.Acquire();
		ContainerTest& Test7 = TestPreAllocated.Acquire();
		ContainerTest& Test8 = TestPreAllocated.Acquire();

		TestPreAllocated.Recycle(Test8);

		TestPreAllocated.Clear();
		ASSERT_EQ(TestPreAllocated.GetCount(), 0);
		ASSERT_EQ(TestPreAllocated.GetUnused(), 10);

		NxFr::Pool<ContainerTest> TestOnDemand = NxFr::Pool<ContainerTest>();
		ASSERT_EQ(TestOnDemand.GetCapacity(), 0);
		ASSERT_EQ(TestOnDemand.GetCount(), 0);
		ASSERT_EQ(TestOnDemand.GetUnused(), 0);

		ContainerTest& Test9 = TestOnDemand.Acquire();
		ASSERT_EQ(TestOnDemand.GetCount(),1);
		ASSERT_EQ(TestOnDemand.GetUnused(), 0);

		ContainerTest& Test10 = TestOnDemand.Acquire();
		ASSERT_EQ(TestOnDemand.GetCount(), 2);
		ASSERT_EQ(TestOnDemand.GetUnused(), 0);

		Test9.Integer = 10;

		TestOnDemand.Recycle(Test9);
		ASSERT_EQ(TestOnDemand.GetCount(), 1);
		ASSERT_EQ(TestOnDemand.GetUnused(), 1);

		ContainerTest& Test11 = TestOnDemand.Acquire();
		ContainerTest& Test12 = TestOnDemand.Acquire();
		ContainerTest& Test13 = TestOnDemand.Acquire();

		Test12.Integer = 5;
		auto ItOnDemand = TestOnDemand.Find(5);
		ASSERT_EQ(ItOnDemand->Integer, 5);

		TestOnDemand.Recycle(Test11);
		TestOnDemand.Recycle(Test12);
		TestOnDemand.Recycle(Test13);

		ContainerTest& Test14 = TestOnDemand.Acquire();
		ContainerTest& Test15 = TestOnDemand.Acquire();
		ContainerTest& Test16 = TestOnDemand.Acquire();

		TestOnDemand.Recycle(Test16);

		TestOnDemand.Clear();
		ASSERT_EQ(TestOnDemand.GetCount(), 0);
		ASSERT_EQ(TestOnDemand.GetUnused(), 4);
	}

	TEST(Containers, Tuple)
	{
		NxFr::Tuple<ContainerTest, ContainerTest> Test = NxFr::Tuple(ContainerTest(5), ContainerTest(10));

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

	TEST(Containers, NativeType)
	{
		NxFr::List<uint64> TestUint;
		TestUint.Append(18);
		TestUint.Append(24);
		TestUint.Append(36);
		TestUint.Append(45);
		TestUint.Append(58);
		TestUint.Append(64);
		TestUint.Append(72);
		ASSERT_EQ(TestUint.GetCount(), 7);

		ContainerTest Container;
		NxFr::List<ContainerTest*> TestPointer;
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

		NxFr::Dictionary<ContainerTest*, ContainerTest> TestPointer2;
		TestPointer2.Append(&TestPointer21, TestPointer21);
		TestPointer2.Append(&TestPointer22, TestPointer22);
		TestPointer2.Append(&TestPointer23, TestPointer23);
		TestPointer2.Append(&TestPointer24, TestPointer24);
		TestPointer2.Append(nullptr, TestPointer24);
		ASSERT_EQ(TestPointer2.GetCount(), 5);

		NxFr::Dictionary<int32, ContainerTest> TestPointer3;
		TestPointer3.Append(1, TestPointer21);
		TestPointer3.Append(2, TestPointer22);
		TestPointer3.Append(3, TestPointer23);
		TestPointer3.Append(4, TestPointer24);
		TestPointer3.Append(0, TestPointer24);
	}

	TEST(Containers, Strings)
	{
		const char* Text = "Hello World";
		NxFr::String Data = Text;

		NxFr::Array<NxFr::String> Array = NxFr::Array<NxFr::String>(10);
		Array.Assign(0, Data);
		Array.Assign(0, Text);
		Array[0] = Data;
		Array[0] = Text;

		NxFr::List<NxFr::String> List = NxFr::List<NxFr::String>();
		List.Append(Data);
		List.Append(Text);
		List.Insert(1, Data);
		List.Insert(1, Text);
		List[1] = Data;
		List[1] = Text;
		List.Remove(1);
		List.Remove(1);

		NxFr::Dequeue<NxFr::String> Dequeue = NxFr::Dequeue<NxFr::String>();
		Dequeue.AppendFront(Data);
		Dequeue.AppendBack(Text);
		List[1] = Data;
		List[1] = Text;
		Dequeue.RemoveFront();
		Dequeue.RemoveBack();

		NxFr::LinkedList<NxFr::String> LinkedList = NxFr::LinkedList<NxFr::String>();
		LinkedList.AppendFront(Data);
		LinkedList.AppendBack(Text);
		LinkedList.RemoveFront();
		LinkedList.RemoveBack();

		NxFr::Stack<NxFr::String> Stack = NxFr::Stack<NxFr::String>();
		Stack.Append(Data);
		Stack.Append(Text);
		Stack.Remove();
		Stack.Remove();

		NxFr::Queue<NxFr::String> Queue = NxFr::Queue<NxFr::String>();
		Queue.Append(Data);
		Queue.Append(Text);
		Queue.Remove();
		Queue.Remove();

		NxFr::Set<NxFr::String> Set = NxFr::Set<NxFr::String>();
		Set.Append(Data);
		Set.Append(Text);
		Set.Remove(Data);

		NxFr::Dictionary<NxFr::String, NxFr::String> Dictionary = NxFr::Dictionary<NxFr::String, NxFr::String>();
		Dictionary.Append("Key 1", Data);
		Dictionary.Append("Key 2", Text);
		Dictionary.AppendConstruct("Key 3", "Hello World");
		Dictionary.Remove("Key 1");

		NxFr::Tree<NxFr::String> Tree = NxFr::Tree<NxFr::String>();
		NxFr::String& Root = Tree.Append(nullptr, Data);
		Tree.Append(&Root, Text);
		Tree.Remove(&Root);

		NxFr::Graph<NxFr::String> Graph = NxFr::Graph<NxFr::String>();
		NxFr::String& A = Graph.Append(Data);
		NxFr::String& B = Graph.Append(Text);
		Graph.Connect(&A, &B);
		Graph.Disconnect(&A, &B);
		Graph.Remove(&A);
		Graph.Remove(&B);

		//NxFr::Pool<NxFr::String> Pool = NxFr::Pool<NxFr::String>();
		//NxFr::String& C = Pool.Acquire();
		//Pool.Recycle(C);

		NxFr::Tuple<NxFr::String, NxFr::String> Tuple = NxFr::Tuple<NxFr::String, NxFr::String>(Data, Text);
		Tuple.SetFirst(Tuple.GetSecond());
	}

	TEST(Containers, Range)
	{
		NxFr::Array<ContainerTest> Container1(10);
		Container1.Assign(0, 18);
		Container1.Assign(1, 24);
		Container1.Assign(2, 36);
		Container1.Assign(3, 45);
		Container1.Assign(4, 58);

		NxFr::List<ContainerTest> Container2;
		Container2.Append(36);
		Container2.Append(58);
		Container2.Append(45);
		Container2.Append(24);
		Container2.Append(18);

		NxFr::Dequeue<ContainerTest> Container3;
		Container3.AppendBack(36);
		Container3.AppendBack(24);
		Container3.AppendBack(18);
		Container3.AppendBack(58);
		Container3.AppendBack(45);

		NxFr::LinkedList<ContainerTest> Container4;
		ContainerTest& Ref4 = Container4.AppendBack(36);
		Container4.AppendBack(24);
		Container4.AppendBack(18);
		Container4.AppendBack(58);
		Container4.AppendBack(45);

		NxFr::Stack<ContainerTest> Container5;
		Container5.Append(36);
		Container5.Append(24);
		Container5.Append(18);
		Container5.Append(58);
		Container5.Append(45);

		NxFr::Queue<ContainerTest> Container6;
		Container6.Append(36);
		Container6.Append(24);
		Container6.Append(18);
		Container6.Append(58);
		Container6.Append(45);

		NxFr::Tree<ContainerTest> Container7;
		ContainerTest& Root = Container7.Append(nullptr, 36);
		Container7.Append(&Root, 24);
		Container7.Append(&Root, 18);
		Container7.Append(&Root, 58);
		Container7.Append(&Root, 45);

		NxFr::Graph<ContainerTest> Container8;
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

		NxFr::Set<ContainerTest> Container9;
		Container9.AppendRange(Container2);
		Container2.AppendRange(Container9);

		NxFr::List<NxFr::KeyValuePair<const ContainerTest, ContainerTest>> Pair;
		Pair.Append({ ContainerTest(1), ContainerTest(10) });
		Pair.Append({ ContainerTest(2), ContainerTest(20) });

		NxFr::Dictionary<ContainerTest, ContainerTest> Container10;
		Container10.AppendRange(Pair);
		Pair.AppendRange(Container10);
	}

	TEST(Containers, Container)
	{
		NxFr::List<ContainerTest> Test;
		Test.Append(1);
		Test.Append(2);
		Test.Append(3);
		Test.Append(4);
		Test.Append(5);

		NxFr::Array<NxFr::List<ContainerTest>> Array = NxFr::Array<NxFr::List<ContainerTest>>(5);
		Array.Assign(0, Test);

		NxFr::List<NxFr::List<ContainerTest>> List;
		List.Append(Test);
		Test.Append(1);
		Test.Append(2);
		Test.Append(3);
		Test.Append(4);
		Test.Append(5);
		List.Assign(0, Test);

		NxFr::LinkedList<NxFr::List<ContainerTest>> LinkedList;
		LinkedList.AppendBack(Test);

		NxFr::Dictionary<ContainerTest, NxFr::List<ContainerTest>> Dictionary;
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

	TEST(Containers, InitializeList)
	{
		NxFr::Array<uint64> Array = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
		ASSERT_EQ(Array.GetCount(), 10);
		ASSERT_EQ(Array[4], 5);

		NxFr::List<uint64> List = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
		ASSERT_EQ(List.GetCount(), 10);
		ASSERT_EQ(List[4], 5);

		NxFr::LinkedList<uint64> LinkedList = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
		ASSERT_EQ(LinkedList.GetCount(), 10);

		NxFr::Dequeue<uint64> Dequeue = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
		ASSERT_EQ(Dequeue.GetCount(), 10);

		NxFr::Set<uint64> Set = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
		ASSERT_EQ(Set.GetCount(), 10);
		ASSERT_EQ(Set.Contains(4), true);

		NxFr::Dictionary<uint64, uint64> Dictionary = { { 9, 9 }, { 8, 8 }, { 7, 7 }, { 6, 6 }, { 5, 5 }, { 4, 4 }, { 3, 3 }, { 2, 2 }, { 1, 1 }, { 0, 0 } };
		ASSERT_EQ(Dictionary.GetCount(), 10);
		ASSERT_EQ(Dictionary[4], 4);
	}

	TEST(Containers, ToString)
	{
		NxFr::Array<ContainerTest> Test1(5);
		Test1[0] = 1;
		Test1[1] = 2;
		Test1[2] = 3;
		Test1[3] = 4;
		Test1[4] = 5;
		NxFr::String String1 = NxFr::StringUtility::ToString(Test1);

		NxFr::List<ContainerTest> Test2(5);
		Test2.Append(1);
		Test2.Append(2);
		Test2.Append(3);
		Test2.Append(4);
		Test2.Append(5);
		NxFr::String String2 = NxFr::StringUtility::ToString(Test2);

		NxFr::Dequeue<ContainerTest> Test3;
		Test3.AppendBack(1);
		Test3.AppendBack(2);
		Test3.AppendBack(3);
		Test3.AppendBack(4);
		Test3.AppendBack(5);
		NxFr::String String3 = NxFr::StringUtility::ToString(Test3);

		NxFr::LinkedList<ContainerTest> Test4;
		Test4.AppendBack(1);
		Test4.AppendBack(2);
		Test4.AppendBack(3);
		Test4.AppendBack(4);
		Test4.AppendBack(5);
		NxFr::String String4 = NxFr::StringUtility::ToString(Test4);

		NxFr::Set<ContainerTest> Test5;
		Test5.Append(1);
		Test5.Append(2);
		Test5.Append(3);
		Test5.Append(4);
		Test5.Append(5);
		NxFr::String String5 = NxFr::StringUtility::ToString(Test5);

		NxFr::Dictionary<ContainerTest, ContainerTest> Test6;
		Test6.Append(1, 50);
		Test6.Append(2, 40);
		Test6.Append(3, 30);
		Test6.Append(4, 20);
		Test6.Append(5, 10);
		NxFr::String String6 = NxFr::StringUtility::ToString(Test6);
	}
}
