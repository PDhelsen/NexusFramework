#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Serialization/Rbs.h"

#include "NexusFramework/IO/Stream.h"

namespace NxFr
{
	Buffer RBS::Serialize(const RBS& Data)
	{
		return Data.Data;
	}

	void RBS::SerializeAndSave(const RBS& Data, StringView Path)
	{
		BinaryStream Stream(Path);
		Stream.Open(File::Mode::Write);
		Stream.WriteAll(Serialize(Data));
		Stream.Close();
	}

	RBS RBS::Deserialize(BufferView Data)
	{
		return RBS(Data);
	}

	RBS RBS::LoadAndDeserialize(StringView Path)
	{
		BinaryStream Stream(Path);
		Stream.Open(File::Mode::Read);
		BufferView Data = Stream.ReadAll();
		Stream.Close();

		return Deserialize(Data);
	}

	RBS::RBS()
		: Data(1024), Cursor(0)
	{
	}

	RBS::RBS(BufferView View)
		: Data(View.GetCount()), Cursor(0)
	{
		Data.Set(View.GetPtr());
	}

	RBS::RBS(Buffer&& Data)
		: Data(Move(Data)), Cursor(0)
	{
	}

	const void* RBS::ReadByte(uint64 Size) const
	{
		const void* Ptr = Data.GetPtr(Cursor);
		Cursor += Size;
		return Ptr;
	}

	void RBS::WriteByte(const void* Pointer, uint64 Size)
	{
		Data.Set(Pointer, Size, Cursor);
		Cursor += Size;
	}

	const Iterator::Pointer RBS::Begin() const
	{
		return Data.Begin();
	}

	const Iterator::Pointer RBS::End() const
	{
		return Data.End();
	}
}
