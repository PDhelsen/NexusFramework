#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	TEST(Misc, Arguments)
	{
		NxFr::StringView Command = "Positional1 Positional2 --Flag1 -Key1=Value1 Positional3 --Flag2 -Key2=Value2 Positional4";

		NxFr::Array<NxFr::String> Arguments;
		NxFr::List<NxFr::String> Positionals;
		NxFr::Dictionary<NxFr::String, NxFr::String> Named;

		NxFr::Arguments::Parse(Command, Arguments, Positionals, Named);

		ASSERT_EQ(Arguments.GetCount(), 8);
		ASSERT_EQ(Positionals.GetCount(), 4);
		ASSERT_EQ(Named.GetCount(), 4);
	}
}
