#include "Core/NexusTestPch.h"

#include "Types/Containers/Array.h"
#include "Types/Containers/List.h"
#include "Types/Containers/Pool.h"
#include "Types/Containers/LinkedList.h"
#include "Types/Containers/Stack.h"
#include "Types/Containers/Queue.h"
#include "Types/Containers/Dequeue.h"
#include "Types/Containers/Tuple.h"
#include "Types/Containers/Tree.h"
#include "Types/Containers/Graph.h"

namespace NxTs
{
	struct ContainerTest
	{
		uint64 Integer;
		float Float;
		bool Boolean;

		ContainerTest()
		{
			Integer = 1;
			Float = 1.0f;
			Boolean = true;
		}

		ContainerTest(uint64 Initialization)
		{
			Integer = Initialization;
			Float = 1.0f;
			Boolean = true;
		}

		ContainerTest(const ContainerTest& Other)
		{
			Integer = Other.Integer;
			Float = Other.Float;
			Boolean = Other.Boolean;

			NEXUS_LOG(App, Info, 0, "Copy");
		}

		ContainerTest(ContainerTest&& Other) noexcept
		{
			Integer = Other.Integer;
			Float = Other.Float;
			Boolean = Other.Boolean;

			NEXUS_LOG(App, Info, 0, "Move");
		}

		ContainerTest& operator=(const ContainerTest& Other)
		{
			Integer = Other.Integer;
			Float = Other.Float;
			Boolean = Other.Boolean;

			NEXUS_LOG(App, Info, 0, "Copy");
			return *this;
		}

		ContainerTest& operator=(ContainerTest&& Other) noexcept
		{
			Integer = Other.Integer;
			Float = Other.Float;
			Boolean = Other.Boolean;

			NEXUS_LOG(App, Info, 0, "Move");
			return *this;
		}

		bool operator==(const ContainerTest& Other)
		{
			return Integer == Other.Integer;
		}

		bool operator!=(const ContainerTest& Other)
		{
			return Integer != Other.Integer;
		}

		bool operator>(const ContainerTest& Other)
		{
			return Integer > Other.Integer;
		}

		bool operator<(const ContainerTest& Other)
		{
			return Integer < Other.Integer;
		}
		
		bool operator>=(const ContainerTest& Other)
		{
			return Integer >= Other.Integer;
		}
		
		bool operator<=(const ContainerTest& Other)
		{
			return Integer <= Other.Integer;
		}
	};

	TEST(Type_Containers, Array)
	{
		NxEn::Array<ContainerTest> Test = NxEn::Array<ContainerTest>(9);
		ASSERT_EQ(Test.GetCount(), 9);
	
		Test.Initialize(5);
		ASSERT_EQ(Test[1].Integer, 5);

		NxEn::Array<ContainerTest> CopyDeep = Test.Copy();
		ASSERT_NE(&Test[5], &CopyDeep[5]);
		ASSERT_EQ(Test[5].Integer, CopyDeep[5].Integer);
		ASSERT_EQ(Test == CopyDeep, false);

		ContainerTest Copy = Test[5];
		Copy.Integer = 10;
		ASSERT_NE(Test[5].Integer, 10);

		ContainerTest& Reference = Test[0];
		Reference.Integer = 10;
		ASSERT_EQ(Test[0].Integer, 10);

		Test[3] = ContainerTest(3);
		Test[6] = ContainerTest(6);
		ASSERT_EQ(Test[3].Integer, 3);
		ASSERT_EQ(Test[6].Integer, 6);

		Test.Assign(5, ContainerTest(7));
		Test.AssignConstruct(7, 8);
		ASSERT_EQ(Test[5].Integer, 7);
		ASSERT_EQ(Test[7].Integer, 8);

		Test.AssignRange(0, CopyDeep);
		ASSERT_EQ(Test[0].Integer, 5);
		ASSERT_EQ(Test[8].Integer, 5);

		Test[3].Integer = 3;
		Test[6].Integer = 6;
		Test.Swap(3, 6);
		ASSERT_EQ(Test[3].Integer, 6);
		ASSERT_EQ(Test[6].Integer, 3);

		uint64 Index = 0;
		for (NxEn::Array<ContainerTest>::Iterator Iterator = Test.Begin(); Iterator != Test.End(); ++Iterator)
		{
			ContainerTest& Container = *Iterator;
			Container.Integer = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index++;
		}
		ASSERT_EQ(Test[3].Integer, 3);

		Index = Test.GetCount() - 1;
		for (NxEn::Array<ContainerTest>::Iterator Iterator = Test.BeginReverse(); Iterator != Test.EndReverse(); --Iterator)
		{
			ContainerTest& Container = *Iterator;
			Container.Integer = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index--;
		}
		ASSERT_EQ(Test[3].Integer, 3);

		Index = 0;
		for (ContainerTest& Container : Test)
		{
			Container.Integer = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index++;
		}
		ASSERT_EQ(Test[3].Integer, 3);

		ContainerTest Test2 = ContainerTest(2);
		ASSERT_EQ(Test.Contains(Test2), true);
		ASSERT_EQ(Test.Find(Test2), 2);

		Test[0].Integer = 8;
		Test[1].Integer = 4;
		Test[2].Integer = 7;
		Test[3].Integer = 1;
		Test[4].Integer = 0;
		Test[5].Integer = 5;
		Test[6].Integer = 6;
		Test[7].Integer = 2;
		Test[8].Integer = 3;

		Test.Sort();
		for (Index = 0; Index < Test.GetCount(); Index++)
		{
			ASSERT_EQ(Test[Index].Integer, Index);
		}

		Test.Reverse();
		for (Index = 0; Index < Test.GetCount(); Index++)
		{
			ASSERT_EQ(Test[Index].Integer, Test.GetCount() - 1 - Index);
		}
	}

	TEST(Type_Containers, List)
	{
		NxEn::List<ContainerTest> Test = NxEn::List<ContainerTest>(3);
		ASSERT_EQ(Test.GetCapacity(), 3);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Test.Append(ContainerTest(5));
		Test.Append(ContainerTest(6));
		Test.Append(ContainerTest(7));
		Test.Append(ContainerTest(8));
		Test.Append(ContainerTest(9));
		ASSERT_EQ(Test.GetCapacity(), 6);
		ASSERT_EQ(Test.GetCount(), 5);
		ASSERT_EQ(Test.IsEmpty(), false);

		NxEn::List<ContainerTest> CopyDeep = Test.Copy();
		ASSERT_NE(&Test[0], &CopyDeep[0]);
		ASSERT_EQ(Test[0].Integer, CopyDeep[0].Integer);
		ASSERT_EQ(Test == CopyDeep, false);

		ContainerTest Copy = Test[0];
		Copy.Integer = 10;
		ASSERT_NE(Test[0].Integer, 10);

		ContainerTest& Reference = Test[0];
		Reference.Integer = 10;
		ASSERT_EQ(Test[0].Integer, 10);

		Test[0] = ContainerTest(3);
		Test[1] = ContainerTest(6);
		ASSERT_EQ(Test[0].Integer, 3);
		ASSERT_EQ(Test[1].Integer, 6);

		Test.Assign(0, ContainerTest(7));
		Test.AssignConstruct(1, 8);
		ASSERT_EQ(Test[0].Integer, 7);
		ASSERT_EQ(Test[1].Integer, 8);

		Test.AssignRange(0, CopyDeep);
		ASSERT_EQ(Test[0].Integer, 5);
		ASSERT_EQ(Test[4].Integer, 9);

		Test.Append(ContainerTest(12));
		Test.AppendConstruct(11);
		ASSERT_EQ(Test[5].Integer, 12);
		ASSERT_EQ(Test[6].Integer, 11);

		Test.AppendRange(CopyDeep);
		ASSERT_EQ(Test[0].Integer, 5);
		ASSERT_EQ(Test[11].Integer, 9);

		Test.Insert(5, Copy);
		ASSERT_EQ(Test[5].Integer, 10);
		ASSERT_EQ(Test[6].Integer, 12);

		Test.InsertConstruct(5, 13);
		ASSERT_EQ(Test[5].Integer, 13);
		ASSERT_EQ(Test[6].Integer, 10);

		Test.InsertRange(5, CopyDeep);
		ASSERT_EQ(Test[5].Integer, 5);
		ASSERT_EQ(Test[9].Integer, 9);
		ASSERT_EQ(Test[10].Integer, 13);

		Test.Remove(5);
		ASSERT_EQ(Test[5].Integer, 6);

		Test.RemoveRange(5, 4);
		ASSERT_EQ(Test[5].Integer, 13);

		Test.RemoveLast();
		ASSERT_EQ(Test[12].Integer, 8);

		Test.RemoveSwap(5);
		ASSERT_EQ(Test[5].Integer, 8);

		Test.Clear(true);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.GetCapacity(), 2);

		Test.AppendRange(CopyDeep);

		Test[0].Integer = 3;
		Test[1].Integer = 6;
		Test.Swap(0, 1);
		ASSERT_EQ(Test[0].Integer, 6);
		ASSERT_EQ(Test[1].Integer, 3);

		uint64 Index = 0;
		for (NxEn::List<ContainerTest>::Iterator Iterator = Test.Begin(); Iterator != Test.End(); ++Iterator)
		{
			ContainerTest& Container = *Iterator;
			Container.Integer = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index++;
		}
		ASSERT_EQ(Test[3].Integer, 3);

		Index = Test.GetCount() - 1;
		for (NxEn::List<ContainerTest>::Iterator Iterator = Test.BeginReverse(); Iterator != Test.EndReverse(); --Iterator)
		{
			ContainerTest& Container = *Iterator;
			Container.Integer = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index--;
		}
		ASSERT_EQ(Test[3].Integer, 3);

		Index = 0;
		for (ContainerTest& Container : Test)
		{
			Container.Integer = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index++;
		}
		ASSERT_EQ(Test[3].Integer, 3);

		Test.Resize(9);
		ASSERT_EQ(Test.GetCount(), 9);
		ASSERT_EQ(Test.GetCapacity(), 9);

		Test.Reserve(18);
		ASSERT_EQ(Test.GetCount(), 9);
		ASSERT_EQ(Test.GetCapacity(), 18);

		Test.Shrink();
		ASSERT_EQ(Test.GetCount(), 9);
		ASSERT_EQ(Test.GetCapacity(), 9);

		ContainerTest Test2 = ContainerTest(2);
		ASSERT_EQ(Test.Contains(Test2), true);
		ASSERT_EQ(Test.Find(Test2), 2);

		Test[0].Integer = 3;
		Test[1].Integer = 4;
		Test[2].Integer = 2;
		Test[3].Integer = 1;
		Test[4].Integer = 6;
		Test[5].Integer = 8;
		Test[6].Integer = 5;
		Test[7].Integer = 7;
		Test[8].Integer = 0;

		Test.Sort();
		for (Index = 0; Index < Test.GetCount(); Index++)
		{
			ASSERT_EQ(Test[Index].Integer, Index);
		}

		Test.Reverse();
		for (Index = 0; Index < Test.GetCount(); Index++)
		{
			ASSERT_EQ(Test[Index].Integer, Test.GetCount() - 1 - Index);
		}
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

	TEST(Type_Containers, LinkedList)
	{
		NxEn::LinkedList<ContainerTest> Test = NxEn::LinkedList<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);

		Test.AppendBack(ContainerTest(5));
		ASSERT_EQ(Test.Last().Integer, 5);
		Test.AppendBack(8);
		ASSERT_EQ(Test.Last().Integer, 8);
		Test.AppendFront(10);
		ASSERT_EQ(Test.First().Integer, 10);
		Test.AppendFront(ContainerTest(3));
		ASSERT_EQ(Test.First().Integer, 3);
		ASSERT_EQ(Test.GetCount(), 4);

		ContainerTest& First = Test.First();
		First.Integer = 62;
		ASSERT_EQ(Test.First().Integer, 62);
		ContainerTest& Next = Test.GetNext(&First);
		Next.Integer = 46;
		ASSERT_EQ(Test.GetNext(&Test.First()).Integer, 46);
		ContainerTest& Last = Test.Last();
		Last.Integer = 24;
		ASSERT_EQ(Test.Last().Integer, 24);
		ContainerTest& Prev = Test.GetPrev(&Last);
		Prev.Integer = 72;
		ASSERT_EQ(Test.GetPrev(&Test.Last()).Integer, 72);

		Test.Assign(&First, ContainerTest(6));
		ASSERT_EQ(Test.First().Integer, 6);
		Test.Assign(&Last, 8);
		ASSERT_EQ(Test.Last().Integer, 8);

		Test.InsertAfter(&Next, ContainerTest(56));
		Test.InsertAfter(&Prev, 23);
		Test.InsertBefore(&Prev, ContainerTest(32));
		Test.InsertBefore(&Next, 2);
		ASSERT_EQ(Test.GetCount(), 8);

		Test.Swap(&Test.First(), &Test.Last());
		Test.Swap(&Test.First(), &Test.Last());
		Test.Swap(&Test.GetNext(&Test.First()), &Test.GetPrev(&Test.Last()));
		Test.Swap(&Test.GetNext(&Test.First()), &Test.GetPrev(&Test.Last()));
		Test.Swap(&Test.GetNext(&Test.GetNext(&Test.GetNext(&Test.First()))), &Test.GetPrev(&Test.GetPrev(&Test.GetPrev(&Test.Last()))));
		Test.Swap(&Test.GetNext(&Test.GetNext(&Test.GetNext(&Test.First()))), &Test.GetPrev(&Test.GetPrev(&Test.GetPrev(&Test.Last()))));

		Test.AppendFront(2);

		bool Contains = Test.Contains(ContainerTest(23));
		ASSERT_EQ(Contains, true);
		Test.Sort();
		for (ContainerTest& It : Test)
		{
			ContainerTest* Next = Test.TryGetNext(&It);
			if (Next != nullptr)
			{
				ASSERT_EQ(It <= *Next, true);
			}
		}
		Test.Reverse();
		for (ContainerTest& It : Test)
		{
			ContainerTest* Next = Test.TryGetNext(&It);
			if (Next != nullptr)
			{
				ASSERT_EQ(It >= *Next, true);
			}
		}

		NxEn::LinkedList<ContainerTest> Copy = Test.Copy();
		uint64 Index = 0;
		for (NxEn::LinkedList<ContainerTest>::Iterator It = Copy.begin(); It != Copy.End(); It++, Index++)
		{
			It->Integer = Index;
		}

		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		Test.RemoveBack();
		Test.RemoveFront();
		ASSERT_EQ(Test.GetCount(), 7);

		Test.Remove(&Test.GetNext(&Test.First()), &Test.GetPrev(&Test.Last()));
		ASSERT_EQ(Test.GetCount(), 2);
	}

	TEST(Type_Containers, Stack)
	{
		NxEn::Stack<ContainerTest> Test = NxEn::Stack<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);

		Test.Append(ContainerTest(5));
		ASSERT_EQ(Test.Get().Integer, 5);
		Test.Append(8);
		ASSERT_EQ(Test.Get().Integer, 8);

		ContainerTest& Test1 = Test.Get();
		Test1.Integer = 15;
		ASSERT_EQ(Test.Get().Integer, 15);

		Test.Append(10);
		ASSERT_EQ(Test.Get().Integer, 10);
		Test.Append(ContainerTest(3));
		ASSERT_EQ(Test.Get().Integer, 3);
		Test.Append(ContainerTest(19));
		ASSERT_EQ(Test.Get().Integer, 19);
		Test.Append(ContainerTest(8));
		ASSERT_EQ(Test.Get().Integer, 8);
		ASSERT_EQ(Test.GetCount(), 6);

		NxEn::Stack<ContainerTest> Copy = Test.Copy();
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		Test.Remove();
		ASSERT_EQ(Test.GetCount(), 5);
		Test.Remove();
		ASSERT_EQ(Test.GetCount(), 4);

		Test.Append(10);
		Test.Append(17);
		Test.Append(25);

		bool Contains = Test.Contains(25);
		ASSERT_EQ(Contains, true);

		Test.Reverse();
		ASSERT_EQ(Test.Get().Integer, 5);
	}

	TEST(Type_Containers, Queue)
	{
		NxEn::Queue<ContainerTest> Test = NxEn::Queue<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);

		Test.Append(ContainerTest(5));
		ASSERT_EQ(Test.Get().Integer, 5);
		Test.Append(8);
		ASSERT_EQ(Test.Get().Integer, 5);

		ContainerTest& Test1 = Test.Get();
		Test1.Integer = 15;
		ASSERT_EQ(Test.Get().Integer, 15);

		Test.Append(10);
		ASSERT_EQ(Test.Get().Integer, 15);
		Test.Append(ContainerTest(3));
		Test.Append(ContainerTest(19));
		Test.Append(ContainerTest(8));
		ASSERT_EQ(Test.GetCount(), 6);

		NxEn::Queue<ContainerTest> Copy = Test.Copy();
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		Test.Remove();
		ASSERT_EQ(Test.GetCount(), 5);
		Test.Remove();
		ASSERT_EQ(Test.GetCount(), 4);
		ASSERT_EQ(Test.Get().Integer, 10);

		Test.Append(10);
		Test.Append(17);
		Test.Append(25);

		bool Contains = Test.Contains(25);
		ASSERT_EQ(Contains, true);

		Test.Reverse();
		ASSERT_EQ(Test.Get().Integer, 25);
	}

	TEST(Type_Containers, Dequeue)
	{
		NxEn::Dequeue<ContainerTest> Test = NxEn::Dequeue<ContainerTest>();
		ASSERT_EQ(Test.GetChunks(), 1);
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Test.AppendBack(ContainerTest(93));
		Test.AppendBack(ContainerTest(16));
		Test.AppendBack(ContainerTest(73));
		Test.AppendBack(21);
		Test.AppendBack(64);
		Test.AppendBack(24);
		Test.AppendFront(ContainerTest(86));
		Test.AppendFront(ContainerTest(82));
		Test.AppendFront(ContainerTest(60));
		Test.AppendFront(26);
		Test.AppendFront(59);
		Test.AppendFront(34);
		ASSERT_EQ(Test.GetChunks(), 3);
		ASSERT_EQ(Test.GetCount(), 12);
		ASSERT_EQ(Test.IsEmpty(), false);

		ASSERT_EQ(Test[3].Integer, 60);
		ASSERT_EQ(Test[9].Integer, 21);

		NxEn::Dequeue<ContainerTest> CopyDeep = Test.Copy();
		ASSERT_NE(&Test[0], &CopyDeep[0]);
		ASSERT_EQ(Test[0].Integer, CopyDeep[0].Integer);
		ASSERT_EQ(Test == CopyDeep, false);

		CopyDeep.Clear();
		ASSERT_EQ(CopyDeep.GetCount(), 0);
		ASSERT_EQ(CopyDeep.IsEmpty(), true);

		Test.Assign(4, 48);
		ASSERT_EQ(Test[4].Integer, 48);

		Test.RemoveBack();
		Test.RemoveBack();
		Test.RemoveFront();
		Test.RemoveFront();
		ASSERT_EQ(Test.GetCount(), 8);
		ASSERT_EQ(Test.GetChunks(), 1);

		ASSERT_EQ(Test.First().Integer, 26);
		ASSERT_EQ(Test.Last().Integer, 21);

		bool Contains = Test.Contains(ContainerTest(86));
		uint64 Found = Test.Find(ContainerTest(73));
		ASSERT_EQ(Contains, true);
		ASSERT_EQ(Found, 6);

		Test.AppendBack(21);
		Test.AppendBack(64);
		Test.AppendBack(24);
		Test.AppendFront(26);
		Test.AppendFront(59);
		Test.AppendFront(34);

		uint64 Index = 0;
		for (NxEn::Dequeue<ContainerTest>::Iterator Iterator = Test.Begin(); Iterator != Test.End(); ++Iterator)
		{
			ContainerTest& Container = *Iterator;
			Container.Integer = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index++;
		}
		ASSERT_EQ(Test[3].Integer, 3);

		Index = Test.GetCount() - 1;
		for (NxEn::Dequeue<ContainerTest>::Iterator Iterator = Test.BeginReverse(); Iterator != Test.EndReverse(); --Iterator)
		{
			Iterator->Integer = Index;
			Iterator->Float = (float)Index;
			Iterator->Boolean = Index % 2 == 1;
			Index--;
		}
		ASSERT_EQ(Test[3].Integer, 3);

		Index = 0;
		for (ContainerTest& Container : Test)
		{
			Container.Integer = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index++;
		}
		ASSERT_EQ(Test[3].Integer, 3);
	}

	TEST(Type_Containers, Tree)
	{
		NxEn::Tree<ContainerTest> Test = NxEn::Tree<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);
		ASSERT_EQ(Test.IsEmpty(), true);

		Test.Initialize(ContainerTest(10));
		ASSERT_EQ(Test.GetCount(), 1);
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test.GetRoot().Integer, 10);

		Test.Append(&Test.GetRoot(), ContainerTest(1));
		Test.Append(&Test.GetRoot(), ContainerTest(2));
		Test.Append(&Test.GetRoot(), ContainerTest(3));
		ASSERT_EQ(Test.GetCount(), 4);

		ContainerTest& Test1Ref = Test.GetChild(&Test.GetRoot());
		ASSERT_EQ(Test1Ref.Integer, 1);
		ContainerTest& Test2Ref = Test.GetSibling(&Test1Ref);
		ASSERT_EQ(Test2Ref.Integer, 2);
		ContainerTest& Test3Ref = Test.GetSibling(&Test2Ref);
		ASSERT_EQ(Test3Ref.Integer, 3);
		ContainerTest& Test0Ref = Test.GetParent(&Test2Ref);
		ASSERT_EQ(Test0Ref.Integer, 10);

		Test.Append(&Test1Ref, ContainerTest(4));
		Test.Append(&Test2Ref, ContainerTest(5));
		ASSERT_EQ(Test.GetCount(), 6);

		Test.InsertSibling(&Test2Ref, ContainerTest(6));
		Test.InsertSibling(&Test3Ref, ContainerTest(7));
		ASSERT_EQ(Test.GetCount(), 8);

		Test.InsertChild(&Test.GetRoot(), ContainerTest(100));
		ASSERT_EQ(Test.GetCount(), 9);

		NxEn::Tree<ContainerTest> Copy = Test.Copy();
		ASSERT_EQ(Test == Copy, false);
		Test.AppendRange(&Test1Ref, Copy);
		ASSERT_EQ(Test.GetCount(), 18);
		Copy.Clear();
		ASSERT_EQ(Copy.GetCount(), 0);

		uint64 Index = 0;
		for (NxEn::Tree<ContainerTest>::Iterator It = Test.Begin(); It != Test.End(); It++)
		{
			(*It).Integer = Index++;
		}

		Index = 0;
		for (auto& It : Test)
		{
			ASSERT_EQ(It.Integer, Index++);
		}

		ASSERT_EQ(Test.Contains(ContainerTest(5)), true);
		ASSERT_EQ(Test.Contains(ContainerTest(20)), false);

		Test.Remove(&Test2Ref);
		Test.Remove(&Test1Ref);
		ASSERT_EQ(Test.GetCount(), 5);
	}

	TEST(Type_Containers, Graph)
	{
		NxEn::Graph<ContainerTest> Test = NxEn::Graph<ContainerTest>();
		ASSERT_EQ(Test.GetCount(), 0);

		Test.Append(ContainerTest(5));
		ContainerTest* Test1 = Test.TryGetRoot();
		ASSERT_EQ(Test.GetRoot().Integer, 5);
		Test.Append(8);
		ContainerTest* Test2 = Test.TryGetRoot();
		ASSERT_EQ(Test.GetRoot().Integer, 8);
		Test.Append(10);
		ContainerTest* Test3 = Test.TryGetRoot();
		ASSERT_EQ(Test.GetRoot().Integer, 10);
		Test.Append(ContainerTest(3));
		ContainerTest* Test4 = Test.TryGetRoot();
		ASSERT_EQ(Test.GetRoot().Integer, 3);
		ASSERT_EQ(Test.GetCount(), 4);

		Test.Assign(&Test.GetRoot(), ContainerTest(6));
		ASSERT_EQ(Test.GetRoot().Integer, 6);

		bool Contains = Test.Contains(ContainerTest(10));
		ASSERT_EQ(Contains, true);
		Test.Append(8);
		ContainerTest* TestA = Test.TryGetRoot();
		Test.Append(4);
		ContainerTest* TestB = Test.TryGetRoot();
		Test.Swap(TestA, TestB);
		ASSERT_EQ(Test.GetRoot().Integer, 8);

		Test.Connect(TestA, TestB);
		Test.Connect(TestA, Test1);
		Test.Connect(TestA, Test4);
		ASSERT_EQ(Test.GetConnectionCount(TestA), 3);

		ContainerTest* TestConnection1 = Test.TryGetConnection(TestA, NxEn::Graph<ContainerTest>::ConnectionType::To);
		ContainerTest* TestConnection2 = Test.TryGetConnection(TestA, NxEn::Graph<ContainerTest>::ConnectionType::To, 2);
		ContainerTest* TestConnection3 = Test.TryGetConnection(Test1, NxEn::Graph<ContainerTest>::ConnectionType::From);
		ASSERT_EQ(TestConnection1->Integer, 8);
		ASSERT_EQ(TestConnection2->Integer, 6);
		ASSERT_EQ(TestConnection3->Integer, 4);

		Test.Disconnect(TestA, Test1);
		Test.Disconnect(TestA, TestB);
		Test.Disconnect(TestA, Test4);
		ASSERT_EQ(Test.GetConnectionCount(TestA), 0);

		Test.Remove(Test.TryGetRoot());
		ASSERT_EQ(Test.GetRoot().Integer, 4);
		Test.Remove(Test.TryGetRoot());
		ASSERT_EQ(Test.GetRoot().Integer, 6);
		ASSERT_EQ(Test.GetCount(), 4);

		uint64 Index = 0;
		for (ContainerTest& It : Test)
		{
			It.Integer = Index++;
		}

		Index = 0;
		for (NxEn::Graph<ContainerTest>::Iterator It = Test.Begin(); It != Test.End(); It++)
		{
			ASSERT_EQ(It->Integer, Index++);
		}

		Test.Clear();
		ASSERT_EQ(Test.GetCount(), 0);
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
}