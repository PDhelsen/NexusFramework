#include "Core/NexusTestPch.h"

#include "Types/Containers/Array.h"

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
		Test.Assign(7, 8);
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

		Index = 8;
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
}