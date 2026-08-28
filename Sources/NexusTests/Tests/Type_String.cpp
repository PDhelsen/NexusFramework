#include "NexusTests/Core/NexusTests.h"
#include "NexusTests/Core/NexusTestsDummy.h"

namespace NxTs
{
	TEST(Type_String, String)
	{
		NxFr::String Test = NxFr::String("Hello World");
		ASSERT_EQ(Test.IsEmpty(), false);
		ASSERT_EQ(Test.GetCount(), 11);
		ASSERT_EQ(Test.GetCapacity(), 16);

		NxFr::String Empty = NxFr::String();
		ASSERT_EQ(Empty, NxFr::StringUtility::Empty);
		ASSERT_EQ(Empty.IsEmpty(), true);

		NxFr::String Test2 = "World Hello";
		NxFr::String Test3 = Test;
		NxFr::String Test4 = Move(Test2);
		ASSERT_EQ(Test2, "");
		ASSERT_EQ(Test3, "Hello World");
		ASSERT_EQ(Test4, "World Hello");

		Test.Append(" Again");
		Test.Append(" Again");
		ASSERT_EQ(Test, "Hello World Again Again");
		Test.Assign("Aga", "Aggga", 1, 1);
		Test.Assign("o", "a", 0, 2);
		ASSERT_EQ(Test, "Hella Warld Again Agggain");
		Test.Insert("Agggain", "Inserted", 0, 1);
		Test.Insert("Inserted", "In");
		ASSERT_EQ(Test, "Hella Warld Again AgggainInsertedIn");
		Test.Remove("Inserted", 0, 1);
		Test.Remove("l");
		ASSERT_EQ(Test, "Hea Ward Again AgggainIn");
		Test.Clear();
		ASSERT_EQ(Test.GetCount(), 0);

		Test.Reserve(100);
		ASSERT_EQ(Test.GetCapacity(), 100);
		Test.Reserve(0, true);
		ASSERT_EQ(Test.GetCapacity(), 17);
	}

	TEST(Type_String, View)
	{
		NxFr::String Reference = "Hello World Extended";

		NxFr::StringView Test0 = Reference;
		NxFr::StringView Test1 = Reference.Substring(0, 5);
		NxFr::StringView Test2 = Reference.Substring(6, 5);
		NxFr::StringView Test3 = Reference.Substring(3, 8);
		NxFr::StringView Test4 = Reference.Substring(12, 8);

		ASSERT_EQ(Reference.C() == Test0.C(), true);
		ASSERT_EQ(Reference.C() == Test1.C(), true);

		ASSERT_EQ(Reference == Test0, true);
		ASSERT_EQ(Reference > Test1, true);
		ASSERT_EQ(Reference < Test2, true);
		ASSERT_EQ(Reference > Test4, true);
	}

	TEST(Type_String, Id)
	{
		NxFr::StringId Id = "Hello World"_Sid;

		NxFr::StringId Test1 = NxFr::StringId("Hello World");
		NxFr::StringId Test2 = NxFr::StringId("World Hello");
		NxFr::StringId Test3 = Test1;

		ASSERT_EQ(Id == Test1, true);
		ASSERT_EQ(Id != Test2, true);
		ASSERT_EQ(Id == Test3, true);

		ASSERT_EQ(Id, "Hello World");
		ASSERT_EQ(Id.GetString(), "Hello World");
		ASSERT_EQ(Id.GetId(), 0x3D58DEE72D4E0C27);

		Test3 = Test2;
		ASSERT_EQ(Test3, Test2);

		NxFr::StringId Test4 = 0;
		ASSERT_EQ(Test4, NxFr::StringUtility::Unknown);
		ASSERT_EQ(Test4.GetId(), 0);
	}

	TEST(Type_String, Utility)
	{
		NxFr::String Test1 = NxFr::String("Hello World");

		ASSERT_EQ(NxFr::StringUtility::Start(Test1, "Hello"), true);
		ASSERT_EQ(NxFr::StringUtility::Start(Test1, "World"), false);
		ASSERT_EQ(NxFr::StringUtility::Start(Test1, "llo"), false);
		ASSERT_EQ(NxFr::StringUtility::End(Test1,"Hello"), false);
		ASSERT_EQ(NxFr::StringUtility::End(Test1,"World"), true);
		ASSERT_EQ(NxFr::StringUtility::End(Test1, "Wor"), false);
		ASSERT_EQ(NxFr::StringUtility::TrimLeading("   Hello World"), "Hello World");
		ASSERT_EQ(NxFr::StringUtility::TrimTrailing("Hello World   "), "Hello World");
		ASSERT_EQ(NxFr::StringUtility::Lower("Hello World"), "hello world");
		ASSERT_EQ(NxFr::StringUtility::Upper("Hello World"), "HELLO WORLD");

		ASSERT_EQ(NxFr::StringUtility::Contains(Test1, "Hello"), true);
		ASSERT_EQ(NxFr::StringUtility::Contains(Test1, "Wd", NxFr::StringUtility::SearchMode::Characters), true);
		ASSERT_EQ(NxFr::StringUtility::Find(Test1, "ll"), "llo World");
		ASSERT_EQ(NxFr::StringUtility::Find(Test1, "l", 1, NxFr::StringUtility::SearchMode::Characters).IsEmpty(), false);
		ASSERT_EQ(NxFr::StringUtility::Split(Test1, "ll"), "He");
		ASSERT_EQ(NxFr::StringUtility::Split(Test1, "He"), "");
		ASSERT_EQ(NxFr::StringUtility::Split(Test1, "l", 2, NxFr::StringUtility::SearchMode::Characters).IsEmpty(), false);

		NxFr::List<NxFr::StringView> Tokens = NxFr::StringUtility::Tokenize("Command Args1 \"Args2 Args3\" \"\" Args4");
		ASSERT_EQ(Tokens.GetCount(), 5);

		NxFr::String Test2 = NxFr::String("Hello World 1 Hello World 2 Hello World 3 Hello World 4");
		NxFr::List<NxFr::StringView> Found = NxFr::StringUtility::FindAll(Test2, "Hello");
		ASSERT_EQ(Found.GetCount(), 4);
		NxFr::List<NxFr::StringView> Split = NxFr::StringUtility::SplitAll(Test2, " ");
		ASSERT_EQ(Split.GetCount(), 12);

		NxFr::String Test3 = NxFr::StringUtility::FormatTo("Hello %i World %.1f, %s", 10, 20.0f, Test1.C());
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

		ASSERT_EQ(NxFr::StringUtility::ToString<bool>(true), "true");
		ASSERT_EQ(NxFr::StringUtility::FromString<bool>("True"), true);
		ASSERT_EQ(NxFr::StringUtility::ToString<int8>(100), "100");
		ASSERT_EQ(NxFr::StringUtility::ToString<int16>(100), "100");
		ASSERT_EQ(NxFr::StringUtility::ToString<int32>(100), "100");
		ASSERT_EQ(NxFr::StringUtility::ToString<int64>(100), "100");
		ASSERT_EQ(NxFr::StringUtility::FromString<int8>("100"), 100);
		ASSERT_EQ(NxFr::StringUtility::FromString<int16>("100"), 100);
		ASSERT_EQ(NxFr::StringUtility::FromString<int32>("100"), 100);
		ASSERT_EQ(NxFr::StringUtility::FromString<int64>("100"), 100);
		ASSERT_EQ(NxFr::StringUtility::ToString<uint8>(100), "100");
		ASSERT_EQ(NxFr::StringUtility::ToString<uint16>(100), "100");
		ASSERT_EQ(NxFr::StringUtility::ToString<uint32>(100), "100");
		ASSERT_EQ(NxFr::StringUtility::ToString<uint64>(100), "100");
		ASSERT_EQ(NxFr::StringUtility::FromString<uint8>("100"), 100);
		ASSERT_EQ(NxFr::StringUtility::FromString<uint16>("100"), 100);
		ASSERT_EQ(NxFr::StringUtility::FromString<uint32>("100"), 100);
		ASSERT_EQ(NxFr::StringUtility::FromString<uint64>("100"), 100);
		ASSERT_EQ(NxFr::StringUtility::ToString<float>(100.5f), "100.50");
		ASSERT_EQ(NxFr::StringUtility::FromString<float>("100.50"), 100.5f);
		ASSERT_EQ(NxFr::StringUtility::ToString<double>(100.5f), "100.50");
		ASSERT_EQ(NxFr::StringUtility::FromString<double>("100.50"), 100.5f);
	}

	TEST(Type_String, Operator)
	{
		NxFr::String Test = NxFr::String("Hello World");
		NxFr::String Reverse = NxFr::String("World Hello");

		ASSERT_EQ(Test, NxFr::String("Hello World"));
		ASSERT_EQ(Test, "Hello World");
		ASSERT_EQ(Test > NxFr::String("Hello World"), false);
		ASSERT_EQ(Test <= "Hello World", true);
		ASSERT_EQ(Test < Reverse, true);
		ASSERT_EQ(Test >= Reverse, false);

		NxFr::StringView View = Test.Substring(0, 5);
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

	TEST(Type_String, Misc)
	{
		NxFr::String Data = NxFr::String("This is a test text");
		for (NxFr::Iterator::StringCharacter It = Data.Begin(); It != Data.End(); ++It)
		{
			ASSERT_EQ(It.Get().IsEmpty(), false);
		}

		for (NxFr::Iterator::StringToken It = Data.Begin(" "); It != Data.End(" "); ++It)
		{
			ASSERT_EQ(It.Get().IsEmpty(), false);
		}

		NxFr::Array<NxFr::StringView> Array = NxFr::Array<NxFr::StringView>(4);
		Array.AssignConstruct(0, "World");
		Array.AssignConstruct(1, "World Hello");
		Array.AssignConstruct(2, "Hello World");
		Array.AssignConstruct(3, "Hello");
		NxFr::ContainerUtility::Sort<NxFr::StringView>(Array);
		for (uint64 Index = 1; Index < Array.GetCount(); Index++)
		{
			ASSERT_EQ(Array[Index - 1] <= Array[Index], true);
		}

		ASSERT_EQ(NxFr::Hash<>::HashObject(Data), 0x96B1BC7DBA071D97);
	}
}
