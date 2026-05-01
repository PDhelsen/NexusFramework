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
		class Hasher<NxTs::ContainerTest, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const NxTs::ContainerTest& Data)
			{
				State.Accumulate(Data.Integer);
				State.Accumulate(Data.Float);
				State.Accumulate(Data.Boolean);
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
	TEST(Type_Containers, Array)
	{
		ContainerTest Container = ContainerTest(1);
		NxFr::Array<ContainerTest> Range = { 4, 4, 4, 4, 4 };

		NxFr::Array<ContainerTest, 10> Test = NxFr::Array<ContainerTest, 10>();
		ASSERT_EQ(Test.GetCount(), 10);

		Test.Assign(0, Container);
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
		ASSERT_EQ(Test.BeginReverse()->Integer, Test.Last().Integer);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.First().Integer, 0);
		ASSERT_EQ(Test.Last().Integer, Test.GetCount() - 1);

		ASSERT_EQ(Test.IsValidIndex(5), true);
		ASSERT_EQ(Test.IsValidIndex(Test.GetCount()), false);
	}

	TEST(Type_Containers, List)
	{
		ContainerTest Container = ContainerTest(1);
		NxFr::List<ContainerTest> Range = { 4, 4, 4, 4, 4 };

		NxFr::List<ContainerTest> Test = NxFr::List<ContainerTest>(10);
		ASSERT_EQ(Test.GetCapacity(), 10);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Test.Append(Container);
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

		Test.Assign(7, Container);
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

		Test.Insert(3, Container);
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
		ASSERT_EQ(Test.BeginReverse()->Integer, Test.Last().Integer);

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
	}

	TEST(Type_Containers, Dequeue)
	{
		ContainerTest Container = ContainerTest(1);
		NxFr::Dequeue<ContainerTest> Range = { 4, 4, 4, 4, 4 };

		NxFr::Dequeue<ContainerTest> Test = NxFr::Dequeue<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Test.AppendBack(Container);
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

		Test.AppendFront(Container);
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

		Test.Assign(3, Container);
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
		ASSERT_EQ(Test.BeginReverse()->Integer, Test.Last().Integer);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.First().Integer, 0);
		ASSERT_EQ(Test.Last().Integer, Test.GetCount() - 1);

		ASSERT_EQ(Test.IsValidIndex(5), true);
		ASSERT_EQ(Test.IsValidIndex(Test.GetCount()), false);
	}

	TEST(Type_Containers, Stack)
	{
		ContainerTest Container = ContainerTest(1);
		NxFr::Stack<ContainerTest> Range = { 4, 4, 4, 4, 4 };

		NxFr::Stack<ContainerTest> Test = NxFr::Stack<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest& Index1 = Test.Append(Container);
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

		ASSERT_EQ(Test.Begin()->Integer, 4);
		ASSERT_EQ(Test.BeginReverse()->Integer, 1);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.Begin()->Integer, 0);
	}

	TEST(Type_Containers, Queue)
	{
		ContainerTest Container = ContainerTest(1);
		NxFr::Queue<ContainerTest> Range = { 4, 4, 4, 4, 4 };

		NxFr::Queue<ContainerTest> Test = NxFr::Queue<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest& Index1 = Test.Append(Container);
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

		ASSERT_EQ(Test.Begin()->Integer, 3);
		ASSERT_EQ(Test.BeginReverse()->Integer, 4);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.Begin()->Integer, 0);
	}

	TEST(Type_Containers, LinkedList)
	{
		ContainerTest Container = ContainerTest(1);
		NxFr::LinkedList<ContainerTest> Range = { 4, 4, 4, 4, 4 };

		NxFr::LinkedList<ContainerTest> Test = NxFr::LinkedList<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest& Index1 = Test.AppendBack(Container);
		ContainerTest& Index2 = Test.AppendBack(ContainerTest(2));
		ContainerTest& Index3 = Test.AppendBackConstruct(3);
		ContainerTest& Index4 = Test.AppendBackRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 1);
		ASSERT_EQ(Index2.Integer, 2);
		ASSERT_EQ(Index3.Integer, 3);
		ASSERT_EQ(Index4.Integer, 4);

		ContainerTest& Index5 = Test.AppendFront(Container);
		ContainerTest& Index6 = Test.AppendFront(ContainerTest(2));
		ContainerTest& Index7 = Test.AppendFrontConstruct(3);
		ContainerTest& Index8 = Test.AppendFrontRange(Range);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Integer, 1);
		ASSERT_EQ(Index6.Integer, 2);
		ASSERT_EQ(Index7.Integer, 3);
		ASSERT_EQ(Index8.Integer, 4);

		Index5 = Test.Assign(&Index5, Container);
		Index6 = Test.Assign(&Index6, ContainerTest(2));
		Index7 = Test.AssignConstruct(&Index7, 3);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Integer, 1);
		ASSERT_EQ(Index6.Integer, 2);
		ASSERT_EQ(Index7.Integer, 3);

		ContainerTest& Index9 = Test.InsertFront(&Index1, Container);
		ContainerTest& Index10 = Test.InsertFront(&Index2, ContainerTest(2));
		ContainerTest& Index11 = Test.InsertFrontConstruct(&Index3, 3);
		ContainerTest& Index12 = Test.InsertFrontRange(&Index4, Range);
		ASSERT_EQ(Test.GetCount(), 24);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index9.Integer, 1);
		ASSERT_EQ(Index10.Integer, 2);
		ASSERT_EQ(Index11.Integer, 3);
		ASSERT_EQ(Index12.Integer, 4);

		ContainerTest& Index13 = Test.InsertBack(&Index1, Container);
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
		ASSERT_EQ(Test.BeginReverse()->Integer, Test.Last().Integer);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
		ASSERT_EQ(Test.First().Integer, 0);
		ASSERT_EQ(Test.Last().Integer, Test.GetCount() - 1);
	}

	TEST(Type_Containers, Set)
	{
		ContainerTest Container = ContainerTest(1);
		NxFr::Set<ContainerTest> Range = { 6, 7, 8, 9, 10 };

		NxFr::Set<ContainerTest> Test = NxFr::Set<ContainerTest>();
		ASSERT_EQ(Test.GetCapacity(), 11);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		const ContainerTest& Index1 = Test.Append(Container);
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
	}

	TEST(Type_Containers, Dictionary)
	{
		ContainerTest Container = ContainerTest(1);
		NxFr::Dictionary<ContainerTest, ContainerTest> Range = { { 6, 60 }, { 7, 70 }, { 8, 80 }, { 9, 90 }, { 10, 100 } };

		NxFr::Dictionary<ContainerTest, ContainerTest> Test = NxFr::Dictionary<ContainerTest, ContainerTest>();
		ASSERT_EQ(Test.GetCapacity(), 11);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		const ContainerTest& Index1 = Test.Append(Container, 10);
		const ContainerTest& Index2 = Test.Append(ContainerTest(2), 20);
		const ContainerTest& Index3 = Test.AppendConstruct(ContainerTest(3), 30);
		const ContainerTest& Index4 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 10);
		ASSERT_EQ(Index2.Integer, 20);
		ASSERT_EQ(Index3.Integer, 30);

		const ContainerTest& Index5 = Test.Assign(Container, 100);
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
	}

	TEST(Type_Containers, Tree)
	{
		ContainerTest Container = ContainerTest(1);
		NxFr::Tree<ContainerTest> Range = NxFr::Tree<ContainerTest>();
		ContainerTest& RangeRoot = Range.Append(nullptr, 0);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);

		NxFr::Tree<ContainerTest> Test = NxFr::Tree<ContainerTest>();
		ContainerTest& Root = Test.Append(nullptr, 0);
		ASSERT_EQ(Test.GetCount(), 1);
		ASSERT_EQ(Test.IsEmpty(), false);

		ContainerTest& Index1 = Test.Append(&Root, Container);
		ContainerTest& Index2 = Test.Append(&Root, ContainerTest(2));
		ContainerTest& Index3 = Test.AppendConstruct(&Root, 3);
		ContainerTest& Index4 = Test.AppendRange(&Root, Range);
		ASSERT_EQ(Test.GetCount(), 10);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 1);
		ASSERT_EQ(Index2.Integer, 2);
		ASSERT_EQ(Index3.Integer, 3);
		ASSERT_EQ(Index4.Integer, 0);

		Index1 = Test.Assign(&Index1, Container);
		Index2 = Test.Assign(&Index2, ContainerTest(2));
		Index3 = Test.AssignConstruct(&Index3, 3);
		ASSERT_EQ(Test.GetCount(), 10);
		ASSERT_EQ(Test.IsEmpty(), false);

		ContainerTest& Index5 = Test.InsertSibling(&Index1, Container);
		ContainerTest& Index6 = Test.InsertSibling(&Index2, ContainerTest(5));
		ContainerTest& Index7 = Test.InsertSibling(&Index3, 6);
		ASSERT_EQ(Test.GetCount(), 13);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Integer, 1);
		ASSERT_EQ(Index6.Integer, 5);
		ASSERT_EQ(Index7.Integer, 6);

		ContainerTest& Index8 = Test.InsertChild(&Index1, Container);
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
		ASSERT_EQ(Test.Begin()->Integer, Root.Integer);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
	}

	TEST(Type_Containers, Graph)
	{
		ContainerTest Container = ContainerTest(1);
		NxFr::Graph<ContainerTest> Range = NxFr::Graph<ContainerTest>();
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);

		NxFr::Graph<ContainerTest> Test = NxFr::Graph<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		ContainerTest& Index1 = Test.Append(Container);
		ContainerTest& Index2 = Test.Append(ContainerTest(2));
		ContainerTest& Index3 = Test.AppendConstruct(3);
		ContainerTest& Index4 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Integer, 1);
		ASSERT_EQ(Index2.Integer, 2);
		ASSERT_EQ(Index3.Integer, 3);
		ASSERT_EQ(Index4.Integer, 4);

		Index1 = Test.Assign(&Index1, Container);
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
		ASSERT_EQ(Test.Begin()->Integer, Test.Get().Integer);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Integer = Index++;
		}
	}

	TEST(Type_Containers, Buffer)
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

	TEST(Type_Containers, Pool)
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

	TEST(Type_Containers, Tuple)
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
	}

	TEST(Type_Containers, Collection)
	{
		NxFr::List<ContainerTest> Data = NxFr::List<ContainerTest>(5);
		Data.Append(1);
		Data.Append(2);
		Data.Append(3);
		Data.Append(4);
		Data.Append(5);

		uint64 Index = 0;
		NxFr::Collection<ContainerTest> Test = Data;
		for (auto It = Test.Begin(); It != Test.End(); ++It)
		{
			ASSERT_EQ(It->Integer, ++Index);
		}

		auto It = Test.Begin();
		ASSERT_EQ((*It).Integer, 1);
	}

	TEST(Type_Containers, ContainersUtils)
	{
		NxFr::Array<ContainerTest> Data = { 1, 3, 5, 7, 9 };
		NxFr::ContainersUtils::Swap<ContainerTest>(Data, 0, 1);
		ASSERT_EQ(Data[0].Integer, 3);
		ASSERT_EQ(Data[1].Integer, 1);
		NxFr::ContainersUtils::Sort<ContainerTest>(Data);
		for (uint64 Index = 1; Index < Data.GetCount(); Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}
		NxFr::ContainersUtils::Reverse<ContainerTest>(Data);
		for (uint64 Index = 1; Index < Data.GetCount(); Index++)
		{
			ASSERT_EQ(Data[Index - 1] > Data[Index], true);
		}
		ASSERT_EQ(NxFr::ContainersUtils::Any<ContainerTest>(Data, [](const ContainerTest& Element) { return Element.Integer == 5; }), true);
		ASSERT_EQ(NxFr::ContainersUtils::Contains<ContainerTest>(Data, 9), true);
		ASSERT_NE(NxFr::ContainersUtils::Where<ContainerTest>(Data, [](const ContainerTest& Element) { return Element.Integer == 5; }), Data.End());
		ASSERT_NE(NxFr::ContainersUtils::Find<ContainerTest>(Data, 9), Data.End());

		NxFr::ContainersUtils::Fill<ContainerTest>(Data, 5);
		ASSERT_EQ(Data.First().Integer, 5);
		ASSERT_EQ(Data.Last().Integer, 5);
		NxFr::ContainersUtils::Resize<ContainerTest>(Data, 10);
		ASSERT_EQ(Data.GetCount(), 10);

		NxFr::Set<ContainerTest> SetBase = { 1, 3, 5, 7, 9 };
		NxFr::Set<ContainerTest> SetOther = { 2, 4, 6, 8, 10 };
		NxFr::ContainersUtils::SetUnion(SetBase, SetOther);
		ASSERT_EQ(SetBase.GetCount(), 10);
		NxFr::ContainersUtils::SetDifference(SetBase, SetOther);
		ASSERT_EQ(SetBase.GetCount(), 5);
		auto It = SetOther.Begin();
		SetBase.Append(*It++);
		SetBase.Append(*It++);
		SetBase.Append(*It++);
		NxFr::ContainersUtils::SetIntersection(SetBase, SetOther);
		ASSERT_EQ(SetBase.GetCount(), 3);

		NxFr::Array<ContainerTest> ToArray = NxFr::ContainersUtils::ToArray<ContainerTest>(SetOther);
		ASSERT_EQ(SetOther.GetCount(), 5);
	}

	TEST(Type_Containers, Misc)
	{
		NxFr::Array<ContainerTest> SrtringArray(5);
		SrtringArray[0] = 1;
		SrtringArray[1] = 2;
		SrtringArray[2] = 3;
		SrtringArray[3] = 4;
		SrtringArray[4] = 5;
		NxFr::List<ContainerTest> StringList(5);
		StringList.Append(1);
		StringList.Append(2);
		StringList.Append(3);
		StringList.Append(4);
		StringList.Append(5);
		NxFr::Dequeue<ContainerTest> StringDequeue;
		StringDequeue.AppendBack(1);
		StringDequeue.AppendBack(2);
		StringDequeue.AppendBack(3);
		StringDequeue.AppendBack(4);
		StringDequeue.AppendBack(5);
		NxFr::LinkedList<ContainerTest> StringLinkedList;
		StringLinkedList.AppendBack(1);
		StringLinkedList.AppendBack(2);
		StringLinkedList.AppendBack(3);
		StringLinkedList.AppendBack(4);
		StringLinkedList.AppendBack(5);
		NxFr::Set<ContainerTest> StringSet;
		StringSet.Append(1);
		StringSet.Append(2);
		StringSet.Append(3);
		StringSet.Append(4);
		StringSet.Append(5);
		NxFr::Dictionary<ContainerTest, ContainerTest> StringDictionary;
		StringDictionary.Append(1, 50);
		StringDictionary.Append(2, 40);
		StringDictionary.Append(3, 30);
		StringDictionary.Append(4, 20);
		StringDictionary.Append(5, 10);

		NxFr::String String1 = NxFr::StringUtility::ToString(SrtringArray);
		NxFr::String String2 = NxFr::StringUtility::ToString(StringList);
		NxFr::String String3 = NxFr::StringUtility::ToString(StringDequeue);
		NxFr::String String4 = NxFr::StringUtility::ToString(StringLinkedList);
		NxFr::String String5 = NxFr::StringUtility::ToString(StringSet);
		NxFr::String String6 = NxFr::StringUtility::ToString(StringDictionary);

		ContainerTest NativePointerData;
		NxFr::Dictionary<ContainerTest*, uint64> Native;
		Native.Append(&NativePointerData, 45);
		Native.Append(nullptr			, 58);
		ASSERT_EQ(Native.GetCount(), 2);

		NxFr::List<ContainerTest> Container = { 1, 2, 3, 4, 5 };
		NxFr::List<NxFr::List<ContainerTest>> ContainerList;
		ContainerList.Append(Container);
		ASSERT_EQ(ContainerList.First().GetCount(), Container.GetCount());
		NxFr::LinkedList<NxFr::List<ContainerTest>> ContainerLinkedList;
		ContainerLinkedList.AppendBack(Container);
		ASSERT_EQ(ContainerLinkedList.First().GetCount(), Container.GetCount());
		NxFr::Dictionary<ContainerTest, NxFr::List<ContainerTest>> ContainerDictionary;
		ContainerDictionary.Append(0, Container);
		ASSERT_EQ(ContainerDictionary[0].GetCount(), Container.GetCount());

		NxFr::Array<ContainerTest> IteratorReverseArray = { 0, 1, 2, 3, 4 };
		uint64 Index = IteratorReverseArray.GetCount() - 1;
		for (auto It = IteratorReverseArray.BeginReverse(); It != IteratorReverseArray.EndReverse(); --It)
		{
			It->Integer = Index--;
		}
		ASSERT_EQ(IteratorReverseArray.First().Integer, 0);
		ASSERT_EQ(IteratorReverseArray.Last().Integer, IteratorReverseArray.GetCount() - 1);
		NxFr::Dequeue<ContainerTest> IteratorReverseDequeue = { 0, 1, 2, 3, 4 };
		Index = IteratorReverseDequeue.GetCount() - 1;
		for (auto It = IteratorReverseDequeue.BeginReverse(); It != IteratorReverseDequeue.EndReverse(); --It)
		{
			It->Integer = Index--;
		}
		ASSERT_EQ(IteratorReverseDequeue.First().Integer, 0);
		ASSERT_EQ(IteratorReverseDequeue.Last().Integer, IteratorReverseDequeue.GetCount() - 1);
		NxFr::LinkedList<ContainerTest> IteratorReverseLinkedList = { 0, 1, 2, 3, 4 };
		Index = IteratorReverseLinkedList.GetCount() - 1;
		for (auto It = IteratorReverseLinkedList.BeginReverse(); It != IteratorReverseLinkedList.EndReverse(); --It)
		{
			It->Integer = Index--;
		}
		ASSERT_EQ(IteratorReverseLinkedList.First().Integer, 0);
		ASSERT_EQ(IteratorReverseLinkedList.Last().Integer, IteratorReverseLinkedList.GetCount() - 1);
	}
}
