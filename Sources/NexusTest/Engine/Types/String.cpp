#include "Core/NexusTestPch.h"

#include "Types/Strings/String.h"
#include "Types/Strings/StringId.h"
#include "Types/Strings/StringView.h"
#include "Types/Strings/StringCApi.h"
#include "Types/Strings/StringFunctions.h"
#include "Types/Strings/StringTemplate.h"

namespace NxTs
{
	TEST(Type_String, String)
	{
		NxEn::String Test = NxEn::String("Hello World");

		NxEn::String Empty = NxEn::String();
		ASSERT_EQ(Empty, NxEn::String::Empty);
		ASSERT_EQ(Empty.IsEmpty(), true);

		NxEn::String Test2 = "World Hello";
		NxEn::String Test3 = Test;
		NxEn::String Test4 = Move(Test2);
		{
			NxEn::String Test5 = NxEn::String("Hello World");
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

	TEST(Type_String, View)
	{
		NxEn::String Reference = "Hello World Extended";

		NxEn::StringView Test0 = Reference.ToView();
		NxEn::StringView Test1 = Reference.ToView(0, 5);
		NxEn::StringView Test2 = Reference.ToView(6, 5);
		NxEn::StringView Test3 = Reference.ToView(3, 8);
		NxEn::StringView Test4 = Reference.ToView(12, 8);

		ASSERT_EQ(Reference.C() == Test0.C(), true);
		ASSERT_EQ(Reference.C() == Test1.C(), true);

		ASSERT_EQ(Reference == Test0, true);
		ASSERT_EQ(Reference > Test1, true);
		ASSERT_EQ(Reference < Test2, true);
		ASSERT_EQ(Reference > Test4, true);

		NxEn::Array<NxEn::StringView> Array = NxEn::Array<NxEn::StringView>(6);
		Array.AssignConstruct(0, Reference);
		Array.AssignConstruct(1, Test0);
		Array.AssignConstruct(2, Test1);
		Array.AssignConstruct(3, Test2);
		Array.AssignConstruct(4, Test3);
		Array.AssignConstruct(5, Test4);
		Array.Sort();

		NxEn::String Builder = Reference;
		Builder.Append(Test1);
		ASSERT_EQ(Builder, "Hello World ExtendedHello");
		Builder.Assign(Test1, Test4);
		ASSERT_EQ(Builder, "Extended World ExtendedHello");
		Builder.Insert(Test4, Test1);
		ASSERT_EQ(Builder, "ExtendedHello World ExtendedHello");
		Builder.Remove(Test2);
		ASSERT_EQ(Builder, "ExtendedHello  ExtendedHello");

		NxEn::String Functions = "Hello Not World Not Extended";
		ASSERT_EQ(Functions.Start(Test1), true);
		ASSERT_EQ(Functions.End(Test4), true);
		ASSERT_EQ(Functions.Contains(Test2), true);
		ASSERT_EQ(Functions.Find(Test2).IsEmpty(), false);
		ASSERT_EQ(Functions.Split(Test2).IsEmpty(), false);
	}

	TEST(Type_String, Id)
	{
		NxEn::StringId Id = "Hello World"_Sid;

		NxEn::StringId Test1 = NxEn::StringId("Hello World");
		NxEn::StringId Test2 = NxEn::StringId("World Hello");

		ASSERT_EQ(Id == Test1, true);
		ASSERT_EQ(Id != Test2, true);

		ASSERT_EQ(Id.ToString(), "Hello World");
		ASSERT_EQ(Id.GetId(), 0x3D58DEE72D4E0C27);
	}

	TEST(Type_String, Utility)
	{
		NxEn::String Test1 = NxEn::String("Hello World");

		ASSERT_EQ(NxEn::StringUtility::Start(Test1, "Hello"), true);
		ASSERT_EQ(NxEn::StringUtility::Start(Test1, "World"), false);
		ASSERT_EQ(NxEn::StringUtility::Start(Test1, "llo"), false);
		ASSERT_EQ(NxEn::StringUtility::End(Test1,"Hello"), false);
		ASSERT_EQ(NxEn::StringUtility::End(Test1,"World"), true);
		ASSERT_EQ(NxEn::StringUtility::End(Test1,"Wor"), false);

		ASSERT_EQ(NxEn::StringUtility::Contains(Test1, "Hello"), true);
		ASSERT_EQ(NxEn::StringUtility::Contains(Test1, "Wd", NxEn::StringUtility::SearchMode::Characters), true);
		ASSERT_EQ(NxEn::StringUtility::Find(Test1, "ll").IsEmpty(), false);
		ASSERT_EQ(NxEn::StringUtility::Find(Test1, "l", 1, NxEn::StringUtility::SearchMode::Characters).IsEmpty(), false);
		ASSERT_EQ(NxEn::StringUtility::Split(Test1, "ll").IsEmpty(), false);
		ASSERT_EQ(NxEn::StringUtility::Split(Test1, "l", 2, NxEn::StringUtility::SearchMode::Characters).IsEmpty(), false);

		NxEn::String Test2 = NxEn::String("Hello World 1 Hello World 2 Hello World 3 Hello World 4");
		NxEn::List<NxEn::StringView> Found = NxEn::StringUtility::FindAll(Test2, "Hello");
		ASSERT_EQ(Found.GetCount(), 4);
		NxEn::List<NxEn::StringView> Split = NxEn::StringUtility::SplitAll(Test2, " ");
		ASSERT_EQ(Split.GetCount(), 12);

		NxEn::String Test3 = NxEn::StringUtility::Format("Hello %i World %.1f, %s", 10, 20.0f, Test1.C());
		ASSERT_EQ(Test3, "Hello 10 World 20.0, Hello World");
		int32 Day = 0, Year = 0;
		NxEn::String Weekday = NxEn::String(), Month = NxEn::String();
		uint64 ScanCount = NxEn::StringUtility::Scan("Saturday March 25 1989", "%s %s %d %d", Weekday.C(), Month.C(), &Day, &Year);
		Weekday.Validate();
		Month.Validate();
		ASSERT_EQ(Day, 25);
		ASSERT_EQ(Year, 1989);
		ASSERT_EQ(Month, "March");
		ASSERT_EQ(Weekday, "Saturday");
		ASSERT_EQ(ScanCount, 4);

		ASSERT_EQ(NxEn::StringUtility::ToStringF(128.6f), "128.60");
		ASSERT_EQ(NxEn::StringUtility::ToStringI(-100), "-100");
		ASSERT_EQ(NxEn::StringUtility::ToStringB(true), "True");
		ASSERT_EQ(NxEn::StringUtility::ToInteger("-10"), -10);
		ASSERT_EQ(NxEn::StringUtility::ToUnsignedInteger("100"), 100);
		ASSERT_EQ(NxEn::StringUtility::ToDouble("-10.0"), -10.0f);
	}

	TEST(Type_String, Operator)
	{
		NxEn::String Test = NxEn::String("Hello World");
		NxEn::String Reverse = NxEn::String("World Hello");

		ASSERT_EQ(Test, NxEn::String("Hello World"));
		ASSERT_EQ(Test, "Hello World");
		ASSERT_EQ(Test > NxEn::String("Hello World"), false);
		ASSERT_EQ(Test <= "Hello World", true);
		ASSERT_EQ(Test < Reverse, true);
		ASSERT_EQ(Test >= Reverse, false);

		NxEn::String Test2 = Test + Reverse;
		ASSERT_EQ(Test, "Hello World");
		ASSERT_EQ(Reverse, "World Hello");
		ASSERT_EQ(Test2, "Hello WorldWorld Hello");
		Test2 += Test;
		ASSERT_EQ(Test2, "Hello WorldWorld HelloHello World");

		NxEn::String Test3 = Test2 - "o";
		ASSERT_EQ(Test2, "Hello WorldWorld HelloHello World");
		ASSERT_EQ(Test3, "Hell WrldWrld HellHell Wrld");
		Test3 -= "l";
		ASSERT_EQ(Test3, "He WrdWrd HeHe Wrd");

		ASSERT_EQ(NxEn::String("Hello") + NxEn::String("World"), "HelloWorld");
		ASSERT_EQ(NxEn::String("Hello") + "World", "HelloWorld");
		ASSERT_EQ("Hello" + NxEn::String("World"), "HelloWorld");
		ASSERT_EQ(NxEn::String("HelloWorld") - NxEn::String("World"), "Hello");
		ASSERT_EQ(NxEn::String("HelloWorld") - "World", "Hello");
		ASSERT_EQ("HelloWorld" - NxEn::String("World"), "Hello");
	}

	TEST(Type_String, Hash_Sort)
	{
		uint64 Hash1 = 0x2474E7FB1AEC9F05;
		uint64 Hash2 = 0x77F122B9F752AACB;
		NxEn::String String1 = NxEn::String("Test");
		NxEn::String String2 = NxEn::String("This is a test text for testing the Hash function");
		ASSERT_EQ(NxEn::Hash<NxEn::String>::HashObject(String1), Hash1);
		ASSERT_EQ(NxEn::Hash<NxEn::String>::HashObject(String2), Hash2);

		NxEn::Array<NxEn::String> Array = NxEn::Array<NxEn::String>(10);
		Array.AssignConstruct(0, NxEn::String("ABEG"));
		Array.AssignConstruct(1, NxEn::String("ABEF"));
		Array.AssignConstruct(2, NxEn::String("BCDE"));
		Array.AssignConstruct(3, NxEn::String("WWWW"));
		Array.AssignConstruct(4, NxEn::String("ABCD"));
		Array.AssignConstruct(5, NxEn::String("BDEF"));
		Array.AssignConstruct(6, NxEn::String("AAAA"));
		Array.AssignConstruct(7, NxEn::String("ABEG"));
		Array.AssignConstruct(8, NxEn::String("WWWW"));
		Array.AssignConstruct(9, NxEn::String("EFGH"));

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
