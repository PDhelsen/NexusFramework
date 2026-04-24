#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	TEST(Misc, Arguments)
	{
		NxFr::StringView Command = "Positional1 Positional2 --Flag1 -Key1=Value1 Positional3 --Flag2 -Key2=Value2 Positional4";

		NxFr::Array<NxFr::String> Arguments;
		NxFr::List<NxFr::String> Positionals;
		NxFr::Dictionary<NxFr::String, NxFr::String> Named;

		NxFr::Arguments::Parse(Command, Arguments, Positionals, Named);

		ASSERT_EQ(Arguments.GetCount(), 8);
		ASSERT_EQ(Positionals.GetCount(), 4);
		ASSERT_EQ(Named.GetCount(), 4);
	}

	TEST(Misc, Color)
	{
		NxFr::Color Color1(1, 0, 0);
		NxFr::Color Color2(0, 0, 1);
		
		ASSERT_EQ(NxFr::Colors::Utility::Access(Color2, 2), 1.0f);
		ASSERT_EQ(NxFr::Colors::Utility::Min(Color1), 0.0f);
		ASSERT_EQ(NxFr::Colors::Utility::Min(Color1, Color2), NxFr::Color(0.0f, 0.0f, 0.0f));
		ASSERT_EQ(NxFr::Colors::Utility::Max(Color1), 1.0f);
		ASSERT_EQ(NxFr::Colors::Utility::Max(Color1, Color2), NxFr::Color(1.0f, 0.0f, 1.0f));
		ASSERT_EQ(NxFr::Colors::Utility::Clamp(NxFr::Color(-1.5f, 0.5f, 1.5f), 0.0f, 1.0f), NxFr::Color(0.0f, 0.5f, 1.0f));
		ASSERT_EQ(NxFr::Colors::Utility::Lerp(Color1, Color2, 0.5f), NxFr::Color(0.5f, 0.0f, 0.5f));
		ASSERT_EQ(Color1 + Color2, NxFr::Color(1.0f, 0.0f, 1.0f, 2.0f));
		ASSERT_EQ(Color1 - 1.0f, NxFr::Color(0.0f, -1.0f, -1.0f, 0.0f));

		ASSERT_EQ(NxFr::Colors::Utility::ToVector3f(Color1), NxFr::Vector3f(1, 0, 0));
		ASSERT_EQ(NxFr::Colors::Utility::ToVector4f(Color2), NxFr::Vector4f(0, 0, 1, 1));

		ASSERT_EQ(NxFr::Colors::Linear(0.21403f, 0.21403f, 0.21403f), NxFr::Color(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::Colors::Bits(127, 127, 127), NxFr::Color(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::Colors::Hsv(0.3333f, 1.0f, 1.0f), NxFr::Color(0.0f, 1.0f, 0.0f));
	}

	TEST(Misc, Hash)
	{
		const NxFr::String Text = "This is a test text for testing the Hash function";

		ASSERT_EQ(NxFr::Hash<NxFr::Hashing::XxHash32>::HashObject(Text), 0x3ba1b2d6);
		ASSERT_EQ(NxFr::Hash<NxFr::Hashing::XxHash64>::HashObject(Text), 0xc5eeeb390253f739);
		ASSERT_EQ(NxFr::Hash<NxFr::Hashing::Murmur32>::HashObject(Text), 0x1da26dd2);
		ASSERT_EQ(NxFr::Hash<NxFr::Hashing::Fnv164>::HashObject(Text), 0x0DB54C1C1610EA87);
		ASSERT_EQ(NxFr::Hash<NxFr::Hashing::Fnv1a64>::HashObject(Text), 0x77F122B9F752AACB);
	}

	TEST(Misc, Sort)
	{
		auto Comparaison = [](const uint64& A, const uint64& B)
		{
			return A >= B;
		};

		NxFr::Array<uint64> MergeSort = { 9, 6, 4, 8, 3, 1, 2, 7, 0, 5 };
		NxFr::Sort::SortCollection<uint64, NxFr::Array<uint64>, NxFr::Sorting::MergeSortIndexed<uint64>>(MergeSort, MergeSort.GetCount());
		for (uint64 Index = 1; Index < MergeSort.GetCount(); Index++)
		{
			ASSERT_EQ(MergeSort[Index - 1] <= MergeSort[Index], true);
		}

		NxFr::Array<uint64> QuickSort = { 9, 6, 4, 8, 3, 1, 2, 7, 0, 5 };
		NxFr::Sort::SortCollection<uint64, NxFr::Array<uint64>, NxFr::Sorting::QuickSort<uint64>>(QuickSort, QuickSort.GetCount(), Comparaison);
		for (uint64 Index = 1; Index < QuickSort.GetCount(); Index++)
		{
			ASSERT_EQ(QuickSort[Index - 1] >= QuickSort[Index], true);
		}

		NxFr::Array<uint64> HeapSort = { 9, 6, 4, 8, 3, 1, 2, 7, 0, 5 };
		NxFr::Sort::SortCollection<uint64, NxFr::Array<uint64>, NxFr::Sorting::HeapSort<uint64>>(HeapSort, HeapSort.GetCount(), Comparaison);
		for (uint64 Index = 1; Index < HeapSort.GetCount(); Index++)
		{
			ASSERT_EQ(HeapSort[Index - 1] >= HeapSort[Index], true);
		}

		NxFr::Node::NodeSimple<uint64> Node1; Node1.Value = 1;
		NxFr::Node::NodeSimple<uint64> Node2; Node2.Value = 2;
		NxFr::Node::NodeSimple<uint64> Node3; Node3.Value = 3;
		NxFr::Node::NodeSimple<uint64> Node4; Node4.Value = 4;
		NxFr::Node::NodeSimple<uint64> Node5; Node5.Value = 5;
		Node1.Next = &Node5;
		Node5.Next = &Node3;
		Node3.Next = &Node2;
		Node2.Next = &Node4;
		Node4.Next = nullptr;

		NxFr::Sorting::MergeSortLinked<uint64, NxFr::Node::NodeSimple<uint64>> MergeSortLinked(
			[](NxFr::Node::NodeSimple<uint64>* Node, NxFr::Node::NodeSimple<uint64>* Value) { Node->Next = Value; },
			[](NxFr::Node::NodeSimple<uint64>* Node) {return Node->Next; },
			[](NxFr::Node::NodeSimple<uint64>* Node) {return Node->Value; }
		);
		NxFr::Node::NodeSimple<uint64>* Node = MergeSortLinked.Sort(&Node1);
		while (Node && Node->Next)
		{
			ASSERT_EQ(Node->Value < Node->Next->Value, true);
			Node = Node->Next;
		}
	}
}
