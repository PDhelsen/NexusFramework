#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Serialization/Csv.h"

namespace NxFr
{
	Csv::Csv(NxFr::StringView Path)
		: Handle(Path), Header(128), Data(1024)
	{
	}

	Csv::~Csv()
	{
	}

	void Csv::ReadFile()
	{
		Handle.Open(File::Mode::Read, false);
		Data = Handle.ReadText();
		Handle.Close();

		Header = Data.Split(StringUtility::NewLine).ToString();
		Data.Remove(Header + StringUtility::NewLine);
	}

	void Csv::WriteFile(bool Clear)
	{
		Handle.Open(File::Mode::Write, true);
		Handle.WriteText(Header);
		Handle.WriteText(StringUtility::NewLine);
		Handle.WriteText(Data);
		Handle.Close();

		if (Clear)
		{
			Data.Clear();
		}
	}

	void Csv::AppendFile(bool AppendHeader)
	{
		Handle.Open(File::Mode::Append, true);
		if (AppendHeader)
		{
			Handle.WriteText(Header);
			Handle.WriteText(StringUtility::NewLine);
		}
		Handle.WriteText(Data);
		Handle.Close();

		Data.Clear();
	}

	void Csv::AppendData(StringView Text)
	{
		Data += Text;
	}

	void Csv::AppendSeparator()
	{
		Data += Separator;
	}

	void Csv::AppendNewLine()
	{
		Data += StringUtility::NewLine;
	}

	void Csv::AppendCell(StringView Text)
	{
		AppendData(Text);
		AppendSeparator();
	}

	void Csv::AppendCells(const Collection<StringView>& Text)
	{
		for (auto& It = Text.Reset(); It != Text.End(); ++It)
		{
			AppendCell(*It);
		}
	}

	void Csv::AppendLine(StringView Text)
	{
		AppendData(Text);
		AppendSeparator();
		AppendNewLine();
	}

	void Csv::AppendLines(const Collection<StringView>& Text)
	{
		for (auto& It = Text.Reset(); It != Text.End(); ++It)
		{
			AppendLine(*It);
		}
	}

	StringView Csv::GetHeader() const
	{
		return Header;
	}

	StringView Csv::GetHeader(uint64 Index) const
	{
		return Header.Split(Separator, Index);
	}

	void Csv::SetHeader(StringView Text)
	{
		Header = Text.ToString();
	}

	void Csv::SetHeader(uint64 Index, StringView Text)
	{
		List<StringView> Cells = Header.SplitAll(Separator);
		Cells[Index] = Text;
		Header = StringUtility::Join(Cells, Separator);
	}

	StringView Csv::GetData() const
	{
		return Data;
	}

	StringView Csv::GetData(uint64 Row) const
	{
		return Data.Split(StringUtility::NewLine, Row);
	}

	StringView Csv::GetData(uint64 Row, uint64 Column) const
	{
		return Data.Split(StringUtility::NewLine, Row).Split(Separator, Column);
	}

	void Csv::SetData(StringView Text)
	{
		Data = Text.ToString();
	}

	void Csv::SetData(uint64 Row, StringView Text)
	{
		List<StringView> Lines = Data.SplitAll(StringUtility::NewLine);
		Lines[Row] = Text;
		Data = StringUtility::Join(Lines, StringUtility::NewLine);
	}

	void Csv::SetData(uint64 Row, uint64 Column, StringView Text)
	{
		List<StringView> Lines = Data.SplitAll(StringUtility::NewLine);
		List<StringView> Cells = Lines[Row].SplitAll(Separator);
		Cells[Column] = Text;
		String Temp = StringUtility::Join(Cells, Separator);
		Lines[Row] = Temp;
		Data = StringUtility::Join(Lines, StringUtility::NewLine);
	}

	List<StringView> Csv::GetLines() const
	{
		return Data.SplitAll(StringUtility::NewLine);
	}

	uint64 Csv::GetLinesCount() const
	{
		return GetLines().GetCount();
	}

	List<StringView> Csv::GetCells() const
	{
		List<StringView> Cells = Data.SplitAll(Separator);
		if (Cells.Last() == StringUtility::NewLine)
		{
			Cells.RemoveLast();
		}
		for (uint64 Index = 0; Index < Cells.GetCount(); ++Index)
		{
			if (Cells[Index].Start(StringUtility::NewLine))
			{
				Cells[Index] = Cells[Index].ToView(1, Cells[Index].GetCount() - 1);
			}
			if (Cells[Index].End(StringUtility::NewLine))
			{
				Cells[Index] = Cells[Index].ToView(0, Cells[Index].GetCount() - 1);
			}
		}
		return Cells;
	}

	uint64 Csv::GetCellsCount() const
	{
		return GetCells().GetCount();
	}
}
