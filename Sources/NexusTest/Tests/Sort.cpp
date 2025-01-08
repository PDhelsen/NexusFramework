#include "Core/NexusTestPch.h"

namespace NxTs
{
	const uint64 ArrayLength = 10;

	bool CompareFunction(const uint64& A, const uint64& B)
	{
		return A >= B;
	}

	uint64* CreateRawArray()
	{
		uint64* Data = new uint64[ArrayLength];

		Data[0] = 5;
		Data[1] = 4;
		Data[2] = 7;
		Data[3] = 1;
		Data[4] = 0;
		Data[5] = 5;
		Data[6] = 6;
		Data[7] = 2;
		Data[8] = 5;
		Data[9] = 3;

		return Data;
	}

	void DestroyRawArray(uint64* Data)
	{
		delete[] Data;
	}

	NxEn::Node::NodeSimple<uint64>* CreateNodes()
	{
		NxEn::Node::NodeSimple<uint64>* Node50 = new NxEn::Node::NodeSimple<uint64>();
		Node50->Value = 5;
		NxEn::Node::NodeSimple<uint64>* Node4 = new NxEn::Node::NodeSimple<uint64>();
		Node4->Value = 4;
		NxEn::Node::NodeSimple<uint64>* Node7 = new NxEn::Node::NodeSimple<uint64>();
		Node7->Value = 7;
		NxEn::Node::NodeSimple<uint64>* Node1 = new NxEn::Node::NodeSimple<uint64>();
		Node1->Value = 1;
		NxEn::Node::NodeSimple<uint64>* Node0 = new NxEn::Node::NodeSimple<uint64>();
		Node0->Value = 0;
		NxEn::Node::NodeSimple<uint64>* Node51 = new NxEn::Node::NodeSimple<uint64>();
		Node51->Value = 5;
		NxEn::Node::NodeSimple<uint64>* Node6 = new NxEn::Node::NodeSimple<uint64>();
		Node6->Value = 6;
		NxEn::Node::NodeSimple<uint64>* Node2 = new NxEn::Node::NodeSimple<uint64>();
		Node2->Value = 2;
		NxEn::Node::NodeSimple<uint64>* Node52 = new NxEn::Node::NodeSimple<uint64>();
		Node52->Value = 5;
		NxEn::Node::NodeSimple<uint64>* Node3 = new NxEn::Node::NodeSimple<uint64>();
		Node3->Value = 3;

		Node50->Next = Node4;
		Node4->Next = Node7;
		Node7->Next = Node1;
		Node1->Next = Node0;
		Node0->Next = Node51;
		Node51->Next = Node6;
		Node6->Next = Node2;
		Node2->Next = Node52;
		Node52->Next = Node3;

		return Node50;
	}

	void DestroyNodes(NxEn::Node::NodeSimple<uint64>* Data)
	{
		if (Data->Next)
			DestroyNodes(Data->Next);

		delete Data;
	}

	NxEn::Array<uint64>& CreateNexusArray()
	{
		NxEn::Array<uint64>* Data = new NxEn::Array<uint64>(ArrayLength);

		(*Data)[0] = 5;
		(*Data)[1] = 4;
		(*Data)[2] = 7;
		(*Data)[3] = 1;
		(*Data)[4] = 0;
		(*Data)[5] = 5;
		(*Data)[6] = 6;
		(*Data)[7] = 2;
		(*Data)[8] = 5;
		(*Data)[9] = 3;

		return *Data;
	}

	void DestroyNexusArray(NxEn::Array<uint64>& Data)
	{
		delete &Data;
	}

	TEST(Sort, MergeSort)
	{
		NxEn::Array<uint64>& Data = CreateNexusArray();
		NxEn::Node::NodeSimple<uint64>* Data2 = CreateNodes();

		NxEn::Sorting::MergeSort::SortIndexBased<uint64>(Data, Data.GetCount());
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}

		NxEn::Sorting::MergeSort::SortIndexBased<uint64>(Data, Data.GetCount(), &CompareFunction);
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			ASSERT_EQ(Data[Index - 1] >= Data[Index], true);
		}

		Data2 = NxEn::Sorting::MergeSort::SortLinkBased<uint64>(Data2);
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			ASSERT_EQ(Data2->Value <= Data2->Next->Value, true);
		}

		DestroyNexusArray(Data);
		DestroyNodes(Data2);
	}

	TEST(Sort, QuickSort)
	{
		uint64* Data = CreateRawArray();

		NxEn::Sorting::QuickSort::SortIndexBased<uint64>(Data, ArrayLength);
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}

		NxEn::Sorting::QuickSort::SortIndexBased<uint64>(Data, ArrayLength, &CompareFunction);
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			ASSERT_EQ(Data[Index - 1] >= Data[Index], true);
		}

		DestroyRawArray(Data);
	}

	TEST(Sort, HeapSort)
	{
		uint64* Data = CreateRawArray();

		NxEn::Sorting::HeapSort::SortIndexBased<uint64>(Data, ArrayLength);
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}

		NxEn::Sorting::HeapSort::SortIndexBased<uint64>(Data, ArrayLength, &CompareFunction);
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			ASSERT_EQ(Data[Index - 1] >= Data[Index], true);
		}

		NxEn::Sorting::HeapSort::Heapify<uint64>(Data, ArrayLength);
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			uint64 Left = 2 * Index + 1;
			if (Left < ArrayLength)
			{
				ASSERT_EQ(Data[Index] >= Data[Left], true);
			}

			uint64 Right = 2 * Index + 2;
			if (Right < ArrayLength)
			{
				ASSERT_EQ(Data[Index] >= Data[Right], true);
			}
		}

		DestroyRawArray(Data);
	}

	TEST(Sort, Sort)
	{
		uint64* Data = CreateRawArray();
		NxEn::Node::NodeSimple<uint64>* Data2 = CreateNodes();

		NxEn::Sort::SortIndexBased<uint64>(Data, ArrayLength);
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			ASSERT_EQ(Data[Index - 1] <= Data[Index], true);
		}

		NxEn::Sort::SortIndexBased<uint64>(Data, ArrayLength, &CompareFunction);
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			ASSERT_EQ(Data[Index - 1] >= Data[Index], true);
		}

		NxEn::Sort::SortLinkBased<uint64>(&Data2);
		for (uint64 Index = 1; Index < ArrayLength; Index++)
		{
			ASSERT_EQ(Data2->Value <= Data2->Next->Value, true);
		}

		DestroyRawArray(Data);
		DestroyNodes(Data2);
	}
}
