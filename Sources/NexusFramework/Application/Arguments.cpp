#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Application/Arguments.h"

namespace NxFr
{
	namespace Arguments
	{
		static List<StringView>& GetArgs() { static List<StringView> Args(1, nullptr); return Args; }
		static Dictionary<StringView, StringView>& GetArgsProcessed() { static Dictionary<StringView, StringView> Args(1, nullptr); return Args; }

		void Parse(uint64 ArgC, char* ArgV[])
		{
			List<StringView>& Args = GetArgs();
			Dictionary<StringView, StringView>& ArgsProcessed = GetArgsProcessed();
			NEXUS_ASSERT(Args.GetCount() == 0, Default, "Arguments were already parsed");

			Args.Grow(ArgC);
			ArgsProcessed.Grow(Math::NextPrime(ArgC));

			Args.Append(ArgV[0]);
			ArgsProcessed.Append(KeyProgram, ArgV[0]);

			for (uint64 Index = 1; Index < ArgC; ++Index)
			{
				StringView Arg = ArgV[Index];

				List<StringView> KeyValue = Arg.SplitAll("=");
				StringView Key = KeyValue[0];
				StringView Value = KeyValue.GetCount() == 2 ? KeyValue[1] : "";

				Args.Append(ArgV[Index]);
				ArgsProcessed.Append(Key, Value);
			}
		}

		void Log()
		{
			Dictionary<StringView, StringView>& Args = GetArgsProcessed();
			NEXUS_LOG(Info, Default, "Arguments count: %d", Args.GetCount());

			uint64 Index = 0;
			for (auto Iter = Begin(); Iter != End(); ++Iter, ++Index)
			{
				NEXUS_LOG(Info, Default, "Argument %d: %s = %s", Index, Iter->Key.ToString().C(), Iter->Value.ToString().C());
			}
		}

		StringView GetValue(uint64 Index, StringView Default)
		{
			List<StringView>& Args = GetArgs();
			return Args[Index];
		}

		StringView GetValue(StringView Key, StringView Default)
		{
			Dictionary<StringView, StringView>& Args = GetArgsProcessed();
			StringView* Contain = Args.TryGet(Key);
			return Contain != nullptr ? *Contain : Default;
		}

		bool HasFlag(StringView Key)
		{
			Dictionary<StringView, StringView>& Args = GetArgsProcessed();
			return Args.ContainsKey(Key);
		}

		Dictionary<StringView, StringView>::I Begin()
		{
			return GetArgsProcessed().Begin();
		}

		Dictionary<StringView, StringView>::I End()
		{
			return GetArgsProcessed().End();
		}

		int64 GetCount()
		{
			return GetArgsProcessed().GetCount();
		}
	}
}
