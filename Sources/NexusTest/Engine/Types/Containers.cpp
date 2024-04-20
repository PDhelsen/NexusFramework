#include "Core/NexusTestPch.h"

#include "Types/Containers/Array.h"
#include "Types/Containers/List.h"

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

		bool operator<=(const ContainerTest& Other)
		{
			return Integer <= Other.Integer;
		}

		bool operator>(const ContainerTest& Other)
		{
			return Integer > Other.Integer;
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
}