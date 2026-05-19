#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Misc/Arguments/Arguments.h"

namespace NxFr
{
	Arguments::Arguments()
		: Args(0), Positionals(), Named()
	{
	}

	Arguments::Arguments(const Arguments& Other)
		: Args(0), Positionals(), Named()
	{
		ParseCollectionString(Other.Args);
	}

	Arguments::Arguments(Arguments&& Other) noexcept
		: Args(Move(Other.Args)), Positionals(Move(Other.Positionals)), Named(Move(Other.Named))
	{
	}

	Arguments::~Arguments()
	{
	}

	Arguments& Arguments::operator=(const Arguments& Other)
	{
		if (this == &Other)
		{
			return *this;
		}

		ParseCollectionString(Other.Args);

		return *this;
	}

	Arguments& Arguments::operator=(Arguments&& Other) noexcept
	{
		if (this == &Other)
		{
			return *this;
		}

		Args = Move(Other.Args);
		Positionals = Move(Other.Positionals);
		Named = Move(Other.Named);

		return *this;
	}

	void Arguments::Clear()
	{
		ContainerUtility::Resize<String>(Args, 0);
		Positionals.Clear();
		Named.Clear();
	}

	void Arguments::Print()
	{
		for (uint64 Index = 0; Index < Positionals.GetCount(); ++Index)
		{
			NX_LOG(Info, Default, "Positional %llu : %s", Index, Positionals[Index].C(true));
		}

		for (auto& [Key, Value] : Named)
		{
			NX_LOG(Info, Default, "Named %s : %s", Key.C(true), Value.C(true));
		}
	}

	void Arguments::ParseExe(uint64 ArgC, char* ArgV[])
	{
		uint64 Index = GetCount();
		uint64 Count = ArgC;

		Resize(Count);
		for (uint64 It = 0; It < ArgC; ++It)
		{
			ParseArg(Index++, ArgV[It]);
		}
	}

	void Arguments::ParseCommand(StringView Command)
	{
		uint64 Index = GetCount();
		uint64 Count = StringUtility::SplitAll(Command, Separator).GetCount();

		Resize(Count);
		for (auto It = Command.Begin(Separator); It != Command.End(Separator); ++It)
		{
			ParseArg(Index++, *It);
		}
	}

	void Arguments::ParseCollectionString(const Collection<String>& Items)
	{
		uint64 Index = GetCount();
		uint64 Count = Items.GetCount();

		Resize(Count);
		for (auto It = Items.Begin(); It != Items.End(); ++It)
		{
			ParseArg(Index++, *It);
		}
	}

	void Arguments::ParseCollectionView(const Collection<StringView>& Items)
	{
		uint64 Index = GetCount();
		uint64 Count = Items.GetCount();

		Resize(Count);
		for (auto It = Items.Begin(); It != Items.End(); ++It)
		{
			ParseArg(Index++, *It);
		}
	}

	void Arguments::ParseArg(uint64 Index, StringView Arg)
	{
		Args[Index] = Arg;

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

	void Arguments::Resize(uint64 Grow)
	{
		uint64 Count = GetCount() + Grow;

		ContainerUtility::Resize<String>(Args, Count);
		Positionals.Reserve(Count);
		Named.Reserve(Count);
	}
}
