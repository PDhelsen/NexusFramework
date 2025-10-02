#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Application/Arguments.h"

namespace NxFr
{
	namespace Arguments
	{
		static String FlagTrue = "true";

		List<String>& GetPositional() { static List<String> Args(1, nullptr); return Args; }
		Dictionary<String, String>& GetNamed() { static Dictionary<String, String> Args(1, nullptr); return Args; }

		void Parse(uint64 ArgC, char* ArgV[])
		{
			List<String>& Positional = GetPositional();
			Dictionary<String, String>& Named = GetNamed();
			NEXUS_ASSERT(Positional.GetCount() == 0 && Named.GetCount() == 0, Default, "Arguments were already parsed");

			Positional.Grow(ArgC);
			Named.Grow(ArgC);

			for (uint64 Index = 0; Index < ArgC; ++Index)
			{
				StringView Arg = ArgV[Index];

				if (Arg.Start("--"))
				{
					Arg = Arg.ToView(2, Arg.GetCount() - 2);
					Named.Append(Arg.ToString(), FlagTrue);
				}
				else if (Arg.Start("-"))
				{
					uint64 Equal = Arg.Find("=").C() - Arg.C();

					StringView Key = Arg.ToView(1, Equal - 1);
					StringView Value = Arg.ToView(Equal + 1, Arg.GetCount() - (Equal + 1));

					Named.Append(Key.ToString(), Value.ToString());
				}
				else
				{
					Positional.Append(Arg.ToString());
				}
			}
		}

		void Log()
		{
			List<String>& Positional = GetPositional();
			Dictionary<String, String>& Named = GetNamed();
			NEXUS_LOG(Info, Default, "Arguments count: %d", Positional.GetCount() + Named.GetCount());

			for (uint64 Index = 0; Index < Positional.GetCount(); ++Index)
			{
				NEXUS_LOG(Info, Default, "Argument %d: %s", Index, Positional[Index].C());
			}
			for (auto It = Named.Begin(); It != Named.End(); ++It)
			{
				if (It->Value.IsEmpty())
				{
					NEXUS_LOG(Info, Default, "Argument %s: true", It->Key.C());
				}
				else
				{
					NEXUS_LOG(Info, Default, "Argument %s: %s", It->Key.C(), It->Value.C());
				}
			}
		}

		StringView Get(uint64 Index, StringView Default)
		{
			const List<String>& Args = GetPositional();
			return Args[Index];
		}

		StringView Get(StringView Key, StringView Default)
		{
			const Dictionary<String, String>& Args = GetNamed();
			const String* Contain = Args.TryGet(Key);
			return Contain != nullptr ? *Contain : Default;
		}

		bool Has(uint64 Index)
		{
			const List<String>& Args = GetPositional();
			return Args.IsValidIndex(Index);
		}

		bool Has(StringView Key)
		{
			const Dictionary<String, String>& Args = GetNamed();
			return Args.ContainsKey(Key);
		}
	}
}
