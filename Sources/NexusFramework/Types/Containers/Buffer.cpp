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
		if (*this == Other)
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
		if (*this == Other)
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
}
