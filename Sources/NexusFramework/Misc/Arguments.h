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
	class Arguments
	{
	public:
		inline static const String Separator = " ";
		inline static const String NamedFlag = "--";
		inline static const String FlagValue = "true";
		inline static const String NamedKey = "-";
		inline static const String NamedSeparator = "=";

		NEXUS_FRAMEWORK_API Arguments();
		NEXUS_FRAMEWORK_API Arguments(const Arguments& Other);
		NEXUS_FRAMEWORK_API Arguments(Arguments&& Other) noexcept;
		NEXUS_FRAMEWORK_API ~Arguments();

		NEXUS_FRAMEWORK_API Arguments& operator=(const Arguments& Other);
		NEXUS_FRAMEWORK_API Arguments& operator=(Arguments&& Other) noexcept;

		NEXUS_FRAMEWORK_API void Clear();
		NEXUS_FRAMEWORK_API void Print();

		NEXUS_FRAMEWORK_API void ParseExe(uint64 ArgC, char* ArgV[]);
		NEXUS_FRAMEWORK_API void ParseCommand(StringView Command);
		NEXUS_FRAMEWORK_API void ParseCollectionString(const Collection<String>& Items);
		NEXUS_FRAMEWORK_API void ParseCollectionView(const Collection<StringView>& Items);

		NEXUS_FRAMEWORK_API bool Has(uint64 Index) const { return Positionals.IsValidIndex(Index); }
		NEXUS_FRAMEWORK_API bool Has(StringView Key) const { return Named.TryGet(Key) != nullptr; }
		NEXUS_FRAMEWORK_API StringView Get(uint64 Index, StringView Default = "") const { return Positionals.IsValidIndex(Index) ? Positionals[Index] : Default; }
		NEXUS_FRAMEWORK_API StringView Get(StringView Key, StringView Default = "") const { const StringView* Ptr = Named.TryGet(Key); return Ptr != nullptr ? *Ptr : Default; }

		NEXUS_FRAMEWORK_API const Array<String>& GetArgs() const { return Args; }
		NEXUS_FRAMEWORK_API const List<StringView>& GetPositionals() const { return Positionals; }
		NEXUS_FRAMEWORK_API const Dictionary<StringView, StringView>& GetNamed() const { return Named; }

		NEXUS_FRAMEWORK_API uint64 GetCount() const { return Args.GetCount(); }
		NEXUS_FRAMEWORK_API bool IsEmpty() const { return Args.IsEmpty(); }

	private:
		void ParseArg(uint64 Index, StringView Arg);
		void Resize(uint64 Grow);

	private:
		Array<String> Args;
		List<StringView> Positionals;
		Dictionary<StringView, StringView> Named;
	};
}
