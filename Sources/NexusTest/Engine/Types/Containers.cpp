#include "Core/NexusTestPch.h"

#include "Types/Containers/Array.h"

namespace NxTs
{
	struct ContainerTest
	{
		uint64 Interger;
		float Float;
		bool Boolean;

		ContainerTest()
		{
			Interger = 1;
			Float = 1.0f;
			Boolean = true;
		}

		ContainerTest(uint64 Initialization)
		{
			Interger = Initialization;
			Float = 1.0f;
			Boolean = true;
		}

		bool operator==(const ContainerTest& Other)
		{
			return Interger == Other.Interger;
		}

		bool operator!=(const ContainerTest& Other)
		{
			return Interger != Other.Interger;
		}
	};

	TEST(Type_Containers, Array)
	{
		NxEn::Array<ContainerTest> Test = NxEn::Array<ContainerTest>(10);
		ASSERT_EQ(Test.GetCount(), 10);
	
		Test.Initialize(5);
		ASSERT_EQ(Test[1].Interger, 5);

		ContainerTest Copy = Test[5];
		Copy.Interger = 10;
		ASSERT_NE(Test[5].Interger, 10);

		NxEn::Array<ContainerTest> CopyShallow = Test.CopyShallow();
		ASSERT_EQ(&Test[5], &CopyShallow[5]);

		NxEn::Array<ContainerTest> CopyDeep = Test.CopyDeep();
		ASSERT_NE(&Test[5], &CopyDeep[5]);
		ASSERT_EQ(Test[5].Interger, CopyDeep[5].Interger);

		ASSERT_EQ(Test == CopyShallow, true);
		ASSERT_EQ(Test == CopyDeep, false);

		Test[3] = ContainerTest(3);
		Test[6] = ContainerTest(6);
		ASSERT_EQ(Test[3].Interger, 3);
		ASSERT_EQ(Test[6].Interger, 6);

		Test.Swap(3, 6);
		ASSERT_EQ(Test[3].Interger, 6);

		uint64 Index = 0;
		for (NxEn::Array<ContainerTest>::Iterator Iterator = Test.Begin(); Iterator != Test.End(); ++Iterator)
		{
			ContainerTest& Container = *Iterator;
			Container.Interger = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index++;
		}
		ASSERT_EQ(Test[3].Interger, 3);

		Index = 9;
		for (NxEn::Array<ContainerTest>::Iterator Iterator = Test.BeginReverse(); Iterator != Test.EndReverse(); --Iterator)
		{
			ContainerTest& Container = *Iterator;
			Container.Interger = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index--;
		}
		ASSERT_EQ(Test[3].Interger, 3);

		Index = 0;
		for (ContainerTest& Container : Test)
		{
			Container.Interger = Index;
			Container.Float = (float)Index;
			Container.Boolean = Index % 2 == 1;
			Index++;
		}
		ASSERT_EQ(Test[3].Interger, 3);

		ContainerTest Test2 = ContainerTest(2);
		bool Contains = Test.Contains(Test2);
		uint64 Found = Test.Find(Test2);
		ASSERT_EQ(Contains, true);
		ASSERT_EQ(Found, 2);
	}
}