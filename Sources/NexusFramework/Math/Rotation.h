#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Numeric/Decimal.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/Utility.h"

#include "NexusFramework/Math/Math.h"
#include "NexusFramework/Math/Vector.h"

namespace NxFr
{
#pragma region Declaration

	struct Quaternion;

	namespace RotationUtility
	{
		float SqrMagnitude(Quaternion Q);
		float Magnitude(Quaternion Q);
		Quaternion Normalize(Quaternion Q);
		Quaternion Inverse(Quaternion Q);
		Quaternion Concatenate(Quaternion A, Quaternion B);
		Vector<3, float> Rotate(Quaternion Q, Vector<3, float> V);
	}

#pragma endregion

#pragma region AxisAngle

	struct AxisAngle
	{
	public:
		AxisAngle()
			: Axis(0), Angle(0)
		{
		}

		AxisAngle(Vector<3, float> Axis, float Angle)
			: Axis(Axis), Angle(Angle)
		{
		}

		operator Vector<4, float>() const { return Vector<4, float>(Axis.x, Axis.y, Axis.z, Angle); }

		bool operator==(AxisAngle Other) const { return Axis == Other.Axis && Math::Equals(Angle, Other.Angle); }
		bool operator!=(AxisAngle Other) const { return !(*this == Other); }

	public:
		Vector<3, float> Axis;
		float Angle;
	};

#pragma endregion

#pragma region Euler

	struct Euler
	{
	public:
		static const Euler Identity;

		Euler()
			: x(0), y(0), z(0)
		{
		}

		Euler(float X, float Y, float Z)
			: x(X), y(Y), z(Z)
		{
		}

		Euler(Vector<3, float> Angles)
			: x(Angles.x), y(Angles.y), z(Angles.z)
		{
		}

		operator Vector<3, float>() const { return Vector<3, float>(x, y, z); }

		bool operator==(Euler Other) const { return Math::Equals(x, Other.x) && Math::Equals(y, Other.y) && Math::Equals(z, Other.z); }
		bool operator!=(Euler Other) const { return !(*this == Other); }
		float& operator[](uint8 Index) { return Index == 0 ? x : Index == 1 ? y : Index == 2 ? z : x; }
		const float& operator[](uint8 Index) const { return Index == 0 ? x : Index == 1 ? y : Index == 2 ? z : x; }

	public:
		float x, y, z;
		//float Pitch, Yaw, Roll;
	};

	inline const Euler Euler::Identity = Euler(0, 0, 0);

#pragma endregion

#pragma region Quaternion

	struct Quaternion
	{
	public:
		static const Quaternion Identity;

		Quaternion()
			: x(0), y(0), z(0), w(1)
		{
		}

		Quaternion(AxisAngle AngleAxis)
			: x(0), y(0), z(0), w(1)
		{
			AngleAxis.Angle *= Math::Radians;

			float Half = AngleAxis.Angle * 0.5f;
			float SinTheta = Math::Sin(Half);
			float CosTheta = Math::Cos(Half);

			w = CosTheta;
			x = SinTheta * AngleAxis.Axis.x;
			y = SinTheta * AngleAxis.Axis.y;
			z = SinTheta * AngleAxis.Axis.z;
		}

		Quaternion(Euler EulerAngle)
			: x(0), y(0), z(0), w(1)
		{
			EulerAngle.x *= Math::Radians * 0.5f;
			EulerAngle.y *= Math::Radians * 0.5f;
			EulerAngle.z *= Math::Radians * 0.5f;

			float CosPitch = Math::Cos(EulerAngle.x);
			float SinPitch = Math::Sin(EulerAngle.x);
			float CosYaw = Math::Cos(EulerAngle.y);
			float SinYaw = Math::Sin(EulerAngle.y);
			float CosRoll = Math::Cos(EulerAngle.z);
			float SinRoll = Math::Sin(EulerAngle.z);

			w = CosPitch * CosYaw * CosRoll + SinPitch * SinYaw * SinRoll;
			x = SinPitch * CosYaw * CosRoll + CosPitch * SinYaw * SinRoll;
			y = CosPitch * SinYaw * CosRoll - SinPitch * CosYaw * SinRoll;
			z = CosPitch * CosYaw * SinRoll - SinPitch * SinYaw * CosRoll;
		}

		Quaternion(float X, float Y, float Z, float W)
			: x(X), y(Y), z(Z), w(W)
		{
		}

		operator AxisAngle() const
		{
			float Length = Math::Sqrt(1.0f - Math::Square(w));
			float Angle = 2.0f * Math::Acos(w);
			Vector<3, float> Axis = (1.0f / Length) * Vector<3, float>(x, y, z);
			return AxisAngle(Axis, Angle * Math::Degree);
		}

		operator Euler() const
		{
			float Yaw = 0.0f;
			float SinYaw = 2 * (w * y + x * z);
			float CosYaw = 1 - 2 * (y * y + x * x);
			if (Math::Equals(Math::Abs(SinYaw), 0.0f) && Math::Equals(Math::Abs(CosYaw), 0.0f))
				Yaw = 2.0f * Math::Atan(y, w);
			else
				Yaw = Math::Atan(SinYaw, CosYaw);

			float SinPitch = 2.0f * Math::Clamp(w * x - y * z, -1.0f, 1.0f);
			float Pitch  = Math::Asin(SinPitch);

			float Roll = 0.0f;
			float SinRoll = 2 * (w * z + x * y);
			float CosRoll = 1 - 2 * (x * x + z * z);
			if (Math::Equals(Math::Abs(SinRoll), 0.0f) && Math::Equals(Math::Abs(CosRoll), 0.0f))
				Roll = 0.0f;
			else
				Roll = Math::Atan(SinRoll, CosRoll);

			return Euler(Pitch * Math::Degree, Yaw * Math::Degree, Roll * Math::Degree);
		}

		operator Vector<4, float>() const { return Vector<4, float>(x, y, z, w); }

		bool operator==(Quaternion Other) const { return Math::Equals(x, Other.x) && Math::Equals(y, Other.y) && Math::Equals(z, Other.z) && Math::Equals(w, Other.w); }
		bool operator!=(Quaternion Other) const { return !(*this == Other); }
		float& operator[](uint8 Index) { return Index == 0 ? x : Index == 1 ? y : Index == 2 ? z : Index == 3 ? w : x; }
		const float& operator[](uint8 Index) const { return Index == 0 ? x : Index == 1 ? y : Index == 2 ? z : Index == 3 ? w : x; }

		Quaternion& operator*=(Quaternion Other) { *this = RotationUtility::Concatenate(*this, Other); return *this; }

		Quaternion Normalized() const { return RotationUtility::Normalize(*this); }
		Quaternion Inverse() const { return RotationUtility::Inverse(*this); }
		float SqrMagnitude() const { return RotationUtility::SqrMagnitude(*this); }
		float Magnitude() const { return RotationUtility::Magnitude(*this); }

	public:
		float x, y, z, w;
	};

	inline const Quaternion Quaternion::Identity = Quaternion(0, 0, 0, 1);

	inline Quaternion operator-(Quaternion Q)
	{
		return RotationUtility::Inverse(Q);
	}

	inline Quaternion operator*(Quaternion A, Quaternion B)
	{
		return RotationUtility::Concatenate(A, B);
	}

	inline Vector<3, float> operator*(Quaternion Q, Vector<3, float> V)
	{
		return RotationUtility::Rotate(Q, V);
	}

	inline Vector<3, float> operator*(Vector<3, float> V, Quaternion Q)
	{
		return RotationUtility::Rotate(Q, V);
	}

#pragma endregion

#pragma region RotationUtility

	namespace RotationUtility
	{
		inline float Dot(Quaternion A, Quaternion B)
		{
			return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
		}

		inline float SqrMagnitude(Quaternion Q)
		{
			return Dot(Q, Q);
		}

		inline float Magnitude(Quaternion Q)
		{
			return Math::Sqrt(Dot(Q, Q));
		}

		inline Quaternion Normalize(Quaternion Q)
		{
			float Size = SqrMagnitude(Q);
			if (Math::Equals(Size, 0.0f))
			{
				return Quaternion::Identity;
			}

			Size = 1.0f / Math::Sqrt(Size);
			return Quaternion(Size * Q.x, Size * Q.y, Size * Q.z, Size * Q.w);
		}

		inline Quaternion Inverse(Quaternion Q)
		{
			return Quaternion(-Q.x, -Q.y, -Q.z, Q.w);
		}

		inline float Angle(Quaternion A, Quaternion B)
		{
			return 2.0f * Math::Acos(Dot(A, B));
		}

		inline Quaternion Lerp(Quaternion A, Quaternion B, float V)
		{
			float InvV = 1.0f - V;
			A = Quaternion(InvV * A.x, InvV * A.y, InvV * A.z, InvV * A.w);
			B = Quaternion(V * B.x, V * B.y, V * B.z, V * B.w);
			return Normalize(Quaternion(A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w));
		}

		inline Quaternion Slerp(Quaternion A, Quaternion B, float V)
		{
			float ADotB = Dot(A, B);
			if (ADotB < 0.0f)
			{
				ADotB = -ADotB;
				B = -B;
			}

			if (Math::Equals(ADotB, 1.0f))
			{
				return Lerp(A, B, V);
			}

			float Theta = Math::Acos(Dot(A, B));
			float SinTheta = Math::Sin(Theta);

			float WeightA = Math::Sin((1.0f - V) * Theta) / SinTheta;
			float WeightB = Math::Sin(V * Theta) / SinTheta;

			A = Quaternion(WeightA * A.x, WeightA * A.y, WeightA * A.z, WeightA * A.w);
			B = Quaternion(WeightB * B.x, WeightB * B.y, WeightB * B.z, WeightB * B.w);
			return Normalize(Quaternion(A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w));
		}

		inline bool Similar(Quaternion A, Quaternion B)
		{
			return Math::Equals(Math::Abs(Dot(A, B)), 1.0f);
		}

		inline Quaternion Concatenate(Quaternion A, Quaternion B)
		{
			Quaternion Result;
			Result.w = A.w * B.w - A.x * B.x - A.y * B.y - A.z * B.z;
			Result.x = A.w * B.x + A.x * B.w + A.y * B.z - A.z * B.y;
			Result.y = A.w * B.y + A.y * B.w + A.z * B.x - A.x * B.z;
			Result.z = A.w * B.z + A.z * B.w + A.x * B.y - A.y * B.x;
			return Result;
		}

		inline Vector<3, float> Rotate(Quaternion Q, Vector<3, float> V)
		{
			float CMult = 2.0f * Q.w;
			float VMult = 2.0f * (Q.x * V.x + Q.y * V.y + Q.z * V.z);
			float PMult = CMult * Q.w - 1.0f;

			return Vector<3, float>(
				PMult * V.x + VMult * Q.x + CMult * (Q.y * V.z - Q.z * V.y),
				PMult * V.y + VMult * Q.y + CMult * (Q.z * V.x - Q.x * V.z),
				PMult * V.z + VMult * Q.z + CMult * (Q.x * V.y - Q.y * V.x)
			);
		}

		inline Quaternion FromTo(Vector<3, float> From, Vector<3, float> To)
		{
			float Angle = VectorUtility::Dot(From, To);

			if (Math::Equals(Angle, 1.0f))
			{
				return Quaternion::Identity;
			}
			else if (Math::Equals(Angle, -1.0f))
			{
				Vector<3, float> Axis = VectorUtility::Orthogonal(From).Normalized();
				return Quaternion({ Axis, 0 });
			}

			Vector<3, float> Axis = VectorUtility::Cross(From, To);
			float Scale = Math::Sqrt((1.0f + Angle) * 2.0f);
			float Inv = 1.0f / Scale;

			return Quaternion(Inv * Axis.x, Inv * Axis.y, Inv * Axis.z, 0.5f * Scale);
		}
	}

#pragma endregion
}
