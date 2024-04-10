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
	};

	TEST(Type_Containers, Array)
	{
		NxEn::Array<ContainerTest> Test = NxEn::Array<ContainerTest>(10);
		ASSERT_EQ(Test.GetCount(), 10);
	
		Test.Initialize(5);
		ASSERT_EQ(Test[1].Integer, 5);

		ContainerTest Copy = Test[5];
		Copy.Integer = 10;
		ASSERT_NE(Test[5].Integer, 10);

		NxEn::Array<ContainerTest> CopyShallow = Test.CopyShallow();
		ASSERT_EQ(&Test[5], &CopyShallow[5]);

		NxEn::Array<ContainerTest> CopyDeep = Test.CopyDeep();
		ASSERT_NE(&Test[5], &CopyDeep[5]);
		ASSERT_EQ(Test[5].Integer, CopyDeep[5].Integer);

		ASSERT_EQ(Test == CopyShallow, true);
		ASSERT_EQ(Test == CopyDeep, false);

		Test[3] = ContainerTest(3);
		Test[6] = ContainerTest(6);
		ASSERT_EQ(Test[3].Integer, 3);
		ASSERT_EQ(Test[6].Integer, 6);

		Test.Swap(3, 6);
		ASSERT_EQ(Test[3].Integer, 6);

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

		Index = 9;
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
		uint64 Found;
		bool Contains = Test.Find(Test2, Found);
		ASSERT_EQ(Contains, true);
		ASSERT_EQ(Found, 2);


		Test[0].Integer = 8;
		Test[1].Integer = 4;
		Test[2].Integer = 7;
		Test[3].Integer = 1;
		Test[4].Integer = 0;
		Test[5].Integer = 5;
		Test[6].Integer = 6;
		Test[7].Integer = 2;
		Test[8].Integer = 9;
		Test[9].Integer = 3;

		Test.Sort();
		for (Index = 0; Index < Test.GetCount(); Index++)
		{
			ASSERT_EQ(Test[Index].Integer, Index);
		}
	}
}