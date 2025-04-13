#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/IO/Stream.h"

namespace NxFr
{
	namespace Streaming
	{
		Line::Line(StringView Path)
			: StreamingAlgorithm<Block>(Path), Buffer(1024), Cursor()
		{
		}

		Line::~Line()
		{
			
		}

		void Line::Cache()
		{
			Buffer.Clear();
			Buffer = Handle.ReadText();
			Cursor = 0;
		}

		void Line::Flush()
		{
			Handle.WriteText(Buffer);
			Buffer.Clear();
		}

		Line::Block Line::Read()
		{
			StringView Substring = Buffer.ToView(Cursor, Buffer.GetCount() - Cursor);
			Substring = Substring.Split("\n");
			Cursor = Math::Min(Cursor + Substring.GetCount() + 1, Buffer.GetCount());
			return Substring;
		}

		void Line::Write(Line::Block Data)
		{
			Buffer.Append(Data);
			Buffer.Append("\n");
			Flush();
		}
	}
}


