#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	TEST(String, String)
	{
		NxFr::String Test = NxFr::String("Hello World");

		NxFr::String Empty = NxFr::String();
		ASSERT_EQ(Empty, NxFr::StringUtility::Empty);
		ASSERT_EQ(Empty.IsEmpty(), true);

		NxFr::String Test2 = "World Hello";
		NxFr::String Test3 = Test;
		NxFr::String Test4 = Move(Test2);

		ASSERT_EQ(Test2, "");
		ASSERT_EQ(Test3, "Hello World");
		ASSERT_EQ(Test4, "World Hello");

		Test2 = Move(Test4);

		ASSERT_EQ(Test2, "World Hello");
		ASSERT_EQ(Test4, "");

		{
			NxFr::String Test5 = NxFr::String("Hello World");
			ASSERT_EQ(Test5.IsEmpty(), false);
		}

		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test.GetCount(), 11);
		ASSERT_EQ(Test.GetCapacity(), 16);

		Test.Append(" Appening");
		Test.Append(" Appening");
		ASSERT_EQ(Test, "Hello World Appening Appening");
		Test.Replace(" Appening", " Replaced");
		Test.Replace(" Replaced", " Again");
		ASSERT_EQ(Test, "Hello World Again Again");
		Test.Assign("Aga", "Aggga", 1, 1);
		Test.Assign("o", "a", 0, 2);
		ASSERT_EQ(Test, "Hella Warld Again Agggain");
		Test.Insert("Agggain", "Inserted", 0, 1);
		Test.Insert("Inserted", "In", 0, 0, true);
		ASSERT_EQ(Test, "Hella Warld Again AgggainInsertedIn");
		Test.Remove("Inserted", 0, 1);
		Test.Remove("l", 0, 0, true);
		ASSERT_EQ(Test, "Hea Ward Again AgggainIn");
		Test.Clear();
		ASSERT_EQ(Test.GetCount(), 0);

		Test.Grow(100);
		ASSERT_EQ(Test.GetCapacity(), 100);
		Test.Shrink();
		ASSERT_EQ(Test.GetCapacity(), 17);
	}

	TEST(String, View)
	{
		NxFr::String Reference = "Hello World Extended";

		NxFr::StringView Test0 = Reference.ToView();
		NxFr::StringView Test1 = Reference.ToView(0, 5);
		NxFr::StringView Test2 = Reference.ToView(6, 5);
		NxFr::StringView Test3 = Reference.ToView(3, 8);
		NxFr::StringView Test4 = Reference.ToView(12, 8);

		ASSERT_EQ(Reference.C() == Test0.C(), true);
		ASSERT_EQ(Reference.C() == Test1.C(), true);

		ASSERT_EQ(Reference == Test0, true);
		ASSERT_EQ(Reference > Test1, true);
		ASSERT_EQ(Reference < Test2, true);
		ASSERT_EQ(Reference > Test4, true);

		NxFr::Array<NxFr::StringView> Array = NxFr::Array<NxFr::StringView>(6);
		Array.AssignConstruct(0, Reference);
		Array.AssignConstruct(1, Test0);
		Array.AssignConstruct(2, Test1);
		Array.AssignConstruct(3, Test2);
		Array.AssignConstruct(4, Test3);
		Array.AssignConstruct(5, Test4);
		Array.Sort();

		NxFr::String Builder = Reference;
		Builder.Append(Test1);
		ASSERT_EQ(Builder, "Hello World ExtendedHello");
		Builder.Assign(Test1, Test4);
		ASSERT_EQ(Builder, "Extended World ExtendedHello");
		Builder.Insert(Test4, Test1);
		ASSERT_EQ(Builder, "ExtendedHello World ExtendedHello");
		Builder.Remove(Test2);
		ASSERT_EQ(Builder, "ExtendedHello  ExtendedHello");

		NxFr::String Convert = Test2.ToString();
		ASSERT_EQ(Convert, "World");

		NxFr::String Functions = "Hello Not World Not Extended";
		ASSERT_EQ(Functions.Start(Test1), true);
		ASSERT_EQ(Functions.End(Test4), true);
		ASSERT_EQ(Functions.Contains(Test2), true);
		ASSERT_EQ(Functions.Find(Test2).IsEmpty(), false);
		ASSERT_EQ(Functions.Split(Test2).IsEmpty(), false);
	}

	TEST(String, Id)
	{
		NxFr::StringId Id = "Hello World"_Sid;

		NxFr::StringId Test1 = NxFr::StringId("Hello World");
		NxFr::StringId Test2 = NxFr::StringId("World Hello");
		NxFr::StringId Test3 = Test1;

		ASSERT_EQ(Id == Test1, true);
		ASSERT_EQ(Id != Test2, true);
		ASSERT_EQ(Id == Test3, true);

		ASSERT_EQ(Id.ToString(), "Hello World");
		ASSERT_EQ(Id.C(), NxFr::StringView("Hello World"));
		ASSERT_EQ(Id.GetId(), 0x3D58DEE72D4E0C27);

		Test3 = Test2;
	}

	TEST(String, Utility)
	{
		NxFr::String Test1 = NxFr::String("Hello World");

		ASSERT_EQ(NxFr::StringUtility::Start(Test1, "Hello"), true);
		ASSERT_EQ(NxFr::StringUtility::Start(Test1, "World"), false);
		ASSERT_EQ(NxFr::StringUtility::Start(Test1, "llo"), false);
		ASSERT_EQ(NxFr::StringUtility::End(Test1,"Hello"), false);
		ASSERT_EQ(NxFr::StringUtility::End(Test1,"World"), true);
		ASSERT_EQ(NxFr::StringUtility::End(Test1,"Wor"), false);

		ASSERT_EQ(NxFr::StringUtility::Contains(Test1, "Hello"), true);
		ASSERT_EQ(NxFr::StringUtility::Contains(Test1, "Wd", NxFr::StringUtility::SearchMode::Characters), true);
		ASSERT_EQ(NxFr::StringUtility::Find(Test1, "ll").IsEmpty(), false);
		ASSERT_EQ(NxFr::StringUtility::Find(Test1, "l", 1, NxFr::StringUtility::SearchMode::Characters).IsEmpty(), false);
		ASSERT_EQ(NxFr::StringUtility::Split(Test1, "ll").IsEmpty(), false);
		ASSERT_EQ(NxFr::StringUtility::Split(Test1, "l", 2, NxFr::StringUtility::SearchMode::Characters).IsEmpty(), false);

		NxFr::String Test2 = NxFr::String("Hello World 1 Hello World 2 Hello World 3 Hello World 4");
		NxFr::List<NxFr::StringView> Found = NxFr::StringUtility::FindAll(Test2, "Hello");
		ASSERT_EQ(Found.GetCount(), 4);
		NxFr::List<NxFr::StringView> Split = NxFr::StringUtility::SplitAll(Test2, " ");
		ASSERT_EQ(Split.GetCount(), 12);

		NxFr::String Test3 = NxFr::StringUtility::Format("Hello %i World %.1f, %s", 10, 20.0f, Test1.C());
		ASSERT_EQ(Test3, "Hello 10 World 20.0, Hello World");
		int32 Day = 0, Year = 0;
		NxFr::String Weekday = NxFr::String(), Month = NxFr::String();
		uint64 ScanCount = NxFr::StringUtility::Scan("Saturday March 25 1989", "%s %s %d %d", Weekday.C(), Month.C(), &Day, &Year);
		Weekday.Validate();
		Month.Validate();
		ASSERT_EQ(Day, 25);
		ASSERT_EQ(Year, 1989);
		ASSERT_EQ(Month, "March");
		ASSERT_EQ(Weekday, "Saturday");
		ASSERT_EQ(ScanCount, 4);

		ASSERT_EQ(NxFr::StringUtility::ToStringF(128.6f), "128.60");
		ASSERT_EQ(NxFr::StringUtility::ToStringI(-100), "-100");
		ASSERT_EQ(NxFr::StringUtility::ToStringB(true), "True");
		ASSERT_EQ(NxFr::StringUtility::ToInteger("-10"), -10);
		ASSERT_EQ(NxFr::StringUtility::ToUnsignedInteger("100"), 100);
		ASSERT_EQ(NxFr::StringUtility::ToDouble("-10.0"), -10.0f);
	}

	TEST(String, Operator)
	{
		NxFr::String Test = NxFr::String("Hello World");
		NxFr::String Reverse = NxFr::String("World Hello");

		ASSERT_EQ(Test, NxFr::String("Hello World"));
		ASSERT_EQ(Test, "Hello World");
		ASSERT_EQ(Test > NxFr::String("Hello World"), false);
		ASSERT_EQ(Test <= "Hello World", true);
		ASSERT_EQ(Test < Reverse, true);
		ASSERT_EQ(Test >= Reverse, false);

		NxFr::StringView View = Test.ToView(0, 5);
		ASSERT_EQ(Test > View, true);
		ASSERT_EQ(Test >= View, true);
		ASSERT_EQ(Test < View, false);
		ASSERT_EQ(Test == View, false);

		NxFr::String Test2 = Test + Reverse;
		ASSERT_EQ(Test, "Hello World");
		ASSERT_EQ(Reverse, "World Hello");
		ASSERT_EQ(Test2, "Hello WorldWorld Hello");
		Test2 += Test;
		ASSERT_EQ(Test2, "Hello WorldWorld HelloHello World");

		NxFr::String Test3 = Test2 - "o";
		ASSERT_EQ(Test2, "Hello WorldWorld HelloHello World");
		ASSERT_EQ(Test3, "Hell WrldWrld HellHell Wrld");
		Test3 -= "l";
		ASSERT_EQ(Test3, "He WrdWrd HeHe Wrd");

		ASSERT_EQ(NxFr::String("Hello") + NxFr::String("World"), "HelloWorld");
		ASSERT_EQ(NxFr::String("Hello") + "World", "HelloWorld");
		ASSERT_EQ("Hello" + NxFr::String("World"), "HelloWorld");
		ASSERT_EQ(NxFr::String("HelloWorld") - NxFr::String("World"), "Hello");
		ASSERT_EQ(NxFr::String("HelloWorld") - "World", "Hello");
		ASSERT_EQ("HelloWorld" - NxFr::String("World"), "Hello");
	}

	TEST(String, Hash_Sort)
	{
		uint64 Hash1 = 0x2474E7FB1AEC9F05;
		uint64 Hash2 = 0x77F122B9F752AACB;
		NxFr::String String1 = NxFr::String("Test");
		NxFr::String String2 = NxFr::String("This is a test text for testing the Hash function");
		ASSERT_EQ(NxFr::Hash<>::HashObject(String1), Hash1);
		ASSERT_EQ(NxFr::Hash<>::HashObject(String2), Hash2);

		NxFr::StringId StringId = NxFr::StringId(String2);
		ASSERT_EQ(NxFr::Hash<>::HashObject(StringId), StringId.GetId());

		NxFr::Array<NxFr::String> Array = NxFr::Array<NxFr::String>(10);
		Array.AssignConstruct(0, NxFr::String("ABEG"));
		Array.AssignConstruct(1, NxFr::String("ABEF"));
		Array.AssignConstruct(2, NxFr::String("BCDE"));
		Array.AssignConstruct(3, NxFr::String("WWWW"));
		Array.AssignConstruct(4, NxFr::String("ABCD"));
		Array.AssignConstruct(5, NxFr::String("BDEF"));
		Array.AssignConstruct(6, NxFr::String("AAAA"));
		Array.AssignConstruct(7, NxFr::String("ABEG"));
		Array.AssignConstruct(8, NxFr::String("WWWW"));
		Array.AssignConstruct(9, NxFr::String("EFGH"));

		Array.Sort();
		auto ItFirst = Array.Begin();
		auto& ItSecond = ++Array.Begin();
		while (ItSecond != Array.End())
		{
			ASSERT_EQ(*ItFirst <= *ItSecond, true);
			ItFirst++;
			ItSecond++;
		}
	}
}
