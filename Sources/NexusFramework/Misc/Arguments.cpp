#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Misc/Arguments.h"

namespace NxFr
{
	namespace Arguments
	{
		static const String Separator = " ";
		static const String NamedFlag = "--";
		static const String FlagValue = "true";
		static const String NamedKey = "-";
		static const String NamedSeparator = "=";

		static Array<String> Arguments;
		static List<String> Positionals;
		static Dictionary<String, String> Named;

		static void Parse(uint64 Index, StringView Arg, Array<String>& Arguments, List<String>& Positionals, Dictionary<String, String>& Named)
		{
			Arguments[Index] = Arg;

			if (StringUtility::Start(Arg, NamedFlag))
			{
				Arg = Arg.Substring(2, Arg.GetCount() - 2);
				Named.Append(Arg, FlagValue);
			}
			else if (StringUtility::Start(Arg, NamedKey))
			{
				uint64 Equal = StringUtility::Find(Arg, NamedSeparator).C() - Arg.C();

				StringView Key = Arg.Substring(1, Equal - 1);
				StringView Value = Arg.Substring(Equal + 1, Arg.GetCount() - (Equal + 1));

				Named.Append(Key, Value);
			}
			else
			{
				Positionals.Append(Arg);
			}
		}

		const Array<String>& GetArguments()
		{
			return Arguments;
		}

		const List<String>& GetPositionals()
		{
			return Positionals;
		}

		const Dictionary<String, String>& GetNamed()
		{
			return Named;
		}

		StringView Get(uint64 Index, StringView Default)
		{
			if (!Positionals.IsValidIndex(Index))
			{
				return Default;
			}

			return Positionals[Index];
		}

		StringView Get(StringView Key, StringView Default)
		{
			String* Contain = Named.TryGet(Key);
			if (Contain == nullptr)
			{
				return Default;
			}

			return *Contain;
		}

		bool Has(uint64 Index)
		{
			return Positionals.IsValidIndex(Index);
		}

		bool Has(StringView Key)
		{
			return ContainersUtils::ContainsKey<String, String>(Named, Key);
		}

		void Parse(uint64 ArgC, char* ArgV[])
		{
			NEXUS_ASSERT(Arguments.GetCount() == 0, Default, "Arguments were already parsed");

			Arguments = Array<String>(ArgC);
			Positionals.Clear();
			Positionals.Reserve(ArgC);
			Named.Clear();
			Named.Reserve(ArgC);

			for (uint64 Index = 0; Index < ArgC; ++Index)
			{
				StringView Arg = ArgV[Index];
				NEXUS_LOG(Info, Default, "Arg %llu : %s", Index, Arg.C());
				Parse(Index, Arg, Arguments, Positionals, Named);
			}
		}

		void Parse(const Collection<StringView>& Args, Array<String>& Arguments, List<String>& Positionals, Dictionary<String, String>& Named)
		{
			uint64 Count = Args.GetCount();

			Arguments = Array<String>(Count);
			Positionals.Clear();
			Positionals.Reserve(Count);
			Named.Clear();
			Named.Reserve(Count);

			uint64 Index = 0;
			for (auto It = Args.Begin(); It != Args.End(); ++It)
			{
				Parse(Index++, *It, Arguments, Positionals, Named);
			}
		}

		void Parse(StringView Command, Array<String>& Arguments, List<String>& Positionals, Dictionary<String, String>& Named)
		{
			uint64 Count = StringUtility::SplitAll(Command, Separator).GetCount();

			Arguments = Array<String>(Count);
			Positionals.Clear();
			Positionals.Reserve(Count);
			Named.Clear();
			Named.Reserve(Count);

			uint64 Index = 0;
			for (auto It = Command.Begin(Separator); It != Command.End(Separator); ++It)
			{
				Parse(Index++, *It, Arguments, Positionals, Named);
			}
		}
	}
}
