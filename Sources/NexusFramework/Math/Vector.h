#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Numbers/Decimal.h"
#include "NexusFramework/Math/Math.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"

namespace NxFr
{
#pragma warning(push)
#pragma warning(disable: 4244)

#pragma region Declaration

	template<uint8 D, typename T>
	struct Vector;

	using Vector2f = Vector<2, float>;
	using Vector2i = Vector<2, int32>;
	using Vector3f = Vector<3, float>;
	using Vector3i = Vector<3, int32>;
	using Vector4f = Vector<4, float>;
	using Vector4i = Vector<4, int32>;

	using Vector2 = Vector2f;
	using Vector3 = Vector3f;
	using Vector4 = Vector4f;

	namespace VectorUtility
	{
		template<uint8 D, typename T>	float				SqrMagnitude(Vector<D, T> V);
		template<uint8 D, typename T>	float				Magnitude(Vector<D, T> V);
		template<uint8 D, typename T>	Vector<D, float>	Normalize(Vector<D, T> V);
	}

#pragma endregion

#pragma region Vector

	template<uint8 D, typename T>
	struct Vector
	{
	public:
		Vector() {}
		template<uint8 S, typename P> Vector(Vector<S, P> Other) {}
		~Vector() {}

		Vector<D, T>& operator=(Vector<D, T> Other) { return *this; }
		bool operator==(Vector<D, T> Other) const { return false; }
		bool operator!=(Vector<D, T> Other) const { return !(*this == Other); }
		T& operator[](uint8 Index) { return 0; }
		const T& operator[](uint8 Index) const { return 0; }

		Vector<D, T>& operator+=(T Other) { return *this; }
		Vector<D, T>& operator-=(T Other) { return *this; }
		Vector<D, T>& operator*=(T Other) { return *this; }
		Vector<D, T>& operator/=(T Other) { return *this; }
		Vector<D, T>& operator+=(Vector<D, T> Other) { return *this; }
		Vector<D, T>& operator-=(Vector<D, T> Other) { return *this; }
		Vector<D, T>& operator*=(Vector<D, T> Other) { return *this; }
		Vector<D, T>& operator/=(Vector<D, T> Other) { return *this; }

		Vector<D, T> Normalized() const { return VectorUtility::Normalize(*this); }
		float SqrMagnitude() const { return VectorUtility::SqrMagnitude(*this); }
		float Magnitude() const { return VectorUtility::Magnitude(*this); }

		String ToString() const { return ""; }
	};

#pragma endregion

#pragma region Vector2

	template<typename T>
	struct Vector<2, T>
	{
	public:
		static const Vector<2, T> Zero;
		static const Vector<2, T> One;
		static const Vector<2, T> Infinity;
		static const Vector<2, T> Right;
		static const Vector<2, T> Up;

		Vector() : x(0), y(0) {}
		template<typename P> explicit Vector(P V) : x(V), y(V) {}
		template<typename P> explicit Vector(P X, P Y) : x(X), y(Y) {}
		template<typename P> Vector(Vector<2, P> Other) : x(Other.x), y(Other.y) {}
		template<typename P> Vector(Vector<3, P> Other) : x(Other.x), y(Other.y) {}
		template<typename P> Vector(Vector<4, P> Other) : x(Other.x), y(Other.y) {}
		~Vector() {}

		Vector<2, T>& operator=(Vector<2, T> Other) { x = Other.x; y = Other.y; return *this; }
		bool operator==(Vector<2, T> Other) const { return x == Other.x && y == Other.y; }
		bool operator!=(Vector<2, T> Other) const { return !(*this == Other); }
		T& operator[](uint8 Index) { return Index == 0 ? x : Index == 1 ? y : x; }
		const T& operator[](uint8 Index) const { return Index == 0 ? x : Index == 1 ? y : x; }

		Vector<2, T>& operator+=(T Other) { x += Other; y += Other; return *this; }
		Vector<2, T>& operator-=(T Other) { x -= Other; y -= Other; return *this; }
		Vector<2, T>& operator*=(T Other) { x *= Other; y *= Other; return *this; }
		Vector<2, T>& operator/=(T Other) { x /= Other; y /= Other; return *this; }
		Vector<2, T>& operator+=(Vector<2, T> Other) { x += Other.x; y += Other.y; return *this; }
		Vector<2, T>& operator-=(Vector<2, T> Other) { x -= Other.x; y -= Other.y; return *this; }
		Vector<2, T>& operator*=(Vector<2, T> Other) { x *= Other.x; y *= Other.y; return *this; }
		Vector<2, T>& operator/=(Vector<2, T> Other) { x /= Other.x; y /= Other.y; return *this; }

		Vector<2, T> Normalized() const { return VectorUtility::Normalize(*this); }
		float SqrMagnitude() const { return VectorUtility::SqrMagnitude(*this); }
		float Magnitude() const { return VectorUtility::Magnitude(*this); }

		String ToString() const { return StringUtility::Format("(%.2f, %.2f)", (float)x, (float)y); }

	public:
		T x, y;
	};

	template<typename T> const Vector<2, T> Vector<2, T>::Zero = Vector<2, T>(0);
	template<typename T> const Vector<2, T> Vector<2, T>::One = Vector<2, T>(1);
	template<typename T> const Vector<2, T> Vector<2, T>::Infinity = Vector<2, T>(Decimal::InfinityF());
	template<typename T> const Vector<2, T> Vector<2, T>::Right = Vector<2, T>(1, 0);
	template<typename T> const Vector<2, T> Vector<2, T>::Up = Vector<2, T>(0, 1);

	template<typename T> Vector<2, T> operator-(Vector<2, T> A) { return Vector<2, T>(-A.x, -A.y); }
	template<typename T> Vector<2, T> operator+(T A, Vector<2, T> B) { return Vector<2, T>(A + B.x, A + B.y); }
	template<typename T> Vector<2, T> operator-(T A, Vector<2, T> B) { return Vector<2, T>(A - B.x, A - B.y); }
	template<typename T> Vector<2, T> operator*(T A, Vector<2, T> B) { return Vector<2, T>(A * B.x, A * B.y); }
	template<typename T> Vector<2, T> operator/(T A, Vector<2, T> B) { return Vector<2, T>(A / B.x, A / B.y); }
	template<typename T> Vector<2, T> operator+(Vector<2, T> A, T B) { return Vector<2, T>(A.x + B, A.y + B); }
	template<typename T> Vector<2, T> operator-(Vector<2, T> A, T B) { return Vector<2, T>(A.x - B, A.y - B); }
	template<typename T> Vector<2, T> operator*(Vector<2, T> A, T B) { return Vector<2, T>(A.x * B, A.y * B); }
	template<typename T> Vector<2, T> operator/(Vector<2, T> A, T B) { return Vector<2, T>(A.x / B, A.y / B); }
	template<typename T> Vector<2, T> operator+(Vector<2, T> A, Vector<2, T> B) { return Vector<2, T>(A.x + B.x, A.y + B.y); }
	template<typename T> Vector<2, T> operator-(Vector<2, T> A, Vector<2, T> B) { return Vector<2, T>(A.x - B.x, A.y - B.y); }
	template<typename T> Vector<2, T> operator*(Vector<2, T> A, Vector<2, T> B) { return Vector<2, T>(A.x * B.x, A.y * B.y); }
	template<typename T> Vector<2, T> operator/(Vector<2, T> A, Vector<2, T> B) { return Vector<2, T>(A.x / B.x, A.y / B.y); }

#pragma endregion

#pragma region Vector3

	template<typename T>
	struct Vector<3, T>
	{
	public:
		static const Vector<3, T> Zero;
		static const Vector<3, T> One;
		static const Vector<3, T> Infinity;
		static const Vector<3, T> Right;
		static const Vector<3, T> Up;
		static const Vector<3, T> Forward;

		Vector() : x(0), y(0), z(0) {}
		template<typename P> explicit Vector(P V) : x(V), y(V), z(V) {}
		template<typename P> explicit Vector(P X, P Y) : x(X), y(Y), z(0) {}
		template<typename P> explicit Vector(P X, P Y, P Z) : x(X), y(Y), z(Z) {}
		template<typename P> explicit Vector(Vector<2, P> Other) : x(Other.x), y(Other.y), z(0) {}
		template<typename P> Vector(Vector<3, P> Other) : x(Other.x), y(Other.y), z(Other.z) {}
		template<typename P> Vector(Vector<4, P> Other) : x(Other.x), y(Other.y), z(Other.z) {}
		~Vector() {}

		Vector<3, T>& operator=(Vector<3, T> Other) { x = Other.x; y = Other.y; z = Other.z; return *this; }
		bool operator==(Vector<3, T> Other) const { return x == Other.x && y == Other.y && z == Other.z; }
		bool operator!=(Vector<3, T> Other) const { return !(*this == Other); }
		T& operator[](uint8 Index) { return Index == 0 ? x : Index == 1 ? y : Index == 2 ? z : x; }
		const T& operator[](uint8 Index) const { return Index == 0 ? x : Index == 1 ? y : Index == 2 ? z : x; }

		Vector<3, T>& operator+=(T Other) { x += Other; y += Other; z += Other; return *this; }
		Vector<3, T>& operator-=(T Other) { x -= Other; y -= Other; z -= Other; return *this; }
		Vector<3, T>& operator*=(T Other) { x *= Other; y *= Other; z *= Other; return *this; }
		Vector<3, T>& operator/=(T Other) { x /= Other; y /= Other; z /= Other; return *this; }
		Vector<3, T>& operator+=(Vector<3, T> Other) { x += Other.x; y += Other.y; z += Other.z; return *this; }
		Vector<3, T>& operator-=(Vector<3, T> Other) { x -= Other.x; y -= Other.y; z -= Other.z; return *this; }
		Vector<3, T>& operator*=(Vector<3, T> Other) { x *= Other.x; y *= Other.y; z *= Other.z; return *this; }
		Vector<3, T>& operator/=(Vector<3, T> Other) { x /= Other.x; y /= Other.y; z /= Other.z; return *this; }

		Vector<3, T> Normalized() const { return VectorUtility::Normalize(*this); }
		float SqrMagnitude() const { return VectorUtility::SqrMagnitude(*this); }
		float Magnitude() const { return VectorUtility::Magnitude(*this); }

		String ToString() const { return StringUtility::Format("(%.2f, %.2f, %.2f)", (float)x, (float)y, (float)z); }

	public:
		T x, y, z;
	};

	template<typename T> const Vector<3, T> Vector<3, T>::Zero = Vector<3, T>(0);
	template<typename T> const Vector<3, T> Vector<3, T>::One = Vector<3, T>(1);
	template<typename T> const Vector<3, T> Vector<3, T>::Infinity = Vector<3, T>(Decimal::InfinityF());
	template<typename T> const Vector<3, T> Vector<3, T>::Right = Vector<3, T>(1, 0, 0);
	template<typename T> const Vector<3, T> Vector<3, T>::Up = Vector<3, T>(0, 1, 0);
	template<typename T> const Vector<3, T> Vector<3, T>::Forward = Vector<3, T>(0, 0, 1);

	template<typename T> Vector<3, T> operator-(Vector<3, T> A) { return Vector<3, T>(-A.x, -A.y, -A.z); }
	template<typename T> Vector<3, T> operator+(T A, Vector<3, T> B) { return Vector<3, T>(A + B.x, A + B.y, A + B.z); }
	template<typename T> Vector<3, T> operator-(T A, Vector<3, T> B) { return Vector<3, T>(A - B.x, A - B.y, A - B.z); }
	template<typename T> Vector<3, T> operator*(T A, Vector<3, T> B) { return Vector<3, T>(A * B.x, A * B.y, A * B.z); }
	template<typename T> Vector<3, T> operator/(T A, Vector<3, T> B) { return Vector<3, T>(A / B.x, A / B.y, A / B.z); }
	template<typename T> Vector<3, T> operator+(Vector<3, T> A, T B) { return Vector<3, T>(A.x + B, A.y + B, A.z + B); }
	template<typename T> Vector<3, T> operator-(Vector<3, T> A, T B) { return Vector<3, T>(A.x - B, A.y - B, A.z - B); }
	template<typename T> Vector<3, T> operator*(Vector<3, T> A, T B) { return Vector<3, T>(A.x * B, A.y * B, A.z * B); }
	template<typename T> Vector<3, T> operator/(Vector<3, T> A, T B) { return Vector<3, T>(A.x / B, A.y / B, A.z / B); }
	template<typename T> Vector<3, T> operator+(Vector<3, T> A, Vector<3, T> B) { return Vector<3, T>(A.x + B.x, A.y + B.y, A.z + B.z); }
	template<typename T> Vector<3, T> operator-(Vector<3, T> A, Vector<3, T> B) { return Vector<3, T>(A.x - B.x, A.y - B.y, A.z - B.z); }
	template<typename T> Vector<3, T> operator*(Vector<3, T> A, Vector<3, T> B) { return Vector<3, T>(A.x * B.x, A.y * B.y, A.z * B.z); }
	template<typename T> Vector<3, T> operator/(Vector<3, T> A, Vector<3, T> B) { return Vector<3, T>(A.x / B.x, A.y / B.y, A.z / B.z); }

#pragma endregion

#pragma region Vector4

	template<typename T>
	struct Vector<4, T>
	{
	public:
		static const Vector<4, T> Zero;
		static const Vector<4, T> One;
		static const Vector<4, T> Infinity;
		static const Vector<4, T> Right;
		static const Vector<4, T> Up;
		static const Vector<4, T> Forward;
		static const Vector<4, T> Fourth;

		Vector() : x(0), y(0), z(0), w(0) {}
		template<typename P> explicit Vector(P V) : x(V), y(V), z(V), w(V) {}
		template<typename P> explicit Vector(P X, P Y) : x(X), y(Y), z(0), w(0) {}
		template<typename P> explicit Vector(P X, P Y, P Z) : x(X), y(Y), z(Z), w(0) {}
		template<typename P> explicit Vector(P X, P Y, P Z, P W) : x(X), y(Y), z(Z), w(W) {}
		template<typename P> Vector(Vector<2, P> Other) : x(Other.x), y(Other.y), z(0), w(0) {}
		template<typename P> Vector(Vector<3, P> Other) : x(Other.x), y(Other.y), z(Other.z), w(0) {}
		template<typename P> Vector(Vector<4, P> Other) : x(Other.x), y(Other.y), z(Other.z), w(Other.w) {}
		~Vector() {}

		Vector<4, T>& operator=(Vector<4, T> Other) { x = Other.x; y = Other.y; z = Other.z; w = Other.w; return *this; }
		bool operator==(Vector<4, T> Other) const { return x == Other.x && y == Other.y && z == Other.z && w == Other.w; }
		bool operator!=(Vector<4, T> Other) const { return !(*this == Other); }
		T& operator[](uint8 Index) { return Index == 0 ? x : Index == 1 ? y : Index == 2 ? z : Index == 3 ? w : x; }
		const T& operator[](uint8 Index) const { return Index == 0 ? x : Index == 1 ? y : Index == 2 ? z : Index == 3 ? w : x; }

		Vector<4, T>& operator+=(T Other) { x += Other; y += Other; z += Other; w += Other; return *this; }
		Vector<4, T>& operator-=(T Other) { x -= Other; y -= Other; z -= Other; w -= Other; return *this; }
		Vector<4, T>& operator*=(T Other) { x *= Other; y *= Other; z *= Other; w *= Other; return *this; }
		Vector<4, T>& operator/=(T Other) { x /= Other; y /= Other; z /= Other; w /= Other; return *this; }
		Vector<4, T>& operator+=(Vector<4, T> Other) { x += Other.x; y += Other.y; z += Other.z; w += Other.w; return *this; }
		Vector<4, T>& operator-=(Vector<4, T> Other) { x -= Other.x; y -= Other.y; z -= Other.z; w -= Other.w; return *this; }
		Vector<4, T>& operator*=(Vector<4, T> Other) { x *= Other.x; y *= Other.y; z *= Other.z; w *= Other.w; return *this; }
		Vector<4, T>& operator/=(Vector<4, T> Other) { x /= Other.x; y /= Other.y; z /= Other.z; w /= Other.w; return *this; }

		Vector<4, T> Normalized() const { return VectorUtility::Normalize(*this); }
		float SqrMagnitude() const { return VectorUtility::SqrMagnitude(*this); }
		float Magnitude() const { return VectorUtility::Magnitude(*this); }

		String ToString() const { return StringUtility::Format("(%.2f, %.2f, %.2f, %.2f)", (float)x, (float)y, (float)z, (float)w); }

	public:
		T x, y, z, w;
	};

	template<typename T> const Vector<4, T> Vector<4, T>::Zero = Vector<4, T>(0);
	template<typename T> const Vector<4, T> Vector<4, T>::One = Vector<4, T>(1);
	template<typename T> const Vector<4, T> Vector<4, T>::Infinity = Vector<4, T>(Decimal::InfinityF());
	template<typename T> const Vector<4, T> Vector<4, T>::Right = Vector<4, T>(1, 0, 0, 0);
	template<typename T> const Vector<4, T> Vector<4, T>::Up = Vector<4, T>(0, 1, 0, 0);
	template<typename T> const Vector<4, T> Vector<4, T>::Forward = Vector<4, T>(0, 0, 1, 0);
	template<typename T> const Vector<4, T> Vector<4, T>::Fourth = Vector<4, T>(0, 0, 0, 1);

	template<typename T> Vector<4, T> operator-(Vector<4, T> A) { return Vector<4, T>(-A.x, -A.y, -A.z, -A.w); }
	template<typename T> Vector<4, T> operator+(T A, Vector<4, T> B) { return Vector<4, T>(A + B.x, A + B.y, A + B.z, A + B.w); }
	template<typename T> Vector<4, T> operator-(T A, Vector<4, T> B) { return Vector<4, T>(A - B.x, A - B.y, A - B.z, A - B.w); }
	template<typename T> Vector<4, T> operator*(T A, Vector<4, T> B) { return Vector<4, T>(A * B.x, A * B.y, A * B.z, A * B.w); }
	template<typename T> Vector<4, T> operator/(T A, Vector<4, T> B) { return Vector<4, T>(A / B.x, A / B.y, A / B.z, A / B.w); }
	template<typename T> Vector<4, T> operator+(Vector<4, T> A, T B) { return Vector<4, T>(A.x + B, A.y + B, A.z + B, A.w + B); }
	template<typename T> Vector<4, T> operator-(Vector<4, T> A, T B) { return Vector<4, T>(A.x - B, A.y - B, A.z - B, A.w - B); }
	template<typename T> Vector<4, T> operator*(Vector<4, T> A, T B) { return Vector<4, T>(A.x * B, A.y * B, A.z * B, A.w * B); }
	template<typename T> Vector<4, T> operator/(Vector<4, T> A, T B) { return Vector<4, T>(A.x / B, A.y / B, A.z / B, A.w / B); }
	template<typename T> Vector<4, T> operator+(Vector<4, T> A, Vector<4, T> B) { return Vector<4, T>(A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w); }
	template<typename T> Vector<4, T> operator-(Vector<4, T> A, Vector<4, T> B) { return Vector<4, T>(A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w); }
	template<typename T> Vector<4, T> operator*(Vector<4, T> A, Vector<4, T> B) { return Vector<4, T>(A.x * B.x, A.y * B.y, A.z * B.z, A.w * B.w); }
	template<typename T> Vector<4, T> operator/(Vector<4, T> A, Vector<4, T> B) { return Vector<4, T>(A.x / B.x, A.y / B.y, A.z / B.z, A.w / B.w); }

#pragma endregion

#pragma region VectorUtility

	namespace VectorUtility
	{
		template<typename T>
		bool Equals(Vector<2, T> A, Vector<2, T> B)
		{
			return Math::Equals(A.x, B.x) && Math::Equals(A.y, B.y);
		}

		template<typename T>
		bool Equals(Vector<3, T> A, Vector<3, T> B)
		{
			return Math::Equals(A.x, B.x) && Math::Equals(A.y, B.y) && Math::Equals(A.z, B.z);
		}

		template<typename T>
		bool Equals(Vector<4, T> A, Vector<4, T> B)
		{
			return Math::Equals(A.x, B.x) && Math::Equals(A.y, B.y) && Math::Equals(A.z, B.z) && Math::Equals(A.w, B.w);
		}

		template<typename T>
		float Dot(Vector<2, T> A, Vector<2, T> B)
		{
			return A.x * B.x + A.y * B.y;
		}

		template<typename T>
		float Dot(Vector<3, T> A, Vector<3, T> B)
		{
			return A.x * B.x + A.y * B.y + A.z * B.z;
		}

		template<typename T>
		float Dot(Vector<4, T> A, Vector<4, T> B)
		{
			return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w;
		}

		template<typename T>
		Vector<2, float> Cross(Vector<2, T> A, Vector<2, T> B)
		{
			return Vector<2, float>(A.x * B.y - B.x * A.y);
		}

		template<typename T>
		Vector<3, float> Cross(Vector<3, T> A, Vector<3, T> B)
		{
			return Vector<3, float>(A.y * B.z - B.y * A.z, A.z * B.x - B.z * A.x, A.x * B.y - B.x * A.y);
		}

		template<typename T>
		Vector<3, float> Orthogonal(Vector<3, T> V)
		{
			if (Math::Abs(V.x) < Math::Abs(V.y) && Math::Abs(V.x) < Math::Abs(V.z))
			{
				return Cross(V, Vector<3, float>(1.0f, 0.0f, 0.0f));
			}
			else if (Math::Abs(V.y) < Math::Abs(V.z))
			{
				return Cross(V, Vector<3, float>(0.0f, 1.0f, 0.0f));
			}
			else
			{
				return Cross(V, Vector<3, float>(0.0f, 0.0f, 1.0f));
			}
		}

		template<uint8 D, typename T>
		Vector<D, float> ProjectOnVector(Vector<D, T> A, Vector<D, T> B)
		{
			return (Dot(A, B) / Dot(B, B)) * B;
		}

		template<uint8 D, typename T>
		Vector<D, float> ProjectOnNormal(Vector<D, T> V, Vector<D, T> N)
		{
			return V - Dot(V, N) * N;
		}

		template<uint8 D, typename T>
		Vector<D, float> Reflect(Vector<D, T> V, Vector<D, T> N)
		{
			return V - 2.0f * Dot(V, N) * N;
		}

		template<uint8 D, typename T>
		float SqrMagnitude(Vector<D, T> V)
		{
			return Dot(V, V);
		}

		template<uint8 D, typename T>
		float Magnitude(Vector<D, T> V)
		{
			return Math::Sqrt(Dot(V, V));
		}

		template<uint8 D, typename T>
		float Distance(Vector<D, T> A, Vector<D, T> B)
		{
			return Magnitude(B - A);
		}

		template<uint8 D, typename T>
		Vector<D, float> Normalize(Vector<D, T> V)
		{
			float Size = SqrMagnitude(V);
			if (Math::Equals(Size, 0.0f))
			{
				return Vector<D, float>();
			}

			Size = 1.0f / Math::Sqrt(Size);
			return Size * Vector<D, float>(V);
		}

		template<uint8 D, typename T>
		float Angle(Vector<D, T> A, Vector<D, T> B)
		{
			return  Math::Acos((Dot(A, B) / (Magnitude(A) * Magnitude(B))));
		}

		template<typename T> Vector<2, T>
		Min(Vector<2, T> A, Vector<2, T> B)
		{
			return Vector<2, T>(Math::Min(A.x, B.x), Math::Min(A.y, B.y));
		}

		template<typename T> Vector<3, T>
		Min(Vector<3, T> A, Vector<3, T> B)
		{
			return Vector<3, T>(Math::Min(A.x, B.x), Math::Min(A.y, B.y), Math::Min(A.z, B.z));
		}

		template<typename T> Vector<4, T>
		Min(Vector<4, T> A, Vector<4, T> B)
		{
			return Vector<4, T>(Math::Min(A.x, B.x), Math::Min(A.y, B.y), Math::Min(A.z, B.z), Math::Min(A.w, B.w));
		}

		template<typename T> Vector<2, T>
		Max(Vector<2, T> A, Vector<2, T> B)
		{
			return Vector<2, T>(Math::Max(A.x, B.x), Math::Max(A.y, B.y));
		}

		template<typename T> Vector<3, T>
		Max(Vector<3, T> A, Vector<3, T> B)
		{
			return Vector<3, T>(Math::Max(A.x, B.x), Math::Max(A.y, B.y), Math::Max(A.z, B.z));
		}

		template<typename T> Vector<4, T>
		Max(Vector<4, T> A, Vector<4, T> B)
		{
			return Vector<4, T>(Math::Max(A.x, B.x), Math::Max(A.y, B.y), Math::Max(A.z, B.z), Math::Max(A.w, B.w));
		}

		template<uint8 D, typename T>
		Vector<D, T> Clamp(Vector<D, T> V, Vector<D, T> A, Vector<D, T> B)
		{
			return Min(Max(V, A), B);
		}

		template<uint8 D, typename T>
		Vector<D, T> Lerp(Vector<D, T> A, Vector<D, T> B, float V)
		{
			return (1.0f - V) * Vector<D, float>(A) + V * Vector<D, float>(B);
		}

		template<uint8 D, typename T>
		Vector<D, float> Slerp(Vector<D, T> A, Vector<D, T> B, float V)
		{
			float ADotB = Dot(A, B);
			if (Math::Equals(Math::Abs(ADotB), 1.0f))
			{
				return Lerp(A, B, V);
			}

			float Theta = Math::Acos(Dot(A, B));
			float SinTheta = Math::Sin(Theta);

			float WeightA = Math::Sin((1.0f - V) * Theta) / SinTheta;
			float WeightB = Math::Sin(V * Theta) / SinTheta;
			return Normalize(WeightA * Vector<D, float>(A) + WeightB * Vector<D, float>(B));
		}

		template<uint8 D, typename T>
		bool Similar(Vector<D, T> A, Vector<D, T> B)
		{
			return Equals(Normalize(A), Normalize(B));
		}

		template<uint8 D, typename T>
		bool Parallel(Vector<D, T> A, Vector<D, T> B)
		{
			return Math::Equals(Math::Abs(Dot(Normalize(A), Normalize(B))), 1.0f);
		}

		template<uint8 D, typename T>
		bool Perpendicular(Vector<D, T> A, Vector<D, T> B)
		{
			return Math::Equals(Math::Abs(Dot(Normalize(A), Normalize(B))), 0.0f);
		}
	}

#pragma endregion

#pragma warning(pop)
}
