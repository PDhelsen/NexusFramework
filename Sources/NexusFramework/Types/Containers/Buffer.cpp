#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Containers/Buffer.h"

namespace NxFr
{
	Buffer::Buffer(uint64 Size, Allocator* Allctr)
		: Alloc(Allctr), Count(0), Data(nullptr)
	{
		Allocate(Size);
	}

	Buffer::Buffer(const Buffer& Other)
		: Alloc(Other.Alloc), Count(Other.Count), Data(nullptr)
	{
		Allocate(Count);
		Set(Other.Data);
	}

	Buffer::Buffer(Buffer&& Other) noexcept
		: Alloc(Other.Alloc), Count(Other.Count), Data(Other.Data)
	{
		Other.Data = nullptr;
	}

	Buffer::~Buffer()
	{
		Free();
	}

	Buffer& Buffer::operator=(const Buffer& Other)
	{
		if (this == &Other)
		{
			return *this;
		}

		Free();

		Count = Other.Count;

		Allocate(Count);
		Set(Other.Data);

		return *this;
	}

	Buffer& Buffer::operator=(Buffer&& Other) noexcept
	{
		if (this == &Other)
		{
			return *this;
		}

		Free();

		Alloc = Other.Alloc;
		Count = Other.Count;
		Data = Other.Data;

		Other.Count = 0;
		Other.Data = nullptr;

		return *this;
	}

	bool Buffer::operator==(const Buffer& Other) const
	{
		return Count == Other.Count && Data == Other.Data;
	}

	bool Buffer::operator!=(const Buffer& Other) const
	{
		return !(*this == Other);
	}

	bool Buffer::operator>(const Buffer& Other) const
	{
		return Count > Other.Count;
	}

	bool Buffer::operator>=(const Buffer& Other) const
	{
		return Count >= Other.Count;
	}

	bool Buffer::operator<(const Buffer& Other) const
	{
		return Count < Other.Count;
	}

	bool Buffer::operator<=(const Buffer& Other) const
	{
		return Count <= Other.Count;
	}

	void Buffer::Clear()
	{
		Clear(Count, 0);
	}

	BufferView Buffer::Get(uint64 Size, uint64 Offset)
	{
		NEXUS_ASSERT(Size > 0, Default, "Size is 0");
		NEXUS_ASSERT(Size <= Count, Default, "Size is greater than the size of the Buffer");
		NEXUS_ASSERT(Offset + Size <= Count, Default, "Get will overflow");

		return BufferView((Byte*)GetPtr() + Offset, Size);
	}

	void Buffer::Set(const void* Source, uint64 Size, uint64 OffsetBuffer, uint64 OffsetSource)
	{
		NEXUS_ASSERT(Source, Default, "Source is null");
		NEXUS_ASSERT(Size <= Count, Default, "Size is greater than the size of the Buffer");
		NEXUS_ASSERT(OffsetBuffer + Size <= Count, Default, "Set will overflow");

		Size = Size != 0 ? Size : Count;
		Copy(Source, Size, OffsetBuffer, OffsetSource);
	}

	void Buffer::Grow(uint64 Size)
	{
		Reallocate(Count + Size);
	}

	void Buffer::Resize(uint64 Size)
	{
		Reallocate(Size);
	}

	Buffer::I Buffer::GetIterator(uint64 Offset)
	{
		return GetIt(Offset);
	}

	const Buffer::I Buffer::GetIterator(uint64 Offset) const
	{
		return GetIt(Offset);
	}

	Buffer::I Buffer::begin()
	{
		return Begin();
	}

	Buffer::I Buffer::Begin()
	{
		return GetIt(0);
	}

	const Buffer::I Buffer::begin() const
	{
		return Begin();
	}

	const Buffer::I Buffer::Begin() const
	{
		return GetIt(0);
	}

	Buffer::I Buffer::BeginReverse()
	{
		I It = End();
		--It;
		return It;
	}

	const Buffer::I Buffer::BeginReverse() const
	{
		I It = End();
		--It;
		return It;
	}

	Buffer::I Buffer::end()
	{
		return End();
	}

	Buffer::I Buffer::End()
	{
		return GetIt(Count);
	}

	const Buffer::I Buffer::end() const
	{
		return End();
	}

	const Buffer::I Buffer::End() const
	{
		return GetIt(Count);
	}

	Buffer::I Buffer::EndReverse()
	{
		I It = Begin();
		--It;
		return It;
	}

	const Buffer::I Buffer::EndReverse() const
	{
		I It = Begin();
		--It;
		return It;
	}

	void Buffer::Allocate(uint64 Size)
	{
		ValidateCount(Size);
		Data = Memory::Allocate(Count, Alloc);
	}

	void Buffer::Reallocate(uint64 Size)
	{
		ValidateCount(Size);
		Data = Memory::Reallocate(Data, Count, Alloc);
	}

	void Buffer::Free()
	{
		Memory::Free(Data, Alloc);
	}

	void Buffer::Clear(uint64 Size, uint64 Offset)
	{
		Memory::MemSet((Byte*)Data + Offset, 0, Size);
	}

	void Buffer::Copy(const void* Source, uint64 Size, uint64 OffsetBuffer, uint64 OffsetSource)
	{
		Memory::MemCopy((Byte*)Source + OffsetSource, (Byte*)Data + OffsetBuffer, Size);
	}

	void Buffer::ValidateCount(uint64 Size)
	{
		Count = Size > 1 ? Size : 1;
	}

	Buffer::I Buffer::GetIt(uint64 Offset)
	{
		return I(Data, Offset);
	}

	const Buffer::I Buffer::GetIt(uint64 Offset) const
	{
		return I(Data, Offset);
	}

	BufferView::BufferView()
		: Data(nullptr), Count(0)
	{
	}

	BufferView::BufferView(void* Ptr, uint64 Size)
		: Data(Ptr), Count(Size)
	{
	}

	BufferView::BufferView(const Buffer& Other)
		: Data(Other.GetPtr()), Count(Other.GetCount())
	{
	}

	BufferView::BufferView(const BufferView& Other)
		: Data(Other.GetPtr()), Count(Other.GetCount())
	{
	}

	BufferView::I BufferView::GetIterator(uint64 Offset)
	{
		return GetIt(Offset);
	}

	const BufferView::I BufferView::GetIterator(uint64 Offset) const
	{
		return GetIt(Offset);
	}

	BufferView::I BufferView::begin()
	{
		return Begin();
	}

	BufferView::I BufferView::Begin()
	{
		return GetIt(0);
	}

	const BufferView::I BufferView::begin() const
	{
		return Begin();
	}

	const BufferView::I BufferView::Begin() const
	{
		return GetIt(0);
	}

	BufferView::I BufferView::BeginReverse()
	{
		I It = End();
		--It;
		return It;
	}

	const BufferView::I BufferView::BeginReverse() const
	{
		I It = End();
		--It;
		return It;
	}

	BufferView::I BufferView::end()
	{
		return End();
	}

	BufferView::I BufferView::End()
	{
		return GetIt(Count);
	}

	const BufferView::I BufferView::end() const
	{
		return End();
	}

	const BufferView::I BufferView::End() const
	{
		return GetIt(Count);
	}

	BufferView::I BufferView::EndReverse()
	{
		I It = Begin();
		--It;
		return It;
	}

	const BufferView::I BufferView::EndReverse() const
	{
		I It = Begin();
		--It;
		return It;
	}

	BufferView::I BufferView::GetIt(uint64 Offset)
	{
		return I(Data, Offset);
	}

	const BufferView::I BufferView::GetIt(uint64 Offset) const
	{
		return I(Data, Offset);
	}
}
