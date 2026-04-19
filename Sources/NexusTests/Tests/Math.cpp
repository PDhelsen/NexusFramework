#include "NexusTests/Core/NexusTests.h"

namespace NxTs
{
	TEST(Math, Math)
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
	
		ASSERT_EQ(NxFr::Math::IsPowerOfTwo(16), true);
		ASSERT_EQ(NxFr::Math::IsPowerOfTwo(15), false);

		ASSERT_EQ(NxFr::Math::NextPowerOfTwo(5), 8);
		ASSERT_EQ(NxFr::Math::NextPowerOfTwo(48), 64);
		ASSERT_EQ(NxFr::Math::NextPowerOfTwo(4), 4);

		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo(16), 4);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo(8), 3);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo(1), 0);
		ASSERT_EQ(NxFr::Math::LogTwoPowerOfTwo(0), -1);
	
		ASSERT_EQ(NxFr::Math::IsPrime((uint32)99), false);
		ASSERT_EQ(NxFr::Math::IsPrime((uint32)97), true);

		ASSERT_EQ(NxFr::Math::NextPrime((uint32)74), 79);
		ASSERT_EQ(NxFr::Math::NextPrime((uint32)84), 89);
	}

	TEST(Math, Vector)
	{
		ASSERT_EQ(NxFr::Vector2f(), NxFr::Vector2f(0, 0));
		ASSERT_EQ(NxFr::Vector2f(1), NxFr::Vector2f(1, 1));
		ASSERT_EQ(NxFr::Vector2f(2, 3), NxFr::Vector2f(2, 3));
		ASSERT_EQ(NxFr::Vector2f(NxFr::Vector3f(1, 2, 3)), NxFr::Vector2f(1, 2));
		ASSERT_EQ(NxFr::Vector2f(NxFr::Vector4f(1, 2, 3, 4)), NxFr::Vector2f(1, 2));
		NxFr::Vector2f Vec2a;
		Vec2a = NxFr::Vector2f(1);
		ASSERT_EQ(Vec2a, NxFr::Vector2f(1, 1));
		Vec2a = NxFr::Vector3f(1);
		ASSERT_EQ(Vec2a, NxFr::Vector2f(1, 1));
		Vec2a = NxFr::Vector4f(1);
		ASSERT_EQ(Vec2a, NxFr::Vector2f(1, 1));
		NxFr::Vector2f Vec2b(3);
		ASSERT_EQ(Vec2b + Vec2b, NxFr::Vector2f(6));
		ASSERT_EQ(Vec2b - Vec2b, NxFr::Vector2f(0));
		ASSERT_EQ(Vec2b * Vec2b, NxFr::Vector2f(9));
		ASSERT_EQ(Vec2b / Vec2b, NxFr::Vector2f(1));
		NxFr::Vector2f Vec2c(1, 2);
		ASSERT_EQ(Vec2c[0], 1);
		ASSERT_EQ(Vec2c[1], 2);
		ASSERT_EQ(NxFr::Vector2f(3), NxFr::Vector3f(3));
		ASSERT_EQ(NxFr::Vector2f(4), NxFr::Vector4f(4));
		ASSERT_EQ(NxFr::Vector2f::Zero, NxFr::Vector2f(0));
		ASSERT_EQ(NxFr::Vector2f::One, NxFr::Vector2f(1));
		ASSERT_EQ(NxFr::Vector2f::Right, NxFr::Vector2f(1, 0));
		ASSERT_EQ(NxFr::Vector2f::Up, NxFr::Vector2f(0, 1));

		ASSERT_EQ(NxFr::Vector3f(), NxFr::Vector3f(0, 0, 0));
		ASSERT_EQ(NxFr::Vector3f(1), NxFr::Vector3f(1, 1, 1));
		ASSERT_EQ(NxFr::Vector3f(2, 3), NxFr::Vector3f(2, 3, 0));
		ASSERT_EQ(NxFr::Vector3f(2, 3, 4), NxFr::Vector3f(2, 3, 4));
		ASSERT_EQ(NxFr::Vector3f(NxFr::Vector2f(1, 2)), NxFr::Vector3f(1, 2, 0));
		ASSERT_EQ(NxFr::Vector3f(NxFr::Vector4f(1, 2, 3, 4)), NxFr::Vector3f(1, 2, 3));
		NxFr::Vector3f Vec3a;
		Vec3a = NxFr::Vector3f(NxFr::Vector2f(1));
		ASSERT_EQ(Vec3a, NxFr::Vector3f(1, 1, 0));
		Vec3a = NxFr::Vector3f(1);
		ASSERT_EQ(Vec3a, NxFr::Vector3f(1, 1, 1));
		Vec3a = NxFr::Vector4f(1);
		ASSERT_EQ(Vec3a, NxFr::Vector3f(1, 1, 1));
		NxFr::Vector3f Vec3b(3);
		ASSERT_EQ(Vec3b + Vec3b, NxFr::Vector3f(6));
		ASSERT_EQ(Vec3b - Vec3b, NxFr::Vector3f(0));
		ASSERT_EQ(Vec3b * Vec3b, NxFr::Vector3f(9));
		ASSERT_EQ(Vec3b / Vec3b, NxFr::Vector3f(1));
		NxFr::Vector3f Vec3c(1, 2, 3);
		ASSERT_EQ(Vec3c[0], 1);
		ASSERT_EQ(Vec3c[1], 2);
		ASSERT_EQ(Vec3c[2], 3);
		NxFr::Vector3f Vec3d = NxFr::Vector3f(1) + (NxFr::Vector3f)NxFr::Vector2f(1);
		ASSERT_EQ(NxFr::Vector3f(4), NxFr::Vector4f(4));
		ASSERT_EQ(NxFr::Vector3f::Zero, NxFr::Vector3f(0));
		ASSERT_EQ(NxFr::Vector3f::One, NxFr::Vector3f(1));
		ASSERT_EQ(NxFr::Vector3f::Right, NxFr::Vector3f(1, 0, 0));
		ASSERT_EQ(NxFr::Vector3f::Up, NxFr::Vector3f(0, 1, 0));
		ASSERT_EQ(NxFr::Vector3f::Forward, NxFr::Vector3f(0, 0, 1));

		ASSERT_EQ(NxFr::Vector4f(), NxFr::Vector4f(0, 0, 0, 0));
		ASSERT_EQ(NxFr::Vector4f(1), NxFr::Vector4f(1, 1, 1, 1));
		ASSERT_EQ(NxFr::Vector4f(2, 3), NxFr::Vector4f(2, 3, 0, 0));
		ASSERT_EQ(NxFr::Vector4f(2, 3, 4), NxFr::Vector4f(2, 3, 4, 0));
		ASSERT_EQ(NxFr::Vector4f(2, 3, 4, 5), NxFr::Vector4f(2, 3, 4, 5));
		ASSERT_EQ(NxFr::Vector4f(NxFr::Vector2f(1, 2)), NxFr::Vector4f(1, 2, 0, 0));
		ASSERT_EQ(NxFr::Vector4f(NxFr::Vector3f(1, 2, 3)), NxFr::Vector4f(1, 2, 3, 0));
		NxFr::Vector4f Vec4a;
		Vec4a = NxFr::Vector4f(NxFr::Vector2f(1));
		ASSERT_EQ(Vec4a, NxFr::Vector4f(1, 1, 0, 0));
		Vec4a = NxFr::Vector4f(NxFr::Vector3f(1));
		ASSERT_EQ(Vec4a, NxFr::Vector4f(1, 1, 1, 0));
		Vec4a = NxFr::Vector4f(1);
		ASSERT_EQ(Vec4a, NxFr::Vector4f(1, 1, 1, 1));
		NxFr::Vector4f Vec4b(3);
		ASSERT_EQ(Vec4b + Vec4b, NxFr::Vector4f(6));
		ASSERT_EQ(Vec4b - Vec4b, NxFr::Vector4f(0));
		ASSERT_EQ(Vec4b * Vec4b, NxFr::Vector4f(9));
		ASSERT_EQ(Vec4b / Vec4b, NxFr::Vector4f(1));
		NxFr::Vector4f Vec4c(1, 2, 3, 4);
		ASSERT_EQ(Vec4c[0], 1);
		ASSERT_EQ(Vec4c[1], 2);
		ASSERT_EQ(Vec4c[2], 3);
		ASSERT_EQ(Vec4c[3], 4);
		ASSERT_EQ(NxFr::Vector4f::Zero, NxFr::Vector4f(0));
		ASSERT_EQ(NxFr::Vector4f::One, NxFr::Vector4f(1));
		ASSERT_EQ(NxFr::Vector4f::Right, NxFr::Vector4f(1, 0, 0));
		ASSERT_EQ(NxFr::Vector4f::Up, NxFr::Vector4f(0, 1, 0));
		ASSERT_EQ(NxFr::Vector4f::Forward, NxFr::Vector4f(0, 0, 1));

		ASSERT_EQ(NxFr::StringUtility::ToString(NxFr::Vector2f(1, 2)), "(1.00, 2.00)");
		ASSERT_EQ(NxFr::StringUtility::ToString(NxFr::Vector3f(1, 2, 3)), "(1.00, 2.00, 3.00)");
		ASSERT_EQ(NxFr::StringUtility::ToString(NxFr::Vector4f(1, 2, 3, 4)), "(1.00, 2.00, 3.00, 4.00)");

		ASSERT_EQ(NxFr::VectorUtility::Angle(NxFr::Vector3f(1, 0, 0), NxFr::Vector3f(1, 1, 0)), 0.785398185f);
		ASSERT_EQ(NxFr::VectorUtility::Clamp(NxFr::Vector3f(-5.0f, 0.5f, 5.0f), NxFr::Vector3f(0), NxFr::Vector3f(1)), NxFr::Vector3f(0.0f, 0.5f, 1.0f));
		ASSERT_EQ(NxFr::VectorUtility::Distance(NxFr::Vector3f(1.0f, 1.0f, 0.0f), NxFr::Vector3f(0)), NxFr::Math::Sqrt(2.0f));
		ASSERT_EQ(NxFr::VectorUtility::Lerp(NxFr::Vector3f(0), NxFr::Vector3f(1), 0.5f), NxFr::Vector3f(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::VectorUtility::Magnitude(NxFr::Vector3f(1.0f, 1.0f, 0.0f)), NxFr::Math::Sqrt(2.0f));
		ASSERT_EQ(NxFr::VectorUtility::SqrMagnitude(NxFr::Vector3f(1.0f, 1.0f, 0.0f)), 2.0f);
		ASSERT_EQ(NxFr::VectorUtility::Normalize(NxFr::Vector3f(1.0f, 1.0f, 1.0f)), NxFr::Vector3f(0.577350259f));
		ASSERT_EQ(NxFr::VectorUtility::ProjectOnVector(NxFr::Vector3f(1.0f, 1.0f, 1.0f), NxFr::Vector3f(0.0f, 1.0f, 0.0f)), NxFr::Vector3f(0.0f, 1.0f, 0.0f));
		ASSERT_EQ(NxFr::VectorUtility::ProjectOnNormal(NxFr::Vector3f(1.0f, 1.0f, 1.0f), NxFr::Vector3f(0.0f, 1.0f, 0.0f)), NxFr::Vector3f(1.0f, 0.0f, 1.0f));
		ASSERT_EQ(NxFr::VectorUtility::Reflect(NxFr::Vector3f(1.0f, 1.0f, 1.0f), NxFr::Vector3f(0.0f, 1.0f, 0.0f)), NxFr::Vector3f(1.0f, -1.0f, 1.0f));
		ASSERT_EQ(NxFr::VectorUtility::Slerp(NxFr::Vector3f(1.0f, 0.0f, 0.0f), NxFr::Vector3f(0.0f, 1.0f, 0.0f), 0.5f), NxFr::Vector3f(0.707106829f, 0.707106829f, 0.0f));
		ASSERT_EQ(NxFr::VectorUtility::Cross(NxFr::Vector3f(1, 0, 0), NxFr::Vector3f(0, 1, 0)), NxFr::Vector3f(0.0f, 0.0f, 1.0f));
		ASSERT_EQ(NxFr::VectorUtility::Dot(NxFr::Vector3f(1, 0, 0), NxFr::Vector3f(0, 1, 0)), 0.0f);
		ASSERT_EQ(NxFr::VectorUtility::Similar(NxFr::Vector3f(1.0f, 0.0f), NxFr::Vector3f(2.0f, 0.0f)), true);
		ASSERT_EQ(NxFr::VectorUtility::Parallel(NxFr::Vector3f(1.0f, 0.0f), NxFr::Vector3f(2.0f, 0.0f)), true);
		ASSERT_EQ(NxFr::VectorUtility::Perpendicular(NxFr::Vector3f(1.0f, 0.0f), NxFr::Vector3f(0.0f, 1.0f)), true);

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
		ASSERT_EQ(NxFr::VectorUtility::Normalize(NxFr::Vector3i(1, 1, 1)), NxFr::Vector3f(0.577350259f));

		NxFr::Vector<5, int32> Vec5a = NxFr::Vector<5, int32>(1);
		NxFr::Vector<5, int32> Vec5b = NxFr::Vector<5, int32>(2);
		ASSERT_EQ(Vec5a + Vec5a, Vec5b);
	}

	TEST(Math, Rotation)
	{
		NxFr::Quaternion Quaternion = NxFr::Quaternion({ NxFr::Vector3f::Up, 45.0f });
		ASSERT_EQ(NxFr::Quaternion::Identity, NxFr::Euler::Identity);

		ASSERT_EQ(Quaternion, (NxFr::Quaternion)NxFr::AxisAngle(NxFr::Vector3f::Up, 45.0f));
		ASSERT_EQ((NxFr::AxisAngle)Quaternion, NxFr::AxisAngle(NxFr::Vector3f::Up, 45.0f));
		ASSERT_EQ(Quaternion, (NxFr::Quaternion)NxFr::Euler(0.0f, 45.0f, 0.0f));
		ASSERT_EQ((NxFr::Euler)Quaternion, NxFr::Euler(0.0f, 45.0f, 0.0f));
		ASSERT_EQ((NxFr::Quaternion)NxFr::Euler(0.0f, 45.0f, 0.0f), (NxFr::Quaternion)NxFr::AxisAngle(NxFr::Vector3f::Up, 45.0f));

		auto Rotation = NxFr::Quaternion::Identity;
		Rotation = NxFr::Quaternion(NxFr::Euler(0, 0, 60)) * Rotation;
		Rotation = NxFr::Quaternion(NxFr::Euler(45, 0, 0)) * Rotation;
		Rotation = NxFr::Quaternion(NxFr::Euler(0, 30, 0)) * Rotation;
		ASSERT_EQ(NxFr::Quaternion(NxFr::Euler(45, 30, 60)), Rotation);
		ASSERT_EQ(NxFr::Quaternion(NxFr::Euler(45, 30, 60)), NxFr::Quaternion(NxFr::Euler(0, 30, 0)) * NxFr::Quaternion(NxFr::Euler(45, 0, 0)) * NxFr::Quaternion(NxFr::Euler(0, 0, 60)));

		NxFr::Vector3f From = NxFr::VectorUtility::Normalize(NxFr::Vector3f(1.0f, 1.0f, 0.0f));
		NxFr::Vector3f To = NxFr::VectorUtility::Normalize(NxFr::Vector3f(0.0f, 1.0f, 1.0f));
		NxFr::Quaternion FromTo = NxFr::RotationUtility::FromTo(From, To);
		ASSERT_EQ(From * FromTo, To);
		NxFr::Quaternion LookAt = NxFr::RotationUtility::LookAt(NxFr::Vector3f::Zero, NxFr::Vector3f::Right, NxFr::Vector3f::Up);
		ASSERT_EQ(LookAt * NxFr::Vector3f::Forward, NxFr::Vector3f::Right);

		ASSERT_EQ(NxFr::Quaternion(NxFr::Euler(45, 30, 60)) * NxFr::Vector3f::Forward, NxFr::Vector3f(0.3535533f, -0.7071068f, 0.6123724f));

		ASSERT_EQ(NxFr::Math::Equals(NxFr::RotationUtility::Dot(Quaternion, NxFr::Quaternion::Identity), 0.9238f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::RotationUtility::SqrMagnitude(Quaternion), 1.0f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::RotationUtility::Magnitude(Quaternion), 1.0f), true);
		ASSERT_EQ(NxFr::RotationUtility::Normalize(Quaternion), Quaternion);
		ASSERT_EQ(NxFr::RotationUtility::Inverse(Quaternion), NxFr::Quaternion({ -NxFr::Vector3f::Up, 45.0f }));
		ASSERT_EQ(NxFr::Math::Equals(NxFr::RotationUtility::Angle(Quaternion, NxFr::Quaternion::Identity) * NxFr::Math::Degree, 45.0f), true);
		ASSERT_EQ(NxFr::RotationUtility::Lerp(Quaternion, NxFr::Quaternion::Identity, 0.5f), NxFr::Quaternion({ NxFr::Vector3f::Up, 22.5f }));
		ASSERT_EQ(NxFr::RotationUtility::Slerp(Quaternion, NxFr::Quaternion::Identity, 0.5f), NxFr::Quaternion({ NxFr::Vector3f::Up, 22.5f }));
		ASSERT_EQ(NxFr::RotationUtility::Similar(Quaternion, NxFr::Quaternion({ NxFr::Vector3f::Up, 45.0f })), true);
	}

	TEST(Math, Matrix)
	{
		NxFr::Matrix4x4f Indentity = NxFr::Matrix4x4f();
		Indentity[0] = Indentity[5] = Indentity[10] = Indentity[15] = 1.0f;
		ASSERT_EQ(Indentity == NxFr::Matrix4x4f::Identity, true);

		NxFr::Matrix4x4f Matrix = NxFr::Matrix4x4f();
		for (uint8 Index = 0; Index < Matrix.Count; ++Index)
		{
			Matrix[Index] = (float)Index;
		}

		ASSERT_EQ(Matrix[4], Matrix(0, 1));
		ASSERT_EQ(Matrix[10], Matrix(2, 2));
		ASSERT_EQ(Matrix.GetColumn(1), NxFr::Vector4f(4.0f, 5.0f, 6.0f, 7.0f));
		ASSERT_EQ(Matrix.GetRow(1), NxFr::Vector4f(1.0f, 5.0f, 9.0f, 13.0f));
		ASSERT_EQ(Matrix.GetDiagonal(), NxFr::Vector4f(0.0f, 5.0f, 10.0f, 15.0f));

		NxFr::Matrix4x4f AddScalar = Matrix + 1.0f;
		ASSERT_EQ(AddScalar.GetColumn(0), NxFr::Vector4f(1.0f, 2.0f, 3.0f, 4.0f));
		NxFr::Matrix4x4f AddMatrix = Matrix + Matrix;
		ASSERT_EQ(AddMatrix.GetColumn(0), NxFr::Vector4f(0.0f, 2.0f, 4.0f, 6.0f));
		NxFr::Matrix4x4f MultiplyScalar = Matrix * 2.0f;
		ASSERT_EQ(MultiplyScalar.GetColumn(0), NxFr::Vector4f(0.0f, 2.0f, 4.0f, 6.0f));
		NxFr::Vector4f MultiplyVector = Matrix * NxFr::Vector4f(2.0f);
		ASSERT_EQ(MultiplyVector, NxFr::Vector4f(48.0f, 56.0f, 64.0f, 72.0f));
		NxFr::Matrix4x4f MultiplyMatrix = Matrix * Matrix;
		ASSERT_EQ(MultiplyMatrix.GetColumn(0), NxFr::Vector4f(56.0f, 62.0f, 68.0f, 74.0f));

		NxFr::Matrix4x4f Transpose = Matrix.Transpose();
		ASSERT_EQ(Transpose.GetRow(1), NxFr::Vector4f(4.0f, 5.0f, 6.0f, 7.0f));
		ASSERT_EQ(Transpose.GetColumn(1), NxFr::Vector4f(1.0f, 5.0f, 9.0f, 13.0f));
		float Trace = Matrix.Trace();
		ASSERT_EQ(Trace, 30.0f);

		Matrix = NxFr::Matrix4x4f();
		Matrix[0] = 1.0f;
		Matrix[5] = 2.0f;
		Matrix[10] = 3.0f;
		Matrix[15] = 4.0f;
		NxFr::Matrix4x4f Inverse = Matrix.Inverse();
		ASSERT_EQ(Inverse.GetDiagonal(), NxFr::Vector4f(1.0f, 0.5f, 0.333333f, 0.25f));
		float Determinant = Matrix.Determinant();
		ASSERT_EQ(Determinant, 24.0f);

		NxFr::Vector3f Vector = NxFr::Vector3f(1.0f, 2.0f, 3.0f);
		NxFr::Quaternion Quaternion = NxFr::Euler(45.0f, 30.0f, 60.0f);

		NxFr::Matrix4x4f Translation = NxFr::Matrix4x4f::Translate(Vector);
		ASSERT_EQ(Translation.MultiplyPosition(NxFr::Vector3f::One), NxFr::Vector3f::One + Vector);
		NxFr::Matrix4x4f Rotation = NxFr::Matrix4x4f::Rotate(Quaternion);
		ASSERT_EQ(Rotation.MultiplyPosition(NxFr::Vector3f::One), Quaternion * NxFr::Vector3f::One);
		NxFr::Matrix4x4f Scale = NxFr::Matrix4x4f::Scale(Vector);
		ASSERT_EQ(Scale.MultiplyPosition(NxFr::Vector3f::One), Vector);
		NxFr::Matrix4x4f Reflection = NxFr::Matrix4x4f::Reflect(NxFr::Vector3f(0.0f, 1.0f, 0.0f));
		ASSERT_EQ(Reflection.MultiplyPosition(NxFr::Vector3f::One), NxFr::Vector3f(1.0f, -1.0f, 1.0f));
		NxFr::Matrix4x4f Shear = NxFr::Matrix4x4f::Shear(NxFr::Vector3f(0.0f, 1.0f, 0.0f), NxFr::Vector3f(1.0f, 0.0f, 3.0f));
		ASSERT_EQ(Shear.MultiplyPosition(NxFr::Vector3f::One), NxFr::Vector3f(2.0f, 1.0f, 4.0f));

		NxFr::Matrix4x4f Trs = NxFr::Matrix4x4f::TRS(Vector, Quaternion, Vector);
		ASSERT_EQ(Trs.MultiplyPosition(Vector), NxFr::Vector3f(2.628287f, -2.337375f, 11.74848f));
		NxFr::Matrix4x4f Origin = NxFr::Matrix4x4f::Origin(Rotation * Scale, Vector);
		ASSERT_EQ(Origin, NxFr::Matrix4x4f::Translate(Vector) * Rotation * Scale * NxFr::Matrix4x4f::Translate(-Vector));

		ASSERT_EQ(Trs.GetPosition(), Vector);
		ASSERT_EQ(Trs.GetRotation(), Quaternion);
		ASSERT_EQ(Trs.GetScale(), Vector);

		NxFr::Vector3f Position = Translation.MultiplyPosition(Vector);
		ASSERT_EQ(Position, 2.0f * Vector);
		NxFr::Vector3f Direction = Translation.MultiplyDirection(Vector);
		ASSERT_EQ(Direction, Vector);

		NxFr::Matrix4x4f LookAt = NxFr::Matrix4x4f::LookAt(NxFr::Vector3f::Zero, NxFr::Vector3f::Right, NxFr::Vector3f::Up);
		ASSERT_EQ(LookAt.MultiplyDirection(NxFr::Vector3f::Forward), NxFr::Vector3f::Right);
	}

	TEST(Math, Shape)
	{
		NxFr::Ray Ray = NxFr::Ray(NxFr::Vector3f::Zero, NxFr::Vector3f::Forward);
		ASSERT_EQ(Ray.Flip().Direction, -NxFr::Vector3f::Forward);
		ASSERT_EQ(Ray.Flip().Direction, NxFr::Vector3f::Forward);
		ASSERT_EQ(Ray.Translate(NxFr::Vector3f(0.0f, 1.0f, 0.0f)).Origin, NxFr::Vector3f(0.0f, 1.0f, 0.0f));
		ASSERT_EQ(Ray.Rotate(NxFr::Euler(-90.0f, 0.0f, 0.0f)).Direction, NxFr::Vector3f::Up);
		ASSERT_EQ(Ray.Rotate(NxFr::Euler(90.0f, 0.0f, 0.0f)).Direction, NxFr::Vector3f::Forward);
		
		NxFr::Plane Plane = NxFr::Plane(NxFr::Vector3f::Forward, 0.0f);
		ASSERT_EQ(Plane.Flip().Normal, -NxFr::Vector3f::Forward);
		ASSERT_EQ(Plane.Flip().Normal, NxFr::Vector3f::Forward);
		ASSERT_EQ(Plane.Translate(1.0f).Distance, 1.0f);
		ASSERT_EQ(Plane.Rotate(NxFr::Euler(-90.0f, 0.0f, 0.0f)).Normal, NxFr::Vector3f::Up);
		ASSERT_EQ(Plane.Rotate(NxFr::Euler(90.0f, 0.0f, 0.0f)).Normal, NxFr::Vector3f::Forward);

		NxFr::Triangle Triangle = NxFr::Triangle(NxFr::Vector3f(1.0f, 0.0f, 0.0f), NxFr::Vector3f(-1.0f, 0.0f, 0.0f), NxFr::Vector3f(0.0f, 1.0f, 0.0f));
		ASSERT_EQ(Triangle.Normal(), NxFr::Vector3f::Forward);
		Triangle.Scale(NxFr::Vector3f::One * 2.0f);
		Triangle.Rotate(NxFr::Euler(0.0f, 0.0f, 45.0f));
		Triangle.Translate(NxFr::Vector3f(0.0f, 1.0f, 0.0f));
		
		NxFr::Rectangle Rectangle = NxFr::Rectangle(NxFr::Vector2f::Zero, NxFr::Vector2f::One);
		ASSERT_EQ(Rectangle.Translate(NxFr::Vector2f(0.0f, 1.0f)).Center, NxFr::Vector2f::Up);
		ASSERT_EQ(Rectangle.Scale(NxFr::Vector2f::One * 2.0f).Extents, NxFr::Vector2f(2.0f));
		ASSERT_EQ(Rectangle.Grow(NxFr::Vector2f::One).Extents, NxFr::Vector2f(3.0f));
		ASSERT_EQ(Rectangle.GetBottomRight().x - Rectangle.GetBottomLeft().x, Rectangle.GetWidth());
		ASSERT_EQ(Rectangle.GetTopLeft().y - Rectangle.GetBottomLeft().y, Rectangle.GetHeight());
		ASSERT_EQ(Rectangle.GetCorner(2), Rectangle.GetTopRight());
		ASSERT_EQ(Rectangle.Encapsulate(NxFr::Rectangle(NxFr::Vector2f::Right * 2.0f, NxFr::Vector2f::One * 3.0f)).Extents, NxFr::Vector2f(4.0f, 3.5f));

		NxFr::Circle Circle = NxFr::Circle(NxFr::Vector3f::Zero, 1.0f);
		ASSERT_EQ(Circle.Translate(NxFr::Vector2f(0.0f, 1.0f)).Center, NxFr::Vector2f::Up);
		ASSERT_EQ(Circle.Scale(2.0f).Radius, 2.0f);
		ASSERT_EQ(Circle.Grow(1.0f).Radius, 3.0f);
		ASSERT_EQ(Circle.GetDiameter(), 6.0f);
		ASSERT_EQ(Circle.Encapsulate(NxFr::Circle(NxFr::Vector2f::Up * 4.0f, 3.0f)).Radius, 4.5f);

		NxFr::Box Box = NxFr::Box(NxFr::Vector3f::Zero, NxFr::Vector3f::One);
		ASSERT_EQ(Box.Translate(NxFr::Vector3f(0.0f, 1.0f)).Center, NxFr::Vector3f::Up);
		ASSERT_EQ(Box.Scale(NxFr::Vector3f::One * 2.0f).Extents, NxFr::Vector3f(2.0f));
		ASSERT_EQ(Box.Grow(NxFr::Vector3f::One).Extents, NxFr::Vector3f(3.0f));
		ASSERT_EQ(Box.GetMin(), NxFr::Vector3f(-3.0f, -2.0f, -3.0f));
		ASSERT_EQ(Box.Encapsulate(NxFr::Box(NxFr::Vector3f::Right * 2.0f, NxFr::Vector3f::One * 3.0f)).Extents, NxFr::Vector3f(4.0f, 3.5f, 3.0f));

		NxFr::Cube Cube = NxFr::Cube(NxFr::Vector3f::Zero, NxFr::Vector3f::One);
		ASSERT_EQ(Cube.Translate(NxFr::Vector3f(0.0f, 1.0f)).Center, NxFr::Vector3f::Up);
		ASSERT_EQ(Cube.Scale(NxFr::Vector3f::One * 2.0f).Extents, NxFr::Vector3f(2.0f));
		ASSERT_EQ(Cube.Grow(NxFr::Vector3f::One).Extents, NxFr::Vector3f(3.0f));
		ASSERT_EQ(Cube.Encapsulate(NxFr::Cube(NxFr::Vector3f::Right * 2.0f, NxFr::Vector3f::One * 3.0f)).Extents, NxFr::Vector3f(4.0f, 3.5f, 3.0f));
		ASSERT_EQ(Cube.Rotate(NxFr::Euler(45.0f, 30.0f, 60.0)).Orientation, NxFr::Euler(45.0f, 30.0f, 60.0));

		NxFr::Sphere Sphere = NxFr::Sphere(NxFr::Vector3f::Zero, 1.0f);
		ASSERT_EQ(Sphere.Translate(NxFr::Vector3f(0.0f, 1.0f)).Center, NxFr::Vector3f::Up);
		ASSERT_EQ(Sphere.Scale(2.0f).Radius, 2.0f);
		ASSERT_EQ(Sphere.Grow(1.0f).Radius, 3.0f);
		ASSERT_EQ(Sphere.GetDiameter(), 6.0f);
		ASSERT_EQ(Sphere.Encapsulate(NxFr::Sphere(NxFr::Vector3f::Up * 4.0f, 3.0f)).Radius, 4.5f);

		Ray = NxFr::Ray(NxFr::Vector3f::Zero, NxFr::Vector3f::Forward);
		Plane = NxFr::Plane(NxFr::Vector3f::Forward, 0.0f);
		Triangle = NxFr::Triangle(NxFr::Vector3f(1.0f, 0.0f, 0.0f), NxFr::Vector3f(-1.0f, 0.0f, 0.0f), NxFr::Vector3f(0.0f, 1.0f, 0.0f));
		Rectangle = NxFr::Rectangle(NxFr::Vector2f::Zero, NxFr::Vector2f::One);
		Circle = NxFr::Circle(NxFr::Vector3f::Zero, 1.0f);
		Box = NxFr::Box(NxFr::Vector3f::Zero, NxFr::Vector3f::One);
		Cube = NxFr::Cube(NxFr::Vector3f::Zero, NxFr::Vector3f::One);
		Sphere = NxFr::Sphere(NxFr::Vector3f::Zero, 1.0f);

		ASSERT_EQ(NxFr::Circle(Rectangle), NxFr::Circle(NxFr::Math::Sqrt(2.0f)));
		ASSERT_EQ(NxFr::Rectangle(Circle), NxFr::Rectangle(NxFr::Vector2f::One));
		ASSERT_EQ(NxFr::Sphere(Box), NxFr::Sphere(NxFr::Math::Sqrt(3.0f)));
		ASSERT_EQ(NxFr::Sphere(Cube), NxFr::Sphere(NxFr::Math::Sqrt(3.0f)));
		ASSERT_EQ(NxFr::Box(Sphere), NxFr::Box(NxFr::Vector3f::One));
		ASSERT_EQ(NxFr::Cube(Sphere), NxFr::Cube(NxFr::Vector3f::One));
		ASSERT_EQ(NxFr::Cube(Box), NxFr::Cube(NxFr::Vector3f::One));
		ASSERT_EQ(NxFr::Box(Cube), NxFr::Box(NxFr::Vector3f::One));

		ASSERT_EQ(NxFr::ShapeUtility::Position(Ray, 2.0f), NxFr::Vector3f::Forward * 2.0f);
		ASSERT_EQ(NxFr::ShapeUtility::Position(Plane, NxFr::Vector2f(0.5f, 0.5f)), NxFr::Vector3f::Zero);
		ASSERT_EQ(NxFr::ShapeUtility::Position(Triangle, NxFr::Vector2f(0.0f, 0.0f)), NxFr::Vector3f(0.0f, 1.0f, 0.0f));
		ASSERT_EQ(NxFr::ShapeUtility::Position(Rectangle, NxFr::Vector2f(0.5f, 0.5f)), NxFr::Vector2f::Zero);
		ASSERT_EQ(NxFr::ShapeUtility::Position(Circle, 0.25f), NxFr::Vector2f::Up);
		ASSERT_EQ(NxFr::ShapeUtility::Position(Box, NxFr::Vector3f(0.5f, 0.5f, 0.5f)), NxFr::Vector3f::Zero);
		ASSERT_EQ(NxFr::ShapeUtility::Position(Cube, NxFr::Vector3f(0.5f, 0.5f, 0.5f)), NxFr::Vector3f::Zero);
		ASSERT_EQ(NxFr::ShapeUtility::Position(Sphere, NxFr::Vector2f(0.25f, 0.25f)), NxFr::Vector3f(0.0f, 1.0f, -1.0f).Normalized());

		ASSERT_EQ(NxFr::ShapeUtility::Coordinates(Rectangle, NxFr::Vector2f::Zero), NxFr::Vector2f(0.5f, 0.5f));
		ASSERT_EQ(NxFr::Math::Equals(NxFr::ShapeUtility::Coordinates(Circle, NxFr::Vector2f::Up), 0.25f), true);
		ASSERT_EQ(NxFr::ShapeUtility::Coordinates(Box,NxFr::Vector3f::Zero), NxFr::Vector3f(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::ShapeUtility::Coordinates(Cube, NxFr::Vector3f::Zero), NxFr::Vector3f(0.5f, 0.5f, 0.5f));
		ASSERT_EQ(NxFr::ShapeUtility::Coordinates(Sphere, NxFr::Vector3f(0.0f, 1.0f, -1.0f).Normalized()), NxFr::Vector2f(0.25f, 0.25f));

		ASSERT_EQ(NxFr::ShapeUtility::Side(Ray, NxFr::Vector3f::Forward), true);
		ASSERT_EQ(NxFr::ShapeUtility::Side(Plane, NxFr::Vector3f::Forward), true);
		ASSERT_EQ(NxFr::ShapeUtility::Side(Triangle, NxFr::Vector3f::Forward), true);

		ASSERT_EQ(NxFr::ShapeUtility::Contains(Ray, NxFr::Vector3f::Forward * 2.0f), true);
		ASSERT_EQ(NxFr::ShapeUtility::Contains(Plane, NxFr::Vector3f(0.5f, 0.5f, 0.0f)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Contains(Triangle, NxFr::Vector3f(0.5f, 0.5f, 0.0f)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Contains(Rectangle, NxFr::Vector3f(0.5f, 0.5f, 0.5f)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Contains(Circle, NxFr::Vector3f(0.5f, 0.5f, 0.5f)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Contains(Box, NxFr::Vector3f(0.5f, 0.5f, 0.5f)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Contains(Cube, NxFr::Vector3f(0.5f, 0.5f, 0.5f)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Contains(Sphere, NxFr::Vector3f(0.5f, 0.5f, 0.5f)), true);

		ASSERT_EQ(NxFr::ShapeUtility::Overlap(Rectangle, NxFr::Rectangle(NxFr::Vector2f::Right, NxFr::Vector2f::One)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Overlap(Circle, NxFr::Circle(NxFr::Vector2f::Right, 1.0f)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Overlap(Box, NxFr::Box(NxFr::Vector3f::Right, NxFr::Vector3f::One)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Overlap(Cube, NxFr::Cube(NxFr::Vector3f::Right, NxFr::Vector3f::One)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Overlap(Sphere, NxFr::Sphere(NxFr::Vector3f::Right, 1.0f)), true);

		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Plane, Ray), true);
		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Triangle, Ray), true);
		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Triangle, NxFr::Plane(NxFr::Vector3f::Up, -0.5f)), true);
		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Rectangle, Ray), true);
		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Circle, Ray), true);
		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Box, Ray), true);
		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Box, Plane), true);
		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Cube, Ray), true);
		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Cube, Plane), true);
		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Sphere, Ray), true);
		ASSERT_EQ(NxFr::ShapeUtility::Intersect(Sphere, Plane), true);

		ASSERT_EQ(NxFr::ShapeUtility::Closest(Ray, NxFr::Vector3f::Up), NxFr::Vector3f::Zero);
		ASSERT_EQ(NxFr::ShapeUtility::Closest(Plane, NxFr::Vector3f(0.0f, 0.0f, 1.0f)), NxFr::Vector3f::Zero);
		ASSERT_EQ(NxFr::ShapeUtility::Closest(Triangle, NxFr::Vector3f(0.0f, 0.0f, 1.0f)), NxFr::Vector3f::Zero);
		ASSERT_EQ(NxFr::ShapeUtility::Closest(Rectangle, NxFr::Vector3f(0.0f, 1.5f, 0.0f)), NxFr::Vector3f::Up);
		ASSERT_EQ(NxFr::ShapeUtility::Closest(Circle, NxFr::Vector3f(0.0f, 1.5f, 0.0f)), NxFr::Vector3f::Up);
		ASSERT_EQ(NxFr::ShapeUtility::Closest(Box, NxFr::Vector3f(0.0f, 1.5f, 0.0f)), NxFr::Vector3f::Up);
		ASSERT_EQ(NxFr::ShapeUtility::Closest(Cube, NxFr::Vector3f(0.0f, 1.5f, 0.0f)), NxFr::Vector3f::Up);
		ASSERT_EQ(NxFr::ShapeUtility::Closest(Sphere, NxFr::Vector3f(0.0f, 1.5f, 0.0f)), NxFr::Vector3f::Up);

		ASSERT_EQ(NxFr::Math::Equals(NxFr::ShapeUtility::Distance(Ray, NxFr::Vector3f::Up), 1.0f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::ShapeUtility::Distance(Plane, NxFr::Vector3f(0.0f, 0.0f, 1.0f)), 1.0f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::ShapeUtility::Distance(Triangle, NxFr::Vector3f(0.0f, 0.0f, 1.0f)), 1.0f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::ShapeUtility::Distance(Rectangle, NxFr::Vector3f(0.0f, 1.5f, 0.0f)), 0.5f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::ShapeUtility::Distance(Circle, NxFr::Vector3f(0.0f, 1.5f, 0.0f)), 0.5f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::ShapeUtility::Distance(Box, NxFr::Vector3f(0.0f, 1.5f, 0.0f)), 0.5f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::ShapeUtility::Distance(Cube, NxFr::Vector3f(0.0f, 1.5f, 0.0f)), 0.5f), true);
		ASSERT_EQ(NxFr::Math::Equals(NxFr::ShapeUtility::Distance(Sphere, NxFr::Vector3f(0.0f, 1.5f, 0.0f)), 0.5f), true);
	}
}
