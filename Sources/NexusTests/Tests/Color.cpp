#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	TEST(Colors, Color)
	{
		NxFr::Color Color;
		Color.r = 1.0f;
		Color[1] = 0.5f;
		NxFr::Color Color1(1, 0, 0);
		NxFr::Color Color2(0, 0, 1);
		NxFr::Color Lerp = NxFr::Colors::Utility::Lerp(Color1, Color2, 0.5f);
		NxFr::Color Clamp = NxFr::Colors::Utility::Clamp(NxFr::Color(-1.5f, 0.5f, 1.5f), 0.0f, 1.0f);
		NxFr::Color MinC = NxFr::Colors::Utility::Min(Color1, Color2);
		float MinF = NxFr::Colors::Utility::Min(Color1);
		NxFr::Color MaxC = NxFr::Colors::Utility::Max(Color1, Color2);
		float MaxF = NxFr::Colors::Utility::Max(Color1);

		ASSERT_EQ(Color, NxFr::Color(1.0f, 0.5f, 0.0f, 1.0f));
		ASSERT_EQ(Color, NxFr::Vector4f(1.0f, 0.5f, 0.0f, 1.0f));
		ASSERT_EQ(Lerp, NxFr::Color(0.5f, 0.0f, 0.5f));
		ASSERT_EQ(Clamp, NxFr::Color(0.0f, 0.5f, 1.0f));
		ASSERT_EQ(MinC, NxFr::Color(0.0f, 0.0f, 0.0f));
		ASSERT_EQ(MaxC, NxFr::Color(1.0f, 0.0f, 1.0f));
		ASSERT_EQ(MinF, 0.0f);
		ASSERT_EQ(MaxF, 1.0f);

		ASSERT_EQ(NxFr::Colors::Linear(0.21403f, 0.21403f, 0.21403f), NxFr::Color(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::Colors::Bits(127, 127, 127), NxFr::Color(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::Colors::Hsv(0.3333f, 1.0f, 1.0f), NxFr::Color(0.0f, 1.0f, 0.0f));

		ASSERT_EQ(NxFr::Colors::Red, NxFr::Color(1.0f, 0.0f, 0.0f));
	}
}
