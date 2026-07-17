#include "NexusTests/Core/NexusTests.h"
#include "NexusTests/Core/NexusTestsDummy.h"

namespace NxTs
{
	TEST(Type_Containers, Array)
	{
		Dummy Container = Dummy(1);
		NxFr::Array<Dummy> Range = { 4, 4, 4, 4, 4 };

		NxFr::Array<Dummy, 10> Test = NxFr::Array<Dummy, 10>();
		ASSERT_EQ(Test.GetCount(), 10);

		Test.Assign(0, Container);
		Test.Assign(1, Dummy(2));
		Test.AssignConstruct(2, 3);
		Test.AssignRange(3, Range);
		ASSERT_EQ(Test[0].Key, 1);
		ASSERT_EQ(Test[1].Key, 2);
		ASSERT_EQ(Test[2].Key, 3);
		ASSERT_EQ(Test[3].Key, 4);
		ASSERT_EQ(Test[7].Key, 4);

		Dummy AccesCopy = Test[5];
		AccesCopy.Key = 10;
		ASSERT_NE(Test[5].Key, 10);

		Dummy& AccesRef= Test[5];
		AccesRef.Key = 10;
		ASSERT_EQ(Test[5].Key, 10);

		NxFr::Array<Dummy, 10> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy[0].Key, Test[0].Key);

		ASSERT_EQ(Test.Get(0).Key, Test[0].Key);
		ASSERT_EQ(Test.First().Key, Test[0].Key);
		ASSERT_EQ(Test.Last().Key, Test[Test.GetCount() - 1].Key);

		ASSERT_EQ(Test.GetIterator(5)->Key, Test[5].Key);
		ASSERT_EQ(Test.GetIterator(5).Id(), 5);
		ASSERT_EQ(Test.Begin()->Key, Test.First().Key);
		ASSERT_EQ(Test.BeginReverse()->Key, Test.Last().Key);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Key = Index++;
		}
		ASSERT_EQ(Test.First().Key, 0);
		ASSERT_EQ(Test.Last().Key, Test.GetCount() - 1);

		ASSERT_EQ(Test.IsValidIndex(5), true);
		ASSERT_EQ(Test.IsValidIndex(Test.GetCount()), false);
	}

	TEST(Type_Containers, List)
	{
		Dummy Container = Dummy(1);
		NxFr::List<Dummy> Range = { 4, 4, 4, 4, 4 };

		NxFr::List<Dummy> Test = NxFr::List<Dummy>(10);
		ASSERT_EQ(Test.GetCapacity(), 10);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Test.Append(Container);
		Test.Append(Dummy(2));
		Test.AppendConstruct(3);
		Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCapacity(), 10);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[0].Key, 1);
		ASSERT_EQ(Test[1].Key, 2);
		ASSERT_EQ(Test[2].Key, 3);
		ASSERT_EQ(Test[3].Key, 4);
		ASSERT_EQ(Test[7].Key, 4);

		Test.Assign(7, Container);
		Test.Assign(6, Dummy(2));
		Test.AssignConstruct(5, 3);
		Test.AssignRange(0, Range);
		ASSERT_EQ(Test.GetCapacity(), 10);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[7].Key, 1);
		ASSERT_EQ(Test[6].Key, 2);
		ASSERT_EQ(Test[5].Key, 3);
		ASSERT_EQ(Test[4].Key, 4);
		ASSERT_EQ(Test[0].Key, 4);

		Test.Insert(3, Container);
		Test.Insert(4, Dummy(2));
		Test.InsertConstruct(5, 3);
		Test.InsertRange(6, Range);
		ASSERT_EQ(Test.GetCapacity() > 10, true);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[3].Key, 1);
		ASSERT_EQ(Test[4].Key, 2);
		ASSERT_EQ(Test[5].Key, 3);
		ASSERT_EQ(Test[6].Key, 4);
		ASSERT_EQ(Test[11].Key, 4);

		Test.Remove(3);
		Test.RemoveLast();
		Test.RemoveSwap(3);
		Test.RemoveRange(3, 3);
		ASSERT_EQ(Test.GetCapacity() > 10, true);
		ASSERT_EQ(Test.GetCount(), 10);
		ASSERT_EQ(Test.IsEmpty(), false);

		Dummy AccesCopy = Test[5];
		AccesCopy.Key = 10;
		ASSERT_NE(Test[5].Key, 10);

		Dummy& AccesRef = Test[5];
		AccesRef.Key = 10;
		ASSERT_EQ(Test[5].Key, 10);

		NxFr::List<Dummy> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy[0].Key, Test[0].Key);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get(0).Key, Test[0].Key);
		ASSERT_EQ(Test.First().Key, Test[0].Key);
		ASSERT_EQ(Test.Last().Key, Test[Test.GetCount() - 1].Key);

		ASSERT_EQ(Test.GetIterator(5)->Key, Test[5].Key);
		ASSERT_EQ(Test.GetIterator(5).Id(), 5);
		ASSERT_EQ(Test.Begin()->Key, Test.First().Key);
		ASSERT_EQ(Test.BeginReverse()->Key, Test.Last().Key);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Key = Index++;
		}
		ASSERT_EQ(Test.First().Key, 0);
		ASSERT_EQ(Test.Last().Key, Test.GetCount() - 1);

		ASSERT_EQ(Test.IsValidIndex(5), true);
		ASSERT_EQ(Test.IsValidIndex(Test.GetCount()), false);
		Test.Reserve(30);
		ASSERT_EQ(Test.GetCapacity(), 30);
		Test.Reserve(0);
		ASSERT_EQ(Test.GetCapacity(), 10);
	}

	TEST(Type_Containers, Dequeue)
	{
		Dummy Container = Dummy(1);
		NxFr::Dequeue<Dummy> Range = { 4, 4, 4, 4, 4 };

		NxFr::Dequeue<Dummy> Test = NxFr::Dequeue<Dummy>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Test.AppendBack(Container);
		Test.AppendBack(Dummy(2));
		Test.AppendBackConstruct(3);
		Test.AppendBackRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[0].Key, 1);
		ASSERT_EQ(Test[1].Key, 2);
		ASSERT_EQ(Test[2].Key, 3);
		ASSERT_EQ(Test[3].Key, 4);
		ASSERT_EQ(Test[7].Key, 4);

		Test.AppendFront(Container);
		Test.AppendFront(Dummy(2));
		Test.AppendFrontConstruct(3);
		Test.AppendFrontRange(Range);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[7].Key, 1);
		ASSERT_EQ(Test[6].Key, 2);
		ASSERT_EQ(Test[5].Key, 3);
		ASSERT_EQ(Test[4].Key, 4);
		ASSERT_EQ(Test[0].Key, 4);

		Test.Assign(3, Container);
		Test.Assign(4, Dummy(2));
		Test.AssignConstruct(5, 3);
		Test.AssignRange(6, Range);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test[3].Key, 1);
		ASSERT_EQ(Test[4].Key, 2);
		ASSERT_EQ(Test[5].Key, 3);
		ASSERT_EQ(Test[6].Key, 4);
		ASSERT_EQ(Test[11].Key, 4);

		Test.RemoveBack();
		Test.RemoveFront();
		ASSERT_EQ(Test.GetCount(), 14);
		ASSERT_EQ(Test.IsEmpty(), false);

		Dummy AccesCopy = Test[5];
		AccesCopy.Key = 10;
		ASSERT_NE(Test[5].Key, 10);

		Dummy& AccesRef = Test[5];
		AccesRef.Key = 10;
		ASSERT_EQ(Test[5].Key, 10);

		NxFr::Dequeue<Dummy> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy[0].Key, Test[0].Key);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get(0).Key, Test[0].Key);
		ASSERT_EQ(Test.First().Key, Test[0].Key);
		ASSERT_EQ(Test.Last().Key, Test[Test.GetCount() - 1].Key);

		ASSERT_EQ(Test.GetIterator(5)->Key, Test[5].Key);
		ASSERT_EQ(Test.GetIterator(5).Id(), 5);
		ASSERT_EQ(Test.Begin()->Key, Test.First().Key);
		ASSERT_EQ(Test.BeginReverse()->Key, Test.Last().Key);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Key = Index++;
		}
		ASSERT_EQ(Test.First().Key, 0);
		ASSERT_EQ(Test.Last().Key, Test.GetCount() - 1);

		ASSERT_EQ(Test.IsValidIndex(5), true);
		ASSERT_EQ(Test.IsValidIndex(Test.GetCount()), false);
	}

	TEST(Type_Containers, Stack)
	{
		Dummy Container = Dummy(1);
		NxFr::Stack<Dummy> Range = { 4, 4, 4, 4, 4 };

		NxFr::Stack<Dummy> Test = NxFr::Stack<Dummy>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Dummy& Index1 = Test.Append(Container);
		Dummy& Index2 = Test.Append(Dummy(2));
		Dummy& Index3 = Test.AppendConstruct(3);
		Dummy& Index4 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Key, 1);
		ASSERT_EQ(Index2.Key, 2);
		ASSERT_EQ(Index3.Key, 3);
		ASSERT_EQ(Index4.Key, 4);

		Test.Remove();
		Test.Remove();
		Test.Remove();
		Test.Remove();
		ASSERT_EQ(Test.GetCount(), 4);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxFr::Stack<Dummy> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy.Get().Key, Test.Get().Key);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get().Key, 4);

		ASSERT_EQ(Test.Begin()->Key, 4);
		ASSERT_EQ(Test.BeginReverse()->Key, 1);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Key = Index++;
		}
		ASSERT_EQ(Test.Begin()->Key, 0);
	}

	TEST(Type_Containers, Queue)
	{
		Dummy Container = Dummy(1);
		NxFr::Queue<Dummy> Range = { 4, 4, 4, 4, 4 };

		NxFr::Queue<Dummy> Test = NxFr::Queue<Dummy>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Dummy& Index1 = Test.Append(Container);
		Dummy& Index2 = Test.Append(Dummy(2));
		Dummy& Index3 = Test.AppendConstruct(3);
		Dummy& Index4 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Key, 1);
		ASSERT_EQ(Index2.Key, 2);
		ASSERT_EQ(Index3.Key, 3);
		ASSERT_EQ(Index4.Key, 4);

		Test.Remove();
		Test.Remove();
		ASSERT_EQ(Test.GetCount(), 6);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxFr::Queue<Dummy> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy.Get().Key, Test.Get().Key);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get().Key, 3);

		ASSERT_EQ(Test.Begin()->Key, 3);
		ASSERT_EQ(Test.BeginReverse()->Key, 4);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Key = Index++;
		}
		ASSERT_EQ(Test.Begin()->Key, 0);
	}

	TEST(Type_Containers, LinkedList)
	{
		Dummy Container = Dummy(1);
		NxFr::LinkedList<Dummy> Range = { 4, 4, 4, 4, 4 };

		NxFr::LinkedList<Dummy> Test = NxFr::LinkedList<Dummy>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Dummy& Index1 = Test.AppendBack(Container);
		Dummy& Index2 = Test.AppendBack(Dummy(2));
		Dummy& Index3 = Test.AppendBackConstruct(3);
		Dummy& Index4 = Test.AppendBackRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Key, 1);
		ASSERT_EQ(Index2.Key, 2);
		ASSERT_EQ(Index3.Key, 3);
		ASSERT_EQ(Index4.Key, 4);

		Dummy& Index5 = Test.AppendFront(Container);
		Dummy& Index6 = Test.AppendFront(Dummy(2));
		Dummy& Index7 = Test.AppendFrontConstruct(3);
		Dummy& Index8 = Test.AppendFrontRange(Range);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Key, 1);
		ASSERT_EQ(Index6.Key, 2);
		ASSERT_EQ(Index7.Key, 3);
		ASSERT_EQ(Index8.Key, 4);

		Index5 = Test.Assign(&Index5, Container);
		Index6 = Test.Assign(&Index6, Dummy(2));
		Index7 = Test.AssignConstruct(&Index7, 3);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Key, 1);
		ASSERT_EQ(Index6.Key, 2);
		ASSERT_EQ(Index7.Key, 3);

		Dummy& Index9 = Test.InsertFront(&Index1, Container);
		Dummy& Index10 = Test.InsertFront(&Index2, Dummy(2));
		Dummy& Index11 = Test.InsertFrontConstruct(&Index3, 3);
		Dummy& Index12 = Test.InsertFrontRange(&Index4, Range);
		ASSERT_EQ(Test.GetCount(), 24);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index9.Key, 1);
		ASSERT_EQ(Index10.Key, 2);
		ASSERT_EQ(Index11.Key, 3);
		ASSERT_EQ(Index12.Key, 4);

		Dummy& Index13 = Test.InsertBack(&Index1, Container);
		Dummy& Index14 = Test.InsertBack(&Index2, Dummy(2));
		Dummy& Index15 = Test.InsertBackConstruct(&Index3, 3);
		Dummy& Index16 = Test.InsertBackRange(&Index4, Range);
		ASSERT_EQ(Test.GetCount(), 32);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index13.Key, 1);
		ASSERT_EQ(Index14.Key, 2);
		ASSERT_EQ(Index15.Key, 3);
		ASSERT_EQ(Index16.Key, 4);

		Test.Remove(&Index1, &Index3);
		Test.Remove(&Index4);
		Test.RemoveBack();
		Test.RemoveFront();
		ASSERT_EQ(Test.GetCount(), 22);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxFr::LinkedList<Dummy> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy.First().Key, Test.First().Key);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get().Key, Test.First().Key);
		ASSERT_EQ(Test.TryGetPrev(&Test.First()), nullptr);
		ASSERT_EQ(Test.TryGetNext(&Test.Last()), nullptr);

		Dummy& Next = Test.GetNext(&Index5);
		Dummy& Prev = Test.GetPrev(&Index5);
		ASSERT_EQ(Test.IsNext(&Index5, &Next), true);
		ASSERT_EQ(Test.IsPrevious(&Index5, &Prev), true);

		ASSERT_EQ(Test.GetIterator(&Index5)->Key, Index5.Key);
		ASSERT_EQ(Test.GetIterator(&Index5).Next()->Key, Test.GetNext(&Index5).Key);
		ASSERT_EQ(Test.GetIterator(&Index5).Previous()->Key, Test.GetPrev(&Index5).Key);
		ASSERT_EQ(Test.Begin()->Key, Test.First().Key);
		ASSERT_EQ(Test.BeginReverse()->Key, Test.Last().Key);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Key = Index++;
		}
		ASSERT_EQ(Test.First().Key, 0);
		ASSERT_EQ(Test.Last().Key, Test.GetCount() - 1);
	}

	TEST(Type_Containers, Set)
	{
		Dummy Container = Dummy(1);
		NxFr::Set<Dummy> Range = { 6, 7, 8, 9, 10 };

		NxFr::Set<Dummy> Test = NxFr::Set<Dummy>();
		ASSERT_EQ(Test.GetCapacity(), 11);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		const Dummy& Index1 = Test.Append(Container);
		const Dummy& Index2 = Test.Append(Dummy(2));
		const Dummy& Index3 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 7);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Key, 1);
		ASSERT_EQ(Index2.Key, 2);

		Test.Remove(8);
		ASSERT_EQ(Test.GetCount(), 6);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxFr::Set<Dummy> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.GetIterator(6)->Key, 6);

		Test.Reserve(21);
		ASSERT_EQ(Test.GetCapacity(), 23);
	}

	TEST(Type_Containers, Dictionary)
	{
		Dummy Container = Dummy(1);
		NxFr::Dictionary<Dummy, Dummy> Range = { { 6, 60 }, { 7, 70 }, { 8, 80 }, { 9, 90 }, { 10, 100 } };

		NxFr::Dictionary<Dummy, Dummy> Test = NxFr::Dictionary<Dummy, Dummy>();
		ASSERT_EQ(Test.GetCapacity(), 11);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		const Dummy& Index1 = Test.Append(Container, 10);
		const Dummy& Index2 = Test.Append(Dummy(2), 20);
		const Dummy& Index3 = Test.AppendConstruct(Dummy(3), 30);
		const Dummy& Index4 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Key, 10);
		ASSERT_EQ(Index2.Key, 20);
		ASSERT_EQ(Index3.Key, 30);

		const Dummy& Index5 = Test.Assign(Container, 100);
		const Dummy& Index6 = Test.Assign(Dummy(2), 200);
		const Dummy& Index7 = Test.AssignConstruct(Dummy(3), 300);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Key, 100);
		ASSERT_EQ(Index6.Key, 200);
		ASSERT_EQ(Index7.Key, 300);

		Test.Remove(8);
		ASSERT_EQ(Test.GetCount(), 7);
		ASSERT_EQ(Test.IsEmpty(), false);

		Dummy AccesCopy = Test[2];
		AccesCopy.Key = 2000;
		ASSERT_NE(Test[2].Key, 2000);

		Dummy& AccesRef = Test[2];
		AccesCopy.Key = 2000;
		ASSERT_NE(Test[2].Key, 2000);

		NxFr::Dictionary<Dummy, Dummy> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get(9).Key, 90);
		ASSERT_EQ(Test.GetIterator(6)->Key.Key, 6);

		for (auto& [Key, Value] : Test)
		{
			Value.Key = Key.Key * 10;
		}

		Test.Reserve(21);
		ASSERT_EQ(Test.GetCapacity(), 23);
	}

	TEST(Type_Containers, Tree)
	{
		Dummy Container = Dummy(1);
		NxFr::Tree<Dummy> Range = NxFr::Tree<Dummy>();
		Dummy& RangeRoot = Range.Append(nullptr, 0);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);
		Range.Append(&RangeRoot, 4);

		NxFr::Tree<Dummy> Test = NxFr::Tree<Dummy>();
		Dummy& Root = Test.Append(nullptr, 0);
		ASSERT_EQ(Test.GetCount(), 1);
		ASSERT_EQ(Test.IsEmpty(), false);

		Dummy& Index1 = Test.Append(&Root, Container);
		Dummy& Index2 = Test.Append(&Root, Dummy(2));
		Dummy& Index3 = Test.AppendConstruct(&Root, 3);
		Dummy& Index4 = Test.AppendRange(&Root, Range);
		ASSERT_EQ(Test.GetCount(), 10);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Key, 1);
		ASSERT_EQ(Index2.Key, 2);
		ASSERT_EQ(Index3.Key, 3);
		ASSERT_EQ(Index4.Key, 0);

		Index1 = Test.Assign(&Index1, Container);
		Index2 = Test.Assign(&Index2, Dummy(2));
		Index3 = Test.AssignConstruct(&Index3, 3);
		ASSERT_EQ(Test.GetCount(), 10);
		ASSERT_EQ(Test.IsEmpty(), false);

		Dummy& Index5 = Test.InsertSibling(&Index1, Container);
		Dummy& Index6 = Test.InsertSibling(&Index2, Dummy(5));
		Dummy& Index7 = Test.InsertSibling(&Index3, 6);
		ASSERT_EQ(Test.GetCount(), 13);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index5.Key, 1);
		ASSERT_EQ(Index6.Key, 5);
		ASSERT_EQ(Index7.Key, 6);

		Dummy& Index8 = Test.InsertChild(&Index1, Container);
		Dummy& Index9 = Test.InsertChild(&Index2, Dummy(7));
		Dummy& Index10 = Test.InsertChild(&Index3, 8);
		ASSERT_EQ(Test.GetCount(), 16);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index8.Key, 1);
		ASSERT_EQ(Index9.Key, 7);
		ASSERT_EQ(Index10.Key, 8);

		Test.Remove(&Index2);
		Test.RemoveChildren(&Index3);
		ASSERT_EQ(Test.GetCount(), 13);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxFr::Tree<Dummy> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		ASSERT_EQ(Copy.Begin()->Key, Test.Begin()->Key);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		ASSERT_EQ(Test.Get().Key, 0);
		ASSERT_EQ(Test.GetParent(&Index1).Key, Root.Key);
		ASSERT_EQ(Test.GetSibling(&Index1).Key, 1);
		ASSERT_EQ(Test.GetChild(&Index1).Key, 1);
		ASSERT_EQ(Test.IsParent(&Index1, &Root), true);
		ASSERT_EQ(Test.IsSibling(&Index1, &Index5), true);
		ASSERT_EQ(Test.IsChild(&Index1, &Index8), true);

		ASSERT_EQ(Test.GetIterator(&Index5)->Key, Index5.Key);
		ASSERT_EQ(Test.Begin()->Key, Root.Key);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Key = Index++;
		}
	}

	TEST(Type_Containers, Graph)
	{
		Dummy Container = Dummy(1);
		NxFr::Graph<Dummy> Range = NxFr::Graph<Dummy>();
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);
		Range.Append(4);

		NxFr::Graph<Dummy> Test = NxFr::Graph<Dummy>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Dummy& Index1 = Test.Append(Container);
		Dummy& Index2 = Test.Append(Dummy(2));
		Dummy& Index3 = Test.AppendConstruct(3);
		Dummy& Index4 = Test.AppendRange(Range);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Index1.Key, 1);
		ASSERT_EQ(Index2.Key, 2);
		ASSERT_EQ(Index3.Key, 3);
		ASSERT_EQ(Index4.Key, 4);

		Index1 = Test.Assign(&Index1, Container);
		Index2 = Test.Assign(&Index2, Dummy(2));
		Index3 = Test.AssignConstruct(&Index3, 3);
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.IsEmpty(), false);

		Test.Remove(&Index4);
		ASSERT_EQ(Test.GetCount(), 7);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxFr::Graph<Dummy> Copy = Test;
		ASSERT_EQ(Copy == Test, false);
		ASSERT_EQ(Copy.GetCount(), Test.GetCount());
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		Test.Connect(&Index1, &Index2);
		Test.Connect(&Index1, &Index3);
		Test.Connect(&Index2, &Index3);
		ASSERT_EQ(Test.GetConnection(&Index1, NxFr::Graph<Dummy>::CT::To, 0).Key, 2);
		ASSERT_EQ(Test.GetConnection(&Index1, NxFr::Graph<Dummy>::CT::To, 1).Key, 3);
		ASSERT_EQ(Test.GetConnection(&Index2, NxFr::Graph<Dummy>::CT::To, 0).Key, 3);
		ASSERT_EQ(Test.IsConnected(&Index1, &Index2, NxFr::Graph<Dummy>::CT::To), true);
		ASSERT_EQ(Test.IsConnected(&Index3, &Index1, NxFr::Graph<Dummy>::CT::From), true);

		Test.Disconnect(&Index1, &Index3);
		ASSERT_EQ(Test.IsConnected(&Index3, &Index1, NxFr::Graph<Dummy>::CT::From), false);

		ASSERT_EQ(Test.GetIterator(&Index1)->Key, Index1.Key);
		ASSERT_EQ(Test.Begin()->Key, Test.Get().Key);

		uint64 Index = 0;
		for (auto& It : Test)
		{
			It.Key = Index++;
		}
	}

	TEST(Type_Containers, Buffer)
	{
		Dummy Raw[5];
		Raw[0].Key = 0;
		Raw[1].Key = 1;
		Raw[2].Key = 2;
		Raw[3].Key = 3;
		Raw[4].Key = 4;

		NxFr::BufferView View = NxFr::BufferView(Raw, 5 * sizeof(Dummy));
		ASSERT_EQ(View.GetPtr<Dummy>(0 * sizeof(Dummy))->Key, 0);
		ASSERT_EQ(View.GetPtr<Dummy>(1 * sizeof(Dummy))->Key, 1);
		ASSERT_EQ(View.GetPtr<Dummy>(2 * sizeof(Dummy))->Key, 2);
		ASSERT_EQ(View.GetPtr<Dummy>(3 * sizeof(Dummy))->Key, 3);
		ASSERT_EQ(View.GetPtr<Dummy>(4 * sizeof(Dummy))->Key, 4);

		NxFr::Buffer Buffer(3 * sizeof(Dummy));
		Buffer.Set(&Raw[0], sizeof(Dummy), 0 * sizeof(Dummy));
		Buffer.Set(&Raw[2], sizeof(Dummy), 2 * sizeof(Dummy));
		Buffer.Set(&Raw[1], sizeof(Dummy), 1 * sizeof(Dummy));
		ASSERT_EQ(View.GetPtr<Dummy>(0 * sizeof(Dummy))->Key, 0);
		ASSERT_EQ(View.GetPtr<Dummy>(1 * sizeof(Dummy))->Key, 1);
		ASSERT_EQ(View.GetPtr<Dummy>(2 * sizeof(Dummy))->Key, 2);

		Buffer.Resize(5 * sizeof(Dummy));
		Buffer.Set(&Raw[4], sizeof(Dummy), 4 * sizeof(Dummy));
		Buffer.Set(&Raw[3], sizeof(Dummy), 3 * sizeof(Dummy));
		ASSERT_EQ(View.GetPtr<Dummy>(3 * sizeof(Dummy))->Key, 3);
		ASSERT_EQ(View.GetPtr<Dummy>(4 * sizeof(Dummy))->Key, 4);
	}

	TEST(Type_Containers, Pool)
	{
		NxFr::Pool<Dummy, NxFr::Pooling::PreAllocated<Dummy>> TestPreAllocated = NxFr::Pool<Dummy, NxFr::Pooling::PreAllocated<Dummy>>(10);
		ASSERT_EQ(TestPreAllocated.GetCapacity(), 10);
		ASSERT_EQ(TestPreAllocated.GetCount(), 0);
		ASSERT_EQ(TestPreAllocated.GetUnused(), 10);

		Dummy& Test1 = TestPreAllocated.Acquire();
		ASSERT_EQ(TestPreAllocated.GetCount(), 1);
		ASSERT_EQ(TestPreAllocated.GetUnused(), 9);

		Dummy& Test2 = TestPreAllocated.Acquire();
		ASSERT_EQ(TestPreAllocated.GetCount(), 2);
		ASSERT_EQ(TestPreAllocated.GetUnused(), 8);

		Test1.Key = 10;

		TestPreAllocated.Recycle(Test1);
		ASSERT_EQ(TestPreAllocated.GetCount(), 1);
		ASSERT_EQ(TestPreAllocated.GetUnused(), 9);

		Dummy& Test3 = TestPreAllocated.Acquire();
		Dummy& Test4 = TestPreAllocated.Acquire();
		Dummy& Test5 = TestPreAllocated.Acquire();

		TestPreAllocated.Recycle(Test4);
		TestPreAllocated.Recycle(Test3);
		TestPreAllocated.Recycle(Test5);

		Dummy& Test6 = TestPreAllocated.Acquire();
		Dummy& Test7 = TestPreAllocated.Acquire();
		Dummy& Test8 = TestPreAllocated.Acquire();

		TestPreAllocated.Recycle(Test8);

		TestPreAllocated.Clear();
		ASSERT_EQ(TestPreAllocated.GetCount(), 0);
		ASSERT_EQ(TestPreAllocated.GetUnused(), 10);

		NxFr::Pool<Dummy> TestOnDemand = NxFr::Pool<Dummy>();
		ASSERT_EQ(TestOnDemand.GetCapacity(), 0);
		ASSERT_EQ(TestOnDemand.GetCount(), 0);
		ASSERT_EQ(TestOnDemand.GetUnused(), 0);

		Dummy& Test9 = TestOnDemand.Acquire();
		ASSERT_EQ(TestOnDemand.GetCount(),1);
		ASSERT_EQ(TestOnDemand.GetUnused(), 0);

		Dummy& Test10 = TestOnDemand.Acquire();
		ASSERT_EQ(TestOnDemand.GetCount(), 2);
		ASSERT_EQ(TestOnDemand.GetUnused(), 0);

		Test9.Key = 10;

		TestOnDemand.Recycle(Test9);
		ASSERT_EQ(TestOnDemand.GetCount(), 1);
		ASSERT_EQ(TestOnDemand.GetUnused(), 1);

		Dummy& Test11 = TestOnDemand.Acquire();
		Dummy& Test12 = TestOnDemand.Acquire();
		Dummy& Test13 = TestOnDemand.Acquire();

		TestOnDemand.Recycle(Test11);
		TestOnDemand.Recycle(Test12);
		TestOnDemand.Recycle(Test13);

		Dummy& Test14 = TestOnDemand.Acquire();
		Dummy& Test15 = TestOnDemand.Acquire();
		Dummy& Test16 = TestOnDemand.Acquire();

		TestOnDemand.Recycle(Test16);

		TestOnDemand.Clear();
		ASSERT_EQ(TestOnDemand.GetCount(), 0);
		ASSERT_EQ(TestOnDemand.GetUnused(), 4);
	}

	TEST(Type_Containers, Tuple)
	{
		NxFr::Tuple<Dummy, Dummy> Test = NxFr::Tuple(Dummy(5), Dummy(10));

		Dummy& Test1 = Test.GetFirst();
		Dummy& Test2 = Test.GetSecond();
		ASSERT_EQ(Test1.Key, 5);
		ASSERT_EQ(Test2.Key, 10);

		Test.SetFirst(Dummy(20));
		Test.SetSecond(Dummy(25));

		ASSERT_EQ(Test1.Key, 20);
		ASSERT_EQ(Test2.Key, 25);
	}

	TEST(Type_Containers, Collection)
	{
		NxFr::List<Dummy> Data = NxFr::List<Dummy>(5);
		Data.Append(1);
		Data.Append(2);
		Data.Append(3);
		Data.Append(4);
		Data.Append(5);

		uint64 Index = 0;
		NxFr::Collection<Dummy> Test = Data;
		for (auto It = Test.Begin(); It != Test.End(); ++It)
		{
			ASSERT_EQ(It->Key, ++Index);
		}

		auto It = Test.Begin();
		ASSERT_EQ((*It).Key, 1);
	}

	TEST(Type_Containers, ContainerUtility)
	{
		NxFr::Array<Dummy> Data = { 1, 3, 5, 7, 9 };
		NxFr::ContainerUtility::Swap<Dummy>(Data, 0, 1);
		ASSERT_EQ(Data[0].Key, 3);
		ASSERT_EQ(Data[1].Key, 1);
		NxFr::ContainerUtility::Sort<Dummy>(Data);
		for (uint64 Index = 1; Index < Data.GetCount(); Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}
		NxFr::ContainerUtility::Reverse<Dummy>(Data);
		for (uint64 Index = 1; Index < Data.GetCount(); Index++)
		{
			ASSERT_EQ(Data[Index - 1] > Data[Index], true);
		}
		ASSERT_EQ(NxFr::ContainerUtility::Any<Dummy>(Data, [](const Dummy& Element) { return Element.Key == 5; }), true);
		ASSERT_EQ(NxFr::ContainerUtility::Contains<Dummy>(Data, 9), true);
		ASSERT_NE(NxFr::ContainerUtility::Where<Dummy>(Data, [](const Dummy& Element) { return Element.Key == 5; }), Data.End());
		ASSERT_NE(NxFr::ContainerUtility::Find<Dummy>(Data, 9), Data.End());

		NxFr::ContainerUtility::Fill<Dummy>(Data, 5);
		ASSERT_EQ(Data.First().Key, 5);
		ASSERT_EQ(Data.Last().Key, 5);
		NxFr::ContainerUtility::Resize<Dummy>(Data, 10);
		ASSERT_EQ(Data.GetCount(), 10);

		NxFr::Set<Dummy> SetBase = { 1, 2, 3, 4, 5, 6, 7, 9 };
		NxFr::Set<Dummy> SetOther = { 2, 4, 6, 8, 10 };
		NxFr::ContainerUtility::Intersection(SetBase, SetOther);
		ASSERT_EQ(SetBase.GetCount(), 3);

		NxFr::Array<Dummy> ToArray = NxFr::ContainerUtility::ToArray<Dummy>(SetOther);
		ASSERT_EQ(SetOther.GetCount(), 5);
	}

	TEST(Type_Containers, Misc)
	{
		NxFr::Array<Dummy> SrtringArray(5);
		SrtringArray[0] = 1;
		SrtringArray[1] = 2;
		SrtringArray[2] = 3;
		SrtringArray[3] = 4;
		SrtringArray[4] = 5;
		NxFr::List<Dummy> StringList(5);
		StringList.Append(1);
		StringList.Append(2);
		StringList.Append(3);
		StringList.Append(4);
		StringList.Append(5);
		NxFr::Dequeue<Dummy> StringDequeue;
		StringDequeue.AppendBack(1);
		StringDequeue.AppendBack(2);
		StringDequeue.AppendBack(3);
		StringDequeue.AppendBack(4);
		StringDequeue.AppendBack(5);
		NxFr::LinkedList<Dummy> StringLinkedList;
		StringLinkedList.AppendBack(1);
		StringLinkedList.AppendBack(2);
		StringLinkedList.AppendBack(3);
		StringLinkedList.AppendBack(4);
		StringLinkedList.AppendBack(5);
		NxFr::Set<Dummy> StringSet;
		StringSet.Append(1);
		StringSet.Append(2);
		StringSet.Append(3);
		StringSet.Append(4);
		StringSet.Append(5);
		NxFr::Dictionary<Dummy, Dummy> StringDictionary;
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

		Dummy NativePointerData;
		NxFr::Dictionary<Dummy*, uint64> Native;
		Native.Append(&NativePointerData, 45);
		Native.Append(nullptr			, 58);
		ASSERT_EQ(Native.GetCount(), 2);

		NxFr::List<Dummy> Container = { 1, 2, 3, 4, 5 };
		NxFr::List<NxFr::List<Dummy>> ContainerList;
		ContainerList.Append(Container);
		ASSERT_EQ(ContainerList.First().GetCount(), Container.GetCount());
		NxFr::LinkedList<NxFr::List<Dummy>> ContainerLinkedList;
		ContainerLinkedList.AppendBack(Container);
		ASSERT_EQ(ContainerLinkedList.First().GetCount(), Container.GetCount());
		NxFr::Dictionary<Dummy, NxFr::List<Dummy>> ContainerDictionary;
		ContainerDictionary.Append(0, Container);
		ASSERT_EQ(ContainerDictionary[0].GetCount(), Container.GetCount());

		NxFr::Array<Dummy> IteratorReverseArray = { 0, 1, 2, 3, 4 };
		uint64 Index = IteratorReverseArray.GetCount() - 1;
		for (auto It = IteratorReverseArray.BeginReverse(); It != IteratorReverseArray.EndReverse(); --It)
		{
			It->Key = Index--;
		}
		ASSERT_EQ(IteratorReverseArray.First().Key, 0);
		ASSERT_EQ(IteratorReverseArray.Last().Key, IteratorReverseArray.GetCount() - 1);
		NxFr::Dequeue<Dummy> IteratorReverseDequeue = { 0, 1, 2, 3, 4 };
		Index = IteratorReverseDequeue.GetCount() - 1;
		for (auto It = IteratorReverseDequeue.BeginReverse(); It != IteratorReverseDequeue.EndReverse(); --It)
		{
			It->Key = Index--;
		}
		ASSERT_EQ(IteratorReverseDequeue.First().Key, 0);
		ASSERT_EQ(IteratorReverseDequeue.Last().Key, IteratorReverseDequeue.GetCount() - 1);
		NxFr::LinkedList<Dummy> IteratorReverseLinkedList = { 0, 1, 2, 3, 4 };
		Index = IteratorReverseLinkedList.GetCount() - 1;
		for (auto It = IteratorReverseLinkedList.BeginReverse(); It != IteratorReverseLinkedList.EndReverse(); --It)
		{
			It->Key = Index--;
		}
		ASSERT_EQ(IteratorReverseLinkedList.First().Key, 0);
		ASSERT_EQ(IteratorReverseLinkedList.Last().Key, IteratorReverseLinkedList.GetCount() - 1);

		NxFr::Array<Dummy> IteratorView = { 0, 1, 2, 3, 4 };
		auto View = NxFr::Iterator::View(IteratorView.Begin(), IteratorView.End());
		for (auto& It : View)
		{
			It.Key = 0;
		}
		ASSERT_EQ(IteratorView.Last().Key, 0);
	}
}
