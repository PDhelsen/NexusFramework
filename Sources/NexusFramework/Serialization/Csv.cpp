#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Serialization/Csv.h"
#include "NexusFramework/IO/Stream.h"

namespace NxFr
{
	String Csv::Serialize(const Csv& Data)
	{
		String Text = 1024;
		for (auto& Line : Data.Data)
		{
			for (auto& Cell : Line)
			{
				Text.Append(Cell);
				Text.Append(Separator);
			}
			Text.Append(StringUtility::NewLine);
		}
		return Text;
	}

	void Csv::SerializeFile(const Csv& Data, StringView Path)
	{
		TextStream Stream(Path);
		Stream.Open(File::Mode::Write);
		Stream.WriteAll(Serialize(Data));
		Stream.Close();
	}

	Csv Csv::Deserialize(StringView Text)
	{
		Csv Data;
		for (auto Line = Text.Begin(StringUtility::NewLine); Line != Text.End(StringUtility::NewLine); ++Line)
		{
			Data.AppendLine();
			for (auto Cell = Line.Get().Begin(Separator); Cell != Line.Get().End(Separator); ++Cell)
			{
				Data.AppendCell(Cell.Get());
			}
		}

		return Data;
	}

	Csv Csv::DeserializeFile(StringView Path)
	{
		TextStream Stream(Path);
		Stream.Open(File::Mode::Read);
		String Text = Stream.ReadAll();
		Stream.Close();

		return Deserialize(Text);
	}

	Csv::Csv()
		: Data()
	{
	}

	Csv::~Csv()
	{
		Clear();
	}

	void Csv::Clear()
	{
		for (auto D : Data)
		{
			D.Clear();
		}

		Data.Clear();
	}

	void Csv::AppendHeader(StringView Text)
	{
		Data.First().Append(Text);
	}

	void Csv::AppendLine()
	{
		Data.AppendConstruct();
	}

	void Csv::AppendLine(const Collection<StringView>& Text)
	{
		List<String>& Line = Data.AppendConstruct();
		for (auto It = Text.Begin(); It != Text.End(); ++It)
		{
			Line.Append(*It);
		}
	}

	void Csv::AppendCell(StringView Text)
	{
		List<String>& Line = Data.Last();
		Line.Append(Text);
	}

	void Csv::AppendCells(const Collection<StringView>& Text)
	{
		List<String>& Line = Data.Last();
		for (auto It = Text.Begin(); It != Text.End(); ++It)
		{
			Line.Append(*It);
		}
	}

	String& Csv::GetHeader(uint64 Column)
	{
		NEXUS_ASSERT(Data.First().IsValidIndex(Column), Default, "Column %d doesn't exist");

		return Data.First().Get(Column);
	}

	void Csv::SetHeader(uint64 Column, StringView Text)
	{
		NEXUS_ASSERT(Data.First().IsValidIndex(Column), Default, "Column %d doesn't exist");

		Data.First().Get(Column) = Text;
	}

	uint64 Csv::GetHeaderCount() const
	{
		return Data.First().GetCount();
	}

	List<String>& Csv::GetLine(uint64 Row)
	{
		NEXUS_ASSERT(Data.IsValidIndex(Row), Default, "Row %d doesn't exist");

		return Data.Get(Row);
	}

	void Csv::SetLine(uint64 Row, const Collection<StringView>& Text)
	{
		NEXUS_ASSERT(Data.IsValidIndex(Row), Default, "Row %d doesn't exist");

		List<String>& Line = GetLine(Row);
		Line.Clear();
		for (auto It = Text.Begin(); It != Text.End(); ++It)
		{
			Line.Append(*It);
		}
	}

	uint64 Csv::GetLinesCount() const
	{
		return Data.GetCount();
	}

	String& Csv::GetCell(uint64 Row, uint64 Column)
	{
		NEXUS_ASSERT(Data.IsValidIndex(Row), Default, "Row %d doesn't exist");
		NEXUS_ASSERT(Data.Get(Row).IsValidIndex(Column), Default, "Column %d doesn't exist");

		return Data.Get(Row).Get(Column);
	}

	void Csv::SetCell(uint64 Row, uint64 Column, StringView Text)
	{
		NEXUS_ASSERT(Data.IsValidIndex(Row), Default, "Row %d doesn't exist");
		NEXUS_ASSERT(Data.Get(Row).IsValidIndex(Column), Default, "Column %d doesn't exist");

		Data.Get(Row).Get(Column) = Text;
	}

	uint64 Csv::GetCellsCount() const
	{
		return GetHeaderCount() * GetLinesCount();
	}

	const List<List<String>>::I Csv::Begin() const
	{
		return Data.Begin();
	}

	const List<String>::I Csv::Begin(uint64 Row) const
	{
		NEXUS_ASSERT(Data.IsValidIndex(Row), Default, "Row %d doesn't exist");

		return Data.Get(Row).Begin();
	}

	const List<List<String>>::I Csv::End() const
	{
		return Data.End();
	}

	const List<String>::I Csv::End(uint64 Row) const
	{
		NEXUS_ASSERT(Data.IsValidIndex(Row), Default, "Row %d doesn't exist");

		return Data.Get(Row).End();
	}
}
