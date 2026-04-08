#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Containers/Collection.h"

namespace NxFr
{
	class Csv
	{
	public:
		NEXUS_FRAMEWORK_API inline static const String Separator = ";";

		NEXUS_FRAMEWORK_API static String Serialize(const Csv& Data);
		NEXUS_FRAMEWORK_API static void SerializeFile(const Csv& Data, StringView Path);
		NEXUS_FRAMEWORK_API static Csv Deserialize(StringView Text);
		NEXUS_FRAMEWORK_API static Csv DeserializeFile(StringView Path);

		NEXUS_FRAMEWORK_API Csv();
		NEXUS_FRAMEWORK_API ~Csv();
		NEXUS_FRAMEWORK_API void Clear();

		NEXUS_FRAMEWORK_API void AppendHeader(StringView Text);
		NEXUS_FRAMEWORK_API void AppendLine();
		NEXUS_FRAMEWORK_API void AppendLine(const Collection<StringView>& Text);
		NEXUS_FRAMEWORK_API void AppendCell(StringView Text);
		NEXUS_FRAMEWORK_API void AppendCells(const Collection<StringView>& Text);

		NEXUS_FRAMEWORK_API String& GetHeader(uint64 Column);
		NEXUS_FRAMEWORK_API void SetHeader(uint64 Column, StringView Text);
		NEXUS_FRAMEWORK_API uint64 GetHeaderCount() const;
		NEXUS_FRAMEWORK_API List<String>& GetLine(uint64 Row);
		NEXUS_FRAMEWORK_API void SetLine(uint64 Row, const Collection<StringView>& Text);
		NEXUS_FRAMEWORK_API uint64 GetLinesCount() const;
		NEXUS_FRAMEWORK_API String& GetCell(uint64 Row, uint64 Column);
		NEXUS_FRAMEWORK_API void SetCell(uint64 Row, uint64 Column, StringView Text);
		NEXUS_FRAMEWORK_API uint64 GetCellsCount() const;

		NEXUS_FRAMEWORK_API const List<List<String>>::I Begin() const;
		NEXUS_FRAMEWORK_API const List<String>::I Begin(uint64 Row) const;
		NEXUS_FRAMEWORK_API const List<List<String>>::I End() const;
		NEXUS_FRAMEWORK_API const List<String>::I End(uint64 Row) const;

	private:
		List<List<String>> Data;
	};
}
