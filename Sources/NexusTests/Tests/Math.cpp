#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	TEST(Math, Modulo)
	{
		ASSERT_EQ(NxFr::Math::Modulo(0, 4), 0);
		ASSERT_EQ(NxFr::Math::Modulo(1, 4), 1);
		ASSERT_EQ(NxFr::Math::Modulo(2, 4), 2);
		ASSERT_EQ(NxFr::Math::Modulo(3, 4), 3);
		ASSERT_EQ(NxFr::Math::Modulo(4, 4), 0);
		ASSERT_EQ(NxFr::Math::Modulo(5, 4), 1);
		ASSERT_EQ(NxFr::Math::Modulo(6, 4), 2);

		ASSERT_EQ(NxFr::Math::Modulo(-0, 4), 0);
		ASSERT_EQ(NxFr::Math::Modulo(-1, 4), 3);
		ASSERT_EQ(NxFr::Math::Modulo(-2, 4), 2);
		ASSERT_EQ(NxFr::Math::Modulo(-3, 4), 1);
		ASSERT_EQ(NxFr::Math::Modulo(-4, 4), 0);
		ASSERT_EQ(NxFr::Math::Modulo(-5, 4), 3);
		ASSERT_EQ(NxFr::Math::Modulo(-6, 4), 2);
	}

	TEST(Math, IsMultiple)
	{
		ASSERT_EQ(NxFr::Math::IsMultiple(12, 2), true);
		ASSERT_EQ(NxFr::Math::IsMultiple(12, 6), true);
		ASSERT_EQ(NxFr::Math::IsMultiple(12, 3), true);
		ASSERT_EQ(NxFr::Math::IsMultiple(12, 4), true);
		ASSERT_EQ(NxFr::Math::IsMultiple(12, 1), true);
		ASSERT_EQ(NxFr::Math::IsMultiple(12, 5), false);
		ASSERT_EQ(NxFr::Math::IsMultiple(12, 7), false);
		ASSERT_EQ(NxFr::Math::IsMultiple(12, 8), false);

		ASSERT_EQ(NxFr::Math::IsMultiple(-12, 2), true);
		ASSERT_EQ(NxFr::Math::IsMultiple(-12, -6), true);
		ASSERT_EQ(NxFr::Math::IsMultiple(-12, 3), true);
		ASSERT_EQ(NxFr::Math::IsMultiple(-12, -4), true);
		ASSERT_EQ(NxFr::Math::IsMultiple(-12, 1), true);
		ASSERT_EQ(NxFr::Math::IsMultiple(-12, 5), false);
		ASSERT_EQ(NxFr::Math::IsMultiple(-12, -7), false);
		ASSERT_EQ(NxFr::Math::IsMultiple(-12, 8), false);
	}

	TEST(Math, PowerOfTwo)
	{
		ASSERT_EQ(NxFr::Math::IsPowerOfTwo((uint32)16), true);
		ASSERT_EQ(NxFr::Math::IsPowerOfTwo((uint32)15), false);

		ASSERT_EQ(NxFr::Math::NextPowerOfTwo((uint32)5), 8);
		ASSERT_EQ(NxFr::Math::NextPowerOfTwo((uint32)48), 64);
		ASSERT_EQ(NxFr::Math::NextPowerOfTwo((uint32)4), 4);

		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo((uint32)16), 4);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo((uint32)8), 3);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo((uint32)1), 0);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo((uint32)0), -1);
	}

	TEST(Math, Prime)
	{
		ASSERT_EQ(NxFr::Math::IsPrime((uint32)99), false);
		ASSERT_EQ(NxFr::Math::IsPrime((uint32)97), true);

		ASSERT_EQ(NxFr::Math::NextPrime((uint32)74), 79);
		ASSERT_EQ(NxFr::Math::NextPrime((uint32)84), 89);
	}

	TEST(Math, Vector)
	{
		ASSERT_EQ(NxFr::Vec2f(), NxFr::Vec2f(0, 0));
		ASSERT_EQ(NxFr::Vec2f(1), NxFr::Vec2f(1, 1));
		ASSERT_EQ(NxFr::Vec2f(2, 3), NxFr::Vec2f(2, 3));
		ASSERT_EQ(NxFr::Vec2f(NxFr::Vec3f(1, 2, 3)), NxFr::Vec2f(1, 2));
		ASSERT_EQ(NxFr::Vec2f(NxFr::Vec4f(1, 2, 3, 4)), NxFr::Vec2f(1, 2));
		NxFr::Vec2f Vec2a;
		Vec2a = NxFr::Vec2f(1);
		ASSERT_EQ(Vec2a, NxFr::Vec2f(1, 1));
		Vec2a = NxFr::Vec3f(1);
		ASSERT_EQ(Vec2a, NxFr::Vec2f(1, 1));
		Vec2a = NxFr::Vec4f(1);
		ASSERT_EQ(Vec2a, NxFr::Vec2f(1, 1));
		NxFr::Vec2f Vec2b(3);
		ASSERT_EQ(Vec2b + Vec2b, NxFr::Vec2f(6));
		ASSERT_EQ(Vec2b - Vec2b, NxFr::Vec2f(0));
		ASSERT_EQ(Vec2b * Vec2b, NxFr::Vec2f(9));
		ASSERT_EQ(Vec2b / Vec2b, NxFr::Vec2f(1));
		NxFr::Vec2f Vec2c(1, 2);
		ASSERT_EQ(Vec2c[0], 1);
		ASSERT_EQ(Vec2c[1], 2);
		ASSERT_EQ(NxFr::Vec2f(3), NxFr::Vec3f(3));
		ASSERT_EQ(NxFr::Vec2f(4), NxFr::Vec4f(4));
		ASSERT_EQ(NxFr::Vec2f(1, 2).ToString(), "(1.00, 2.00)");
		ASSERT_EQ(NxFr::Vec2f::Zero, NxFr::Vec2f(0));
		ASSERT_EQ(NxFr::Vec2f::One, NxFr::Vec2f(1));
		ASSERT_EQ(NxFr::Vec2f::Right, NxFr::Vec2f(1, 0));
		ASSERT_EQ(NxFr::Vec2f::Up, NxFr::Vec2f(0, 1));

		ASSERT_EQ(NxFr::Vec3f(), NxFr::Vec3f(0, 0, 0));
		ASSERT_EQ(NxFr::Vec3f(1), NxFr::Vec3f(1, 1, 1));
		ASSERT_EQ(NxFr::Vec3f(2, 3), NxFr::Vec3f(2, 3, 0));
		ASSERT_EQ(NxFr::Vec3f(2, 3, 4), NxFr::Vec3f(2, 3, 4));
		ASSERT_EQ(NxFr::Vec3f(NxFr::Vec2f(1, 2)), NxFr::Vec3f(1, 2, 0));
		ASSERT_EQ(NxFr::Vec3f(NxFr::Vec4f(1, 2, 3, 4)), NxFr::Vec3f(1, 2, 3));
		NxFr::Vec3f Vec3a;
		Vec3a = NxFr::Vec3f(NxFr::Vec2f(1));
		ASSERT_EQ(Vec3a, NxFr::Vec3f(1, 1, 0));
		Vec3a = NxFr::Vec3f(1);
		ASSERT_EQ(Vec3a, NxFr::Vec3f(1, 1, 1));
		Vec3a = NxFr::Vec4f(1);
		ASSERT_EQ(Vec3a, NxFr::Vec3f(1, 1, 1));
		NxFr::Vec3f Vec3b(3);
		ASSERT_EQ(Vec3b + Vec3b, NxFr::Vec3f(6));
		ASSERT_EQ(Vec3b - Vec3b, NxFr::Vec3f(0));
		ASSERT_EQ(Vec3b * Vec3b, NxFr::Vec3f(9));
		ASSERT_EQ(Vec3b / Vec3b, NxFr::Vec3f(1));
		NxFr::Vec3f Vec3c(1, 2, 3);
		ASSERT_EQ(Vec3c[0], 1);
		ASSERT_EQ(Vec3c[1], 2);
		ASSERT_EQ(Vec3c[2], 3);
		ASSERT_EQ(NxFr::Vec3f(4), NxFr::Vec4f(4));
		ASSERT_EQ(NxFr::Vec3f(1, 2, 3).ToString(), "(1.00, 2.00, 3.00)");
		ASSERT_EQ(NxFr::Vec3f::Zero, NxFr::Vec3f(0));
		ASSERT_EQ(NxFr::Vec3f::One, NxFr::Vec3f(1));
		ASSERT_EQ(NxFr::Vec3f::Right, NxFr::Vec3f(1, 0, 0));
		ASSERT_EQ(NxFr::Vec3f::Up, NxFr::Vec3f(0, 1, 0));
		ASSERT_EQ(NxFr::Vec3f::Forward, NxFr::Vec3f(0, 0, 1));

		ASSERT_EQ(NxFr::Vec4f(), NxFr::Vec4f(0, 0, 0, 0));
		ASSERT_EQ(NxFr::Vec4f(1), NxFr::Vec4f(1, 1, 1, 1));
		ASSERT_EQ(NxFr::Vec4f(2, 3), NxFr::Vec4f(2, 3, 0, 0));
		ASSERT_EQ(NxFr::Vec4f(2, 3, 4), NxFr::Vec4f(2, 3, 4, 0));
		ASSERT_EQ(NxFr::Vec4f(2, 3, 4, 5), NxFr::Vec4f(2, 3, 4, 5));
		ASSERT_EQ(NxFr::Vec4f(NxFr::Vec2f(1, 2)), NxFr::Vec4f(1, 2, 0, 0));
		ASSERT_EQ(NxFr::Vec4f(NxFr::Vec3f(1, 2, 3)), NxFr::Vec4f(1, 2, 3, 0));
		NxFr::Vec4f Vec4a;
		Vec4a = NxFr::Vec4f(NxFr::Vec2f(1));
		ASSERT_EQ(Vec4a, NxFr::Vec4f(1, 1, 0, 0));
		Vec4a = NxFr::Vec4f(NxFr::Vec3f(1));
		ASSERT_EQ(Vec4a, NxFr::Vec4f(1, 1, 1, 0));
		Vec4a = NxFr::Vec4f(1);
		ASSERT_EQ(Vec4a, NxFr::Vec4f(1, 1, 1, 1));
		NxFr::Vec4f Vec4b(3);
		ASSERT_EQ(Vec4b + Vec4b, NxFr::Vec4f(6));
		ASSERT_EQ(Vec4b - Vec4b, NxFr::Vec4f(0));
		ASSERT_EQ(Vec4b * Vec4b, NxFr::Vec4f(9));
		ASSERT_EQ(Vec4b / Vec4b, NxFr::Vec4f(1));
		NxFr::Vec4f Vec4c(1, 2, 3, 4);
		ASSERT_EQ(Vec4c[0], 1);
		ASSERT_EQ(Vec4c[1], 2);
		ASSERT_EQ(Vec4c[2], 3);
		ASSERT_EQ(Vec4c[3], 4);
		ASSERT_EQ(NxFr::Vec4f(1, 2, 3, 4).ToString(), "(1.00, 2.00, 3.00, 4.00)");
		ASSERT_EQ(NxFr::Vec4f::Zero, NxFr::Vec4f(0));
		ASSERT_EQ(NxFr::Vec4f::One, NxFr::Vec4f(1));
		ASSERT_EQ(NxFr::Vec4f::Right, NxFr::Vec4f(1, 0, 0));
		ASSERT_EQ(NxFr::Vec4f::Up, NxFr::Vec4f(0, 1, 0));
		ASSERT_EQ(NxFr::Vec4f::Forward, NxFr::Vec4f(0, 0, 1));

		ASSERT_EQ(NxFr::Vec3f::Angle(NxFr::Vec3f(1, 0, 0), NxFr::Vec3f(1, 1, 0)), 0.785398185f);
		ASSERT_EQ(NxFr::Vec3f::Clamp(NxFr::Vec3f(-5.0f, 0.5f, 5.0f), NxFr::Vec3f(0), NxFr::Vec3f(1)), NxFr::Vec3f(0.0f, 0.5f, 1.0f));
		ASSERT_EQ(NxFr::Vec3f::Distance(NxFr::Vec3f(1.0f, 1.0f, 0.0f), NxFr::Vec3f(0)), NxFr::Math::Sqrt(2.0f));
		ASSERT_EQ(NxFr::Vec3f::Lerp(NxFr::Vec3f(0), NxFr::Vec3f(1), 0.5f), NxFr::Vec3f(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::Vec3f::Magnitude(NxFr::Vec3f(1.0f, 1.0f, 0.0f)), NxFr::Math::Sqrt(2.0f));
		ASSERT_EQ(NxFr::Vec3f::SqrMagnitude(NxFr::Vec3f(1.0f, 1.0f, 0.0f)), 2.0f);
		ASSERT_EQ(NxFr::Vec3f::Normalize(NxFr::Vec3f(1.0f, 1.0f, 1.0f)), NxFr::Vec3f(0.577350259f));
		ASSERT_EQ(NxFr::Vec3f::ProjectOnVector(NxFr::Vec3f(1.0f, 1.0f, 1.0f), NxFr::Vec3f(0.0f, 1.0f, 0.0f)), NxFr::Vec3f(0.0f, 1.0f, 0.0f));
		ASSERT_EQ(NxFr::Vec3f::ProjectOnNormal(NxFr::Vec3f(1.0f, 1.0f, 1.0f), NxFr::Vec3f(0.0f, 1.0f, 0.0f)), NxFr::Vec3f(1.0f, 0.0f, 1.0f));
		ASSERT_EQ(NxFr::Vec3f::Reflect(NxFr::Vec3f(1.0f, 1.0f, 1.0f), NxFr::Vec3f(0.0f, 1.0f, 0.0f)), NxFr::Vec3f(1.0f, -1.0f, 1.0f));
		ASSERT_EQ(NxFr::Vec3f::SLerp(NxFr::Vec3f(1.0f, 0.0f, 0.0f), NxFr::Vec3f(0.0f, 1.0f, 0), 0.5f), NxFr::Vec3f(0.707106829f, 0.707106829f, 0.0f));
		ASSERT_EQ(NxFr::Vec3f::Cross(NxFr::Vec3f(1, 0, 0), NxFr::Vec3f(0, 1, 0)), NxFr::Vec3f(0.0f, 0.0f, 1.0f));
		ASSERT_EQ(NxFr::Vec3f::Dot(NxFr::Vec3f(1, 0, 0), NxFr::Vec3f(0, 1, 0)), 0.0f);
		ASSERT_EQ(NxFr::Vec3f::Equals(NxFr::Vec3f(0.5f), NxFr::Vec3f(0.5f)), true);

		ASSERT_EQ(NxFr::Vec3i(), NxFr::Vec3i(0, 0, 0));
		ASSERT_EQ(NxFr::Vec3i(1), NxFr::Vec3i(1, 1, 1));
		ASSERT_EQ(NxFr::Vec3i(2, 3), NxFr::Vec3i(2, 3, 0));
		ASSERT_EQ(NxFr::Vec3i(2, 3, 4), NxFr::Vec3i(2, 3, 4));
		ASSERT_EQ(NxFr::Vec3i(NxFr::Vec2i(1, 2)), NxFr::Vec3i(1, 2, 0));
		ASSERT_EQ(NxFr::Vec3i(NxFr::Vec4i(1, 2, 3, 4)), NxFr::Vec3i(1, 2, 3));
		NxFr::Vec3i Vec3i;
		Vec3i = NxFr::Vec3i(NxFr::Vec2i(1));
		ASSERT_EQ(Vec3i, NxFr::Vec3i(1, 1, 0));
		Vec3i = NxFr::Vec3i(1);
		ASSERT_EQ(Vec3i, NxFr::Vec3i(1, 1, 1));
		Vec3i = NxFr::Vec4i(1);
		ASSERT_EQ(Vec3i, NxFr::Vec3i(1, 1, 1));
		Vec3i = NxFr::Vec3i(3);
		ASSERT_EQ(Vec3i + Vec3i, NxFr::Vec3i(6));
		ASSERT_EQ(Vec3i - Vec3i, NxFr::Vec3i(0));
		ASSERT_EQ(Vec3i * Vec3i, NxFr::Vec3i(9));
		ASSERT_EQ(Vec3i / Vec3i, NxFr::Vec3i(1));

		ASSERT_EQ(NxFr::Vec3i::Clamp(NxFr::Vec3i(-5, 0, 5), NxFr::Vec3i(0), NxFr::Vec3i(1)), NxFr::Vec3i(0, 0, 1));
		ASSERT_EQ(NxFr::Vec3i::Lerp(NxFr::Vec3i(0), NxFr::Vec3i(1), 1), NxFr::Vec3i(1, 1, 1));
		ASSERT_EQ(NxFr::Vec3i::Magnitude(NxFr::Vec3i(1, 1, 0)), 1);
		ASSERT_EQ(NxFr::Vec3i::Normalize(NxFr::Vec3i(1, 1, 1)), NxFr::Vec3i(0));
	}
}
