#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Containers/Collection.h"

namespace NxFr
{
	class NX_FRAMEWORK_API Arguments
	{
	public:
		inline static const String Separator = " ";
		inline static const String NamedFlag = "--";
		inline static const String FlagValue = "true";
		inline static const String NamedKey = "-";
		inline static const String NamedSeparator = "=";

		Arguments();
		Arguments(const Arguments& Other);
		Arguments(Arguments&& Other) noexcept;
		~Arguments();

		Arguments& operator=(const Arguments& Other);
		Arguments& operator=(Arguments&& Other) noexcept;

		void Clear();
		void Print();

		void ParseExe(uint64 ArgC, char* ArgV[]);
		void ParseCommand(StringView Command);
		void ParseCollectionString(const Collection<String>& Items);
		void ParseCollectionView(const Collection<StringView>& Items);

		bool Has(uint64 Index) const { return Positionals.IsValidIndex(Index); }
		bool Has(StringView Key) const { return Named.TryGet(Key) != nullptr; }
		StringView Get(uint64 Index, StringView Default = "") const { return Positionals.IsValidIndex(Index) ? Positionals[Index] : Default; }
		StringView Get(StringView Key, StringView Default = "") const { const StringView* Ptr = Named.TryGet(Key); return Ptr != nullptr ? *Ptr : Default; }

		const Array<String>& GetArgs() const { return Args; }
		const List<StringView>& GetPositionals() const { return Positionals; }
		const Dictionary<StringView, StringView>& GetNamed() const { return Named; }

		uint64 GetCount() const { return Args.GetCount(); }
		bool IsEmpty() const { return Args.IsEmpty(); }

	private:
		void ParseArg(uint64 Index, StringView Arg);
		void Resize(uint64 Grow);

		Array<String> Args;
		List<StringView> Positionals;
		Dictionary<StringView, StringView> Named;
	};
}
