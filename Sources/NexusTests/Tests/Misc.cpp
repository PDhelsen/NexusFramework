#include "NexusTests/Core/NexusTests.h"
#include "NexusTests/Core/NexusTestsDummy.h"

namespace NxTs
{
	TEST(Misc, Arguments)
	{
		NxFr::StringView Command = "Positional1 Positional2 --Flag1 -Key1=Value1 \"Positional3 bis\" --Flag2 -Key2=Value2 Positional4";

		NxFr::Arguments Args;
		Args.ParseCommand(Command);

		ASSERT_EQ(Args.GetArgs().GetCount(), 8);
		ASSERT_EQ(Args.GetPositionals().GetCount(), 4);
		ASSERT_EQ(Args.GetNamed().GetCount(), 4);
	}

	TEST(Misc, Color)
	{
		NxFr::Color Color1(1, 0, 0);
		NxFr::Color Color2(0, 0, 1);
		
		ASSERT_EQ(NxFr::ColorUtility::Access(Color2, 2), 1.0f);
		ASSERT_EQ(NxFr::ColorUtility::Min(Color1), 0.0f);
		ASSERT_EQ(NxFr::ColorUtility::Min(Color1, Color2), NxFr::Color(0.0f, 0.0f, 0.0f));
		ASSERT_EQ(NxFr::ColorUtility::Max(Color1), 1.0f);
		ASSERT_EQ(NxFr::ColorUtility::Max(Color1, Color2), NxFr::Color(1.0f, 0.0f, 1.0f));
		ASSERT_EQ(NxFr::ColorUtility::Clamp(NxFr::Color(-1.5f, 0.5f, 1.5f), 0.0f, 1.0f), NxFr::Color(0.0f, 0.5f, 1.0f));
		ASSERT_EQ(NxFr::ColorUtility::Lerp(Color1, Color2, 0.5f), NxFr::Color(0.5f, 0.0f, 0.5f));
		ASSERT_EQ(Color1 + Color2, NxFr::Color(1.0f, 0.0f, 1.0f, 2.0f));
		ASSERT_EQ(Color1 - 1.0f, NxFr::Color(0.0f, -1.0f, -1.0f, 0.0f));

		ASSERT_EQ(NxFr::ColorUtility::ToVector3f(Color1), NxFr::Vector3f(1, 0, 0));
		ASSERT_EQ(NxFr::ColorUtility::ToVector4f(Color2), NxFr::Vector4f(0, 0, 1, 1));

		ASSERT_EQ(NxFr::Colors::Linear(0.21403f, 0.21403f, 0.21403f), NxFr::Color(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::Colors::Bits(127, 127, 127), NxFr::Color(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::Colors::Hsv(0.3333f, 1.0f, 1.0f), NxFr::Color(0.0f, 1.0f, 0.0f));
	}

	TEST(Misc, Hash)
	{
		ASSERT_EQ(NxFr::Hash<NxFr::Hashing::XxHash32>::HashObject(Dummy::GetText()), 0x237D78AE);
		ASSERT_EQ(NxFr::Hash<NxFr::Hashing::XxHash64>::HashObject(Dummy::GetText()), 0xDF598B12B511E2BF);
		ASSERT_EQ(NxFr::Hash<NxFr::Hashing::Murmur32>::HashObject(Dummy::GetText()), 0xA497CA32);
		ASSERT_EQ(NxFr::Hash<NxFr::Hashing::Fnv164>::HashObject(Dummy::GetText()), 0x96F716A541384708);
		ASSERT_EQ(NxFr::Hash<NxFr::Hashing::Fnv1a64>::HashObject(Dummy::GetText()), 0xF2BA71E0791C1F5A);
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

		NxFr::LinkedList<uint64> LinkedSort = { 9, 6, 4, 8, 3, 1, 2, 7, 0, 5 };
		NxFr::ContainerUtility::Sort(LinkedSort);
		NxFr::LinkedList<uint64>::N* Node = NxFr::Node::GetNode<uint64, NxFr::LinkedList<uint64>::N>(&LinkedSort.First());
		while (Node && Node->Next)
		{
			ASSERT_EQ(Node->Value < Node->Next->Value, true);
			Node = Node->Next;
		}
	}

	TEST(Misc, Pattern)
	{
		Dummy Test;

		{
			NxFr::Context<Dummy> Contexts;
			NxFr::Context<Dummy>::Reference _ = Contexts.PushReference(&Test);
			ASSERT_EQ(Contexts.GetCount(), 1);
			ASSERT_EQ(Contexts.TryGet(), &Test);
		}

		NxFr::Registry<Dummy*> Registry;
		Registry.Register(Dummy::GetId(), &Test);
		ASSERT_EQ(Registry.GetCount(), 1);
		ASSERT_EQ(Registry.TryGet(Dummy::GetId()), &Test);

		NxFr::Factory<Dummy> Factory;
		Factory.Register<Dummy>(Dummy::GetId());
		Dummy* Instance = Factory.Create(Dummy::GetId());
		ASSERT_EQ(Factory.GetCount(), 1);
		ASSERT_EQ(Instance->Value, Test.Value);
		delete Instance;
	}
}
