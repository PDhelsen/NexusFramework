#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Collection.h"

namespace NxFr
{
	class NX_FRAMEWORK_API Csv
	{
	public:
		inline static const String Separator = ";";

		static String Serialize(const Csv& Data);
		static void SerializeAndSave(const Csv& Data, StringView Path);
		static Csv Deserialize(StringView Text);
		static Csv LoadAndDeserialize(StringView Path);

		Csv();

		void Clear();

		void AppendHeader(StringView Text);
		void AppendLine();
		void AppendLine(const Collection<StringView>& Text);
		void AppendCell(StringView Text);
		void AppendCells(const Collection<StringView>& Text);

		String& GetHeader(uint64 Column);
		void SetHeader(uint64 Column, StringView Text);
		uint64 GetHeaderCount() const;
		List<String>& GetLine(uint64 Row);
		void SetLine(uint64 Row, const Collection<StringView>& Text);
		uint64 GetLinesCount() const;
		String& GetCell(uint64 Row, uint64 Column);
		void SetCell(uint64 Row, uint64 Column, StringView Text);
		uint64 GetCellsCount() const;

		const List<List<String>>::I Begin() const;
		const List<String>::I Begin(uint64 Row) const;
		const List<List<String>>::I End() const;
		const List<String>::I End(uint64 Row) const;

	private:
		List<List<String>> Data;
	};
}
