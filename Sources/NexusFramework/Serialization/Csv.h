#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Containers/Collection.h"
#include "NexusFramework/IO/File.h"

namespace NxFr
{
	class Csv
	{
	public:
		NEXUS_FRAMEWORK_API inline static const String Separator = ";";

		NEXUS_FRAMEWORK_API Csv(NxFr::StringView Path);
		NEXUS_FRAMEWORK_API ~Csv();

		NEXUS_FRAMEWORK_API void ReadFile();
		NEXUS_FRAMEWORK_API void WriteFile(bool Clear = false);
		NEXUS_FRAMEWORK_API void AppendFile(bool AppendHeader = false);

		NEXUS_FRAMEWORK_API void AppendData(StringView Text);
		NEXUS_FRAMEWORK_API void AppendSeparator();
		NEXUS_FRAMEWORK_API void AppendNewLine();

		NEXUS_FRAMEWORK_API void AppendCell(StringView Text);
		NEXUS_FRAMEWORK_API void AppendCells(const Collection<StringView>& Text);
		NEXUS_FRAMEWORK_API void AppendLine(StringView Text);
		NEXUS_FRAMEWORK_API void AppendLines(const Collection<StringView>& Text);

		NEXUS_FRAMEWORK_API StringView GetHeader() const;
		NEXUS_FRAMEWORK_API StringView GetHeader(uint64 Index) const;
		NEXUS_FRAMEWORK_API void SetHeader(StringView Text);
		NEXUS_FRAMEWORK_API void SetHeader(uint64 Index, StringView Text);
		NEXUS_FRAMEWORK_API StringView GetData() const;
		NEXUS_FRAMEWORK_API StringView GetData(uint64 Row) const;
		NEXUS_FRAMEWORK_API StringView GetData(uint64 Row, uint64 Column) const;
		NEXUS_FRAMEWORK_API void SetData(StringView Text);
		NEXUS_FRAMEWORK_API void SetData(uint64 Row, StringView Text);
		NEXUS_FRAMEWORK_API void SetData(uint64 Row, uint64 Column, StringView Text);

		NEXUS_FRAMEWORK_API List<StringView> GetLines() const;
		NEXUS_FRAMEWORK_API uint64 GetLinesCount() const;
		NEXUS_FRAMEWORK_API List<StringView> GetCells() const;
		NEXUS_FRAMEWORK_API uint64 GetCellsCount() const;

	private:
		NxFr::File Handle;
		NxFr::String Header;
		NxFr::String Data;
	};
}
