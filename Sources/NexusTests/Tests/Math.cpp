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
		ASSERT_EQ(NxFr::Math::IsPowerOfTwo(16), true);
		ASSERT_EQ(NxFr::Math::IsPowerOfTwo(15), false);

		ASSERT_EQ(NxFr::Math::NextPowerOfTwo(5), 8);
		ASSERT_EQ(NxFr::Math::NextPowerOfTwo(48), 64);
		ASSERT_EQ(NxFr::Math::NextPowerOfTwo(4), 4);

		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo(16), 4);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo(8), 3);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo(1), 0);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo(0), -1);
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
		ASSERT_EQ(NxFr::Vector2(), NxFr::Vector2(0, 0));
		ASSERT_EQ(NxFr::Vector2(1), NxFr::Vector2(1, 1));
		ASSERT_EQ(NxFr::Vector2(2, 3), NxFr::Vector2(2, 3));
		ASSERT_EQ(NxFr::Vector2(NxFr::Vector3(1, 2, 3)), NxFr::Vector2(1, 2));
		ASSERT_EQ(NxFr::Vector2(NxFr::Vector4(1, 2, 3, 4)), NxFr::Vector2(1, 2));
		NxFr::Vector2 Vec2a;
		Vec2a = NxFr::Vector2(1);
		ASSERT_EQ(Vec2a, NxFr::Vector2(1, 1));
		Vec2a = NxFr::Vector3(1);
		ASSERT_EQ(Vec2a, NxFr::Vector2(1, 1));
		Vec2a = NxFr::Vector4(1);
		ASSERT_EQ(Vec2a, NxFr::Vector2(1, 1));
		NxFr::Vector2 Vec2b(3);
		ASSERT_EQ(Vec2b + Vec2b, NxFr::Vector2(6));
		ASSERT_EQ(Vec2b - Vec2b, NxFr::Vector2(0));
		ASSERT_EQ(Vec2b * Vec2b, NxFr::Vector2(9));
		ASSERT_EQ(Vec2b / Vec2b, NxFr::Vector2(1));
		NxFr::Vector2 Vec2c(1, 2);
		ASSERT_EQ(Vec2c[0], 1);
		ASSERT_EQ(Vec2c[1], 2);
		ASSERT_EQ(NxFr::Vector2(3), NxFr::Vector3(3));
		ASSERT_EQ(NxFr::Vector2(4), NxFr::Vector4(4));
		ASSERT_EQ(NxFr::Vector2(1, 2).ToString(), "(1.00, 2.00)");
		ASSERT_EQ(NxFr::Vector2::Zero, NxFr::Vector2(0));
		ASSERT_EQ(NxFr::Vector2::One, NxFr::Vector2(1));
		ASSERT_EQ(NxFr::Vector2::Right, NxFr::Vector2(1, 0));
		ASSERT_EQ(NxFr::Vector2::Up, NxFr::Vector2(0, 1));

		ASSERT_EQ(NxFr::Vector3(), NxFr::Vector3(0, 0, 0));
		ASSERT_EQ(NxFr::Vector3(1), NxFr::Vector3(1, 1, 1));
		ASSERT_EQ(NxFr::Vector3(2, 3), NxFr::Vector3(2, 3, 0));
		ASSERT_EQ(NxFr::Vector3(2, 3, 4), NxFr::Vector3(2, 3, 4));
		ASSERT_EQ(NxFr::Vector3(NxFr::Vector2(1, 2)), NxFr::Vector3(1, 2, 0));
		ASSERT_EQ(NxFr::Vector3(NxFr::Vector4(1, 2, 3, 4)), NxFr::Vector3(1, 2, 3));
		NxFr::Vector3 Vec3a;
		Vec3a = NxFr::Vector3(NxFr::Vector2(1));
		ASSERT_EQ(Vec3a, NxFr::Vector3(1, 1, 0));
		Vec3a = NxFr::Vector3(1);
		ASSERT_EQ(Vec3a, NxFr::Vector3(1, 1, 1));
		Vec3a = NxFr::Vector4(1);
		ASSERT_EQ(Vec3a, NxFr::Vector3(1, 1, 1));
		NxFr::Vector3 Vec3b(3);
		ASSERT_EQ(Vec3b + Vec3b, NxFr::Vector3(6));
		ASSERT_EQ(Vec3b - Vec3b, NxFr::Vector3(0));
		ASSERT_EQ(Vec3b * Vec3b, NxFr::Vector3(9));
		ASSERT_EQ(Vec3b / Vec3b, NxFr::Vector3(1));
		NxFr::Vector3 Vec3c(1, 2, 3);
		ASSERT_EQ(Vec3c[0], 1);
		ASSERT_EQ(Vec3c[1], 2);
		ASSERT_EQ(Vec3c[2], 3);
		NxFr::Vector3 Vec3d = NxFr::Vector3(1) + (NxFr::Vector3)NxFr::Vector2(1);
		ASSERT_EQ(NxFr::Vector3(4), NxFr::Vector4(4));
		ASSERT_EQ(NxFr::Vector3(1, 2, 3).ToString(), "(1.00, 2.00, 3.00)");
		ASSERT_EQ(NxFr::Vector3::Zero, NxFr::Vector3(0));
		ASSERT_EQ(NxFr::Vector3::One, NxFr::Vector3(1));
		ASSERT_EQ(NxFr::Vector3::Right, NxFr::Vector3(1, 0, 0));
		ASSERT_EQ(NxFr::Vector3::Up, NxFr::Vector3(0, 1, 0));
		ASSERT_EQ(NxFr::Vector3::Forward, NxFr::Vector3(0, 0, 1));

		ASSERT_EQ(NxFr::Vector4(), NxFr::Vector4(0, 0, 0, 0));
		ASSERT_EQ(NxFr::Vector4(1), NxFr::Vector4(1, 1, 1, 1));
		ASSERT_EQ(NxFr::Vector4(2, 3), NxFr::Vector4(2, 3, 0, 0));
		ASSERT_EQ(NxFr::Vector4(2, 3, 4), NxFr::Vector4(2, 3, 4, 0));
		ASSERT_EQ(NxFr::Vector4(2, 3, 4, 5), NxFr::Vector4(2, 3, 4, 5));
		ASSERT_EQ(NxFr::Vector4(NxFr::Vector2(1, 2)), NxFr::Vector4(1, 2, 0, 0));
		ASSERT_EQ(NxFr::Vector4(NxFr::Vector3(1, 2, 3)), NxFr::Vector4(1, 2, 3, 0));
		NxFr::Vector4 Vec4a;
		Vec4a = NxFr::Vector4(NxFr::Vector2(1));
		ASSERT_EQ(Vec4a, NxFr::Vector4(1, 1, 0, 0));
		Vec4a = NxFr::Vector4(NxFr::Vector3(1));
		ASSERT_EQ(Vec4a, NxFr::Vector4(1, 1, 1, 0));
		Vec4a = NxFr::Vector4(1);
		ASSERT_EQ(Vec4a, NxFr::Vector4(1, 1, 1, 1));
		NxFr::Vector4 Vec4b(3);
		ASSERT_EQ(Vec4b + Vec4b, NxFr::Vector4(6));
		ASSERT_EQ(Vec4b - Vec4b, NxFr::Vector4(0));
		ASSERT_EQ(Vec4b * Vec4b, NxFr::Vector4(9));
		ASSERT_EQ(Vec4b / Vec4b, NxFr::Vector4(1));
		NxFr::Vector4 Vec4c(1, 2, 3, 4);
		ASSERT_EQ(Vec4c[0], 1);
		ASSERT_EQ(Vec4c[1], 2);
		ASSERT_EQ(Vec4c[2], 3);
		ASSERT_EQ(Vec4c[3], 4);
		ASSERT_EQ(NxFr::Vector4(1, 2, 3, 4).ToString(), "(1.00, 2.00, 3.00, 4.00)");
		ASSERT_EQ(NxFr::Vector4::Zero, NxFr::Vector4(0));
		ASSERT_EQ(NxFr::Vector4::One, NxFr::Vector4(1));
		ASSERT_EQ(NxFr::Vector4::Right, NxFr::Vector4(1, 0, 0));
		ASSERT_EQ(NxFr::Vector4::Up, NxFr::Vector4(0, 1, 0));
		ASSERT_EQ(NxFr::Vector4::Forward, NxFr::Vector4(0, 0, 1));

		ASSERT_EQ(NxFr::VectorUtility::Angle(NxFr::Vector3(1, 0, 0), NxFr::Vector3(1, 1, 0)), 0.785398185f);
		ASSERT_EQ(NxFr::VectorUtility::Clamp(NxFr::Vector3(-5.0f, 0.5f, 5.0f), NxFr::Vector3(0), NxFr::Vector3(1)), NxFr::Vector3(0.0f, 0.5f, 1.0f));
		ASSERT_EQ(NxFr::VectorUtility::Distance(NxFr::Vector3(1.0f, 1.0f, 0.0f), NxFr::Vector3(0)), NxFr::Math::Sqrt(2.0f));
		ASSERT_EQ(NxFr::VectorUtility::Lerp(NxFr::Vector3(0), NxFr::Vector3(1), 0.5f), NxFr::Vector3(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::VectorUtility::Magnitude(NxFr::Vector3(1.0f, 1.0f, 0.0f)), NxFr::Math::Sqrt(2.0f));
		ASSERT_EQ(NxFr::VectorUtility::SqrMagnitude(NxFr::Vector3(1.0f, 1.0f, 0.0f)), 2.0f);
		ASSERT_EQ(NxFr::VectorUtility::Normalize(NxFr::Vector3(1.0f, 1.0f, 1.0f)), NxFr::Vector3(0.577350259f));
		ASSERT_EQ(NxFr::VectorUtility::ProjectOnVector(NxFr::Vector3(1.0f, 1.0f, 1.0f), NxFr::Vector3(0.0f, 1.0f, 0.0f)), NxFr::Vector3(0.0f, 1.0f, 0.0f));
		ASSERT_EQ(NxFr::VectorUtility::ProjectOnNormal(NxFr::Vector3(1.0f, 1.0f, 1.0f), NxFr::Vector3(0.0f, 1.0f, 0.0f)), NxFr::Vector3(1.0f, 0.0f, 1.0f));
		ASSERT_EQ(NxFr::VectorUtility::Reflect(NxFr::Vector3(1.0f, 1.0f, 1.0f), NxFr::Vector3(0.0f, 1.0f, 0.0f)), NxFr::Vector3(1.0f, -1.0f, 1.0f));
		ASSERT_EQ(NxFr::VectorUtility::Slerp(NxFr::Vector3(1.0f, 0.0f, 0.0f), NxFr::Vector3(0.0f, 1.0f, 0.0f), 0.5f), NxFr::Vector3(0.707106829f, 0.707106829f, 0.0f));
		ASSERT_EQ(NxFr::VectorUtility::Cross(NxFr::Vector3(1, 0, 0), NxFr::Vector3(0, 1, 0)), NxFr::Vector3(0.0f, 0.0f, 1.0f));
		ASSERT_EQ(NxFr::VectorUtility::Dot(NxFr::Vector3(1, 0, 0), NxFr::Vector3(0, 1, 0)), 0.0f);
		ASSERT_EQ(NxFr::VectorUtility::Equals(NxFr::Vector3(0.5f), NxFr::Vector3(0.5f)), true);
		ASSERT_EQ(NxFr::VectorUtility::Similar(NxFr::Vector3(1.0f, 0.0f), NxFr::Vector3(2.0f, 0.0f)), true);
		ASSERT_EQ(NxFr::VectorUtility::Parallel(NxFr::Vector3(1.0f, 0.0f), NxFr::Vector3(2.0f, 0.0f)), true);
		ASSERT_EQ(NxFr::VectorUtility::Perpendicular(NxFr::Vector3(1.0f, 0.0f), NxFr::Vector3(0.0f, 1.0f)), true);

		ASSERT_EQ(NxFr::Vector3i(), NxFr::Vector3i(0, 0, 0));
		ASSERT_EQ(NxFr::Vector3i(1), NxFr::Vector3i(1, 1, 1));
		ASSERT_EQ(NxFr::Vector3i(2, 3), NxFr::Vector3i(2, 3, 0));
		ASSERT_EQ(NxFr::Vector3i(2, 3, 4), NxFr::Vector3i(2, 3, 4));
		ASSERT_EQ(NxFr::Vector3i(NxFr::Vector2i(1, 2)), NxFr::Vector3i(1, 2, 0));
		ASSERT_EQ(NxFr::Vector3i(NxFr::Vector4i(1, 2, 3, 4)), NxFr::Vector3i(1, 2, 3));
		NxFr::Vector3i Vec3i;
		Vec3i = NxFr::Vector3i(NxFr::Vector2i(1));
		ASSERT_EQ(Vec3i, NxFr::Vector3i(1, 1, 0));
		Vec3i = NxFr::Vector3i(1);
		ASSERT_EQ(Vec3i, NxFr::Vector3i(1, 1, 1));
		Vec3i = NxFr::Vector4i(1);
		ASSERT_EQ(Vec3i, NxFr::Vector3i(1, 1, 1));
		Vec3i = NxFr::Vector3i(3);
		ASSERT_EQ(Vec3i + Vec3i, NxFr::Vector3i(6));
		ASSERT_EQ(Vec3i - Vec3i, NxFr::Vector3i(0));
		ASSERT_EQ(Vec3i * Vec3i, NxFr::Vector3i(9));
		ASSERT_EQ(Vec3i / Vec3i, NxFr::Vector3i(1));

		ASSERT_EQ(NxFr::VectorUtility::Clamp(NxFr::Vector3i(-5.5f, 0.5f, 5.5f), NxFr::Vector3i(0), NxFr::Vector3i(1)), NxFr::Vector3i(0, 0, 1));
		ASSERT_EQ(NxFr::VectorUtility::Lerp(NxFr::Vector3i(0), NxFr::Vector3i(1), 0.25f), NxFr::Vector3i(0));
		ASSERT_EQ(NxFr::VectorUtility::Magnitude(NxFr::Vector3i(1, 1, 0)), 1.41421354f);
		ASSERT_EQ(NxFr::VectorUtility::Normalize(NxFr::Vector3i(1, 1, 1)), NxFr::Vector3(0.577350259f));
	}

	TEST(Math, Rotation)
	{
		NxFr::Quaternion Quaternion = NxFr::Quaternion({ NxFr::Vector3::Up, 45.0f });
		ASSERT_EQ(NxFr::Quaternion::Identity, NxFr::Euler::Identity);

		ASSERT_EQ(NxFr::RotationUtility::Equals(Quaternion, (NxFr::Quaternion)NxFr::AxisAngle(NxFr::Vector3::Up, 45.0f)), true);
		ASSERT_EQ(NxFr::RotationUtility::Equals((NxFr::AxisAngle)Quaternion, NxFr::AxisAngle(NxFr::Vector3::Up, 45.0f)), true);
		ASSERT_EQ(NxFr::RotationUtility::Equals(Quaternion, (NxFr::Quaternion)NxFr::Euler(0.0f, 45.0f, 0.0f)), true);
		ASSERT_EQ(NxFr::RotationUtility::Equals((NxFr::Euler)Quaternion, NxFr::Euler(0.0f, 45.0f, 0.0f)), true);
		ASSERT_EQ(NxFr::RotationUtility::Equals((NxFr::Quaternion)NxFr::Euler(0.0f, 45.0f, 0.0f), (NxFr::Quaternion)NxFr::AxisAngle(NxFr::Vector3::Up, 45.0f)), true);

		auto Rotation = NxFr::Quaternion::Identity;
		Rotation = NxFr::Quaternion(NxFr::Euler(0, 0, 60)) * Rotation;
		Rotation = NxFr::Quaternion(NxFr::Euler(45, 0, 0)) * Rotation;
		Rotation = NxFr::Quaternion(NxFr::Euler(0, 30, 0)) * Rotation;
		ASSERT_EQ(NxFr::RotationUtility::Equals(NxFr::Quaternion(NxFr::Euler(45, 30, 60)), Rotation), true);

		auto From = NxFr::VectorUtility::Normalize(NxFr::Vector3(1.0f, 1.0f, 0.0f));
		auto To = NxFr::VectorUtility::Normalize(NxFr::Vector3(0.0f, 1.0f, 1.0f));
		auto FromTo = NxFr::RotationUtility::FromTo(From, To);
		ASSERT_EQ(NxFr::VectorUtility::Equals(From * FromTo, To), true);

		ASSERT_EQ(NxFr::VectorUtility::Equals(NxFr::Quaternion(NxFr::Euler(45, 30, 60)) * NxFr::Vector3::Forward, NxFr::Vector3(0.3535533f, -0.7071068f, 0.6123724f)), true);

		ASSERT_EQ(NxFr::Math::Equals(NxFr::RotationUtility::Dot(Quaternion, NxFr::Quaternion::Identity), 0.9238f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::RotationUtility::SqrMagnitude(Quaternion), 1.0f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::RotationUtility::Magnitude(Quaternion), 1.0f),true);
		ASSERT_EQ(NxFr::RotationUtility::Equals(NxFr::RotationUtility::Normalize(Quaternion), Quaternion), true);
		ASSERT_EQ(NxFr::RotationUtility::Equals(NxFr::RotationUtility::Inverse(Quaternion), NxFr::Quaternion({ -NxFr::Vector3::Up, 45.0f })), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::RotationUtility::Angle(Quaternion, NxFr::Quaternion::Identity) * NxFr::Math::Degree, 45.0f), true);
		ASSERT_EQ(NxFr::RotationUtility::Equals(NxFr::RotationUtility::Lerp(Quaternion, NxFr::Quaternion::Identity, 0.5f), NxFr::Quaternion({ NxFr::Vector3::Up, 22.5f })), true);
		ASSERT_EQ(NxFr::RotationUtility::Equals(NxFr::RotationUtility::Slerp(Quaternion, NxFr::Quaternion::Identity, 0.5f), NxFr::Quaternion({ NxFr::Vector3::Up, 22.5f })), true);
		ASSERT_EQ(NxFr::RotationUtility::Similar(Quaternion, NxFr::Quaternion({ NxFr::Vector3::Up, 45.0f })), true);
	}
}
