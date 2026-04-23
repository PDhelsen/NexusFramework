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

	TEST(Misc, Color)
	{
		NxFr::Color Color1(1, 0, 0);
		NxFr::Color Color2(0, 0, 1);
		
		ASSERT_EQ(NxFr::Colors::Utility::Access(Color2, 2), 1.0f);
		ASSERT_EQ(NxFr::Colors::Utility::Min(Color1), 0.0f);
		ASSERT_EQ(NxFr::Colors::Utility::Min(Color1, Color2), NxFr::Color(0.0f, 0.0f, 0.0f));
		ASSERT_EQ(NxFr::Colors::Utility::Max(Color1), 1.0f);
		ASSERT_EQ(NxFr::Colors::Utility::Max(Color1, Color2), NxFr::Color(1.0f, 0.0f, 1.0f));
		ASSERT_EQ(NxFr::Colors::Utility::Clamp(NxFr::Color(-1.5f, 0.5f, 1.5f), 0.0f, 1.0f), NxFr::Color(0.0f, 0.5f, 1.0f));
		ASSERT_EQ(NxFr::Colors::Utility::Lerp(Color1, Color2, 0.5f), NxFr::Color(0.5f, 0.0f, 0.5f));
		ASSERT_EQ(Color1 + Color2, NxFr::Color(1.0f, 0.0f, 1.0f, 2.0f));
		ASSERT_EQ(Color1 - 1.0f, NxFr::Color(0.0f, -1.0f, -1.0f, 0.0f));

		ASSERT_EQ(NxFr::Colors::Utility::ToVector3f(Color1), NxFr::Vector3f(1, 0, 0));
		ASSERT_EQ(NxFr::Colors::Utility::ToVector4f(Color2), NxFr::Vector4f(0, 0, 1, 1));

		ASSERT_EQ(NxFr::Colors::Linear(0.21403f, 0.21403f, 0.21403f), NxFr::Color(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::Colors::Bits(127, 127, 127), NxFr::Color(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::Colors::Hsv(0.3333f, 1.0f, 1.0f), NxFr::Color(0.0f, 1.0f, 0.0f));
	}
}
