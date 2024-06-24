#include "Core/NexusTestPch.h"
#include "Types/String.h"

namespace NxTs
{
	TEST(Type_String, String)
	{
		NxEn::String Test1 = NxEn::String("Hello World");
		NxEn::String Test2 = NxEn::String("World Hello");
		NxEn::String Test3 = Test1;
		NxEn::String Test4 = Move(Test2);
		{
			NxEn::String Test5 = NxEn::String("Hello World");
			ASSERT_EQ(Test5.IsEmpty(), false);
		}

		ASSERT_EQ(Test1.IsEmpty(), false);
		ASSERT_EQ(Test1.IsNullTerminated(), true);
		ASSERT_EQ(Test1.GetCount(), 11);
		ASSERT_EQ(Test1.GetCapacity(), 16);

		ASSERT_EQ(Test1, NxEn::String("Hello World"));
		ASSERT_EQ(Test1, "Hello World");
		ASSERT_EQ(Test1 > NxEn::String("Hello World"), false);
		ASSERT_EQ(Test1 <= "Hello World", true);
		ASSERT_EQ(Test1 < Test4, true);
		ASSERT_EQ(Test1 >= Test4, false);
		NxEn::String Test6 = Test1 + Test4;
		ASSERT_EQ(Test1, "Hello World");
		ASSERT_EQ(Test4, "World Hello");
		ASSERT_EQ(Test6, "Hello WorldWorld Hello");
		Test4 += Test1;
		ASSERT_EQ(Test4, "World HelloHello World");
		NxEn::String Test7 = Test6 - "o";
		ASSERT_EQ(Test6, "Hello WorldWorld Hello");
		ASSERT_EQ(Test7, "Hell WrldWrld Hell");
		Test7 -= "l";
		ASSERT_EQ(Test7, "He WrdWrd He");
		NxEn::String Test8 = NxEn::String("Hello") + NxEn::String("World");
		NxEn::String Test9 = NxEn::String("Hello") + "World";
		NxEn::String Test10 = "Hello" + NxEn::String("World");
		NxEn::String Test11 = "HelloWorld";
		ASSERT_EQ(Test8, "HelloWorld");
		ASSERT_EQ(Test9, "HelloWorld");
		ASSERT_EQ(Test10, "HelloWorld");
		ASSERT_EQ(Test11, "HelloWorld");

		Test1.Append("Modofood");
		ASSERT_EQ(Test1, "Hello WorldModofood");
		Test1.Remove("o", 1, 1);
		ASSERT_EQ(Test1, "Hello WrldModofood");
		Test1.Remove("o", 0, 0, true);
		ASSERT_EQ(Test1, "Hell WrldMdfd");
		Test1.Remove("Mdfd");
		ASSERT_EQ(Test1, "Hell Wrld");
		Test1.Remove("Hell");
		ASSERT_EQ(Test1, " Wrld");
		Test1.Assign("rl", "abc");
		ASSERT_EQ(Test1, " Wabcd");
		Test1.Assign("abc", "ef");
		ASSERT_EQ(Test1, " Wefd");
		Test1.Assign("efd", "ijkl");
		ASSERT_EQ(Test1, " Wijkl");
		Test1.Insert("ij", "mnop");
		ASSERT_EQ(Test1, " Wijmnopkl");
		Test1.Clear();
		ASSERT_EQ(Test1.GetCount(), 0);

		Test1.Clear();
		Test1.Append("Hello World");

		Test1.Grow(30);
		ASSERT_EQ(Test1.GetCapacity(), 30);
		Test1.Shrink();
		ASSERT_EQ(Test1.GetCapacity(), 17);

		Test1.Clear();
		Test1.Append("Hello World");

		ASSERT_EQ(Test1.Replace("l", "m"), "Hemmo Wormd");
		ASSERT_EQ(Test1.Replace("m", "m"), "Hemmo Wormd");

		Test1.Clear();
		Test1.Append("Hello World");

		ASSERT_EQ(Test1.Compare(Test4), -1);
		ASSERT_EQ(Test1.Start("Hello"), true);
		ASSERT_EQ(Test1.Start("World"), false);
		ASSERT_EQ(Test1.Start("llo"), false);
		ASSERT_EQ(Test1.End("Hello"), false);
		ASSERT_EQ(Test1.End("World"), true);
		ASSERT_EQ(Test1.End("Wor"), false);

		Test1.Clear();
		Test1.Append("Hello World");

		ASSERT_EQ(Test1.Contains("Hello"), true);
		ASSERT_EQ(Test1.Contains("Wd", NxEn::String::SearchMode::Characters), true);
		ASSERT_EQ(Test1.Find("ll") != nullptr, true);
		ASSERT_EQ(Test1.Find("l", 1, NxEn::String::SearchMode::Characters) != nullptr, true);
		ASSERT_EQ(Test1.Split("ll") != nullptr, true);
		ASSERT_EQ(Test1.Split("l", 1, NxEn::String::SearchMode::Characters) != nullptr, true);

		Test1.Clear();
		Test1.Append("Hello World 1 Hello World 2 Hello World 3 Hello World 4");
		NxEn::List<const char*> Found = Test1.FindAll("Hello");
		ASSERT_EQ(Found.GetCount(), 4);
		NxEn::List<const char*> Split = Test1.SplitAll(" ");
		ASSERT_EQ(Split.GetCount(), 12);

		NxEn::String Test12 = NxEn::String::Format("Hello %i World %.1f, %s", 10, 20.0f, "Out of space");
		ASSERT_EQ(Test12, "Hello 10 World 20.0, Out of space");
		NxEn::String Test13 = NxEn::String::Format(40, "Hello %i World %.1f, %s", 10, 20.0f, "Out of space");
		ASSERT_EQ(Test13, "Hello 10 World 20.0, Out of space");
		NxEn::String Test14 = NxEn::String::Format("%s", Test1.C());
		ASSERT_EQ(Test14, "Hello World 1 Hello World 2 Hello World 3 Hello World 4");
		NxEn::String Test15 = NxEn::String::Format("%d", 1997);
		ASSERT_EQ(Test15, "1997");
		int32 Day, Year;
		NxEn::String Weekday = NxEn::String(20), Month = NxEn::String(20);
		NxEn::String::Scan("Saturday March 25 1989", "%s %s %d  %d", Weekday.C(), Month.C(), &Day, &Year);
		ASSERT_EQ(Day, 25);
		ASSERT_EQ(Year, 1989);
		ASSERT_EQ(Month, "March");
		ASSERT_EQ(Weekday, "Saturday");

		NxEn::String Test16 = NxEn::String::ToStringF(128.6f);
		ASSERT_EQ(Test16, "128.60");
		NxEn::String Test17 = NxEn::String::ToStringI(-100);
		ASSERT_EQ(Test17, "-100");
		NxEn::String Test18 = NxEn::String::ToStringB(true);
		ASSERT_EQ(Test18, "True");
		ASSERT_EQ(NxEn::String::ToInteger("-10"), -10);
		ASSERT_EQ(NxEn::String::ToUnsignedInteger("100"), 100);
		ASSERT_EQ(NxEn::String::ToDouble("-10.0"), -10.0f);
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
