#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Application/Arguments.h"

namespace NxFr
{
	namespace Arguments
	{
		static Dictionary<StringView, StringView>& GetArgs() { static Dictionary<StringView, StringView> Args(1, nullptr); return Args; }
		static const char* Program = "Program";

		void Parse(uint64 ArgC, char* ArgV[])
		{
			Dictionary<StringView, StringView>& Args = GetArgs();
			NEXUS_ASSERT(Args.GetCount() == 0, Default, "Arguments were already parsed");

			Args.Grow(Math::NextPrime(ArgC));

			Args.Append(Program, ArgV[0]);
			for (uint64 Index = 1; Index < ArgC; ++Index)
			{
				StringView Arg = ArgV[Index];

				List<StringView> KeyValue = Arg.SplitAll("=");
				StringView Key = KeyValue[0];
				StringView Value = KeyValue.GetCount() == 2 ? KeyValue[1] : "";

				Args.Append(Key, Value);
			}
		}

		bool HasFlag(StringView Key, bool Default)
		{
			Dictionary<StringView, StringView>& Args = GetArgs();
			bool Contain = Args.ContainsKey(Key);
			return Contain ? true : Default;
		}

		StringView GetValue(StringView Key, StringView Default)
		{
			Dictionary<StringView, StringView>& Args = GetArgs();
			StringView* Contain = Args.TryGet(Key);
			return Contain != nullptr ? *Contain: Default;
		}

		int64 GetCount()
		{
			return GetArgs().GetCount();
		}

		StringView GetProgram()
		{
			return GetArgs()[Program];
		}

		void Log()
		{
			Dictionary<StringView, StringView>& Args = GetArgs();
			NEXUS_LOG(Info, Default, "Arguments count: %d", Args.GetCount());

			uint64 Index = 0;
			for (auto& [Key, Value] : Args)
			{
				if (Value == "")
				{
					NEXUS_LOG(Info, Default, "Argument %d: %s", Index, Key.C());
				}
				else
				{
					NEXUS_LOG(Info, Default, "Argument %d: %s = %s", Index, Key.C(), Value.C());
				}
				Index++;
			}
		}
	}
}
