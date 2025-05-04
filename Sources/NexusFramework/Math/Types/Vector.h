#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Numbers/Decimal.h"
#include "NexusFramework/Math/Math.h"
#include "NexusFramework/Types/Strings/String.h"

namespace NxFr
{
#pragma warning(push)
#pragma warning(disable: 4244)

#pragma region Vector

	template<uint8 D, typename T>
	struct Vector { };

	using Vec2f = Vector<2, float>;
	using Vec2i = Vector<2, int32>;
	using Vec3f = Vector<3, float>;
	using Vec3i = Vector<3, int32>;
	using Vec4f = Vector<4, float>;
	using Vec4i = Vector<4, int32>;

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

		static bool Equals(const Vector<2, T>& A, const Vector<2, T>& B) { return Math::Equals((float)A.x, (float)B.x) && Math::Equals((float)A.y, (float)B.y) && Math::Equals((float)A.z, (float)B.z); }
		static Vector<2, T> Clamp(const Vector<2, T>& V, const Vector<2, T>& A, const Vector<2, T>& B) { return Min(Max(V, A), B); }
		static Vector<2, T> Min(const Vector<2, T>& A, const Vector<2, T>& B) { return Vector<2, T>(Math::Min(A.x, B.x), Math::Min(A.y, B.y), Math::Min(A.z, B.z)); }
		static Vector<2, T> Max(const Vector<2, T>& A, const Vector<2, T>& B) { return Vector<2, T>(Math::Max(A.x, B.x), Math::Max(A.y, B.y), Math::Max(A.z, B.z)); }
		static Vector<2, T> Lerp(const Vector<2, T>& A, const Vector<2, T>& B, float V) { return (1.0f - V) * Vector<2, float>(A) + V * Vector<2, float>(B); }
		static Vector<2, float> SLerp(const Vector<2, T>& A, const Vector<2, T>& B, float V) { float Theta = Math::Acos(Dot(A, B)); float SinTheta = Math::Sin(Theta); return Normalize(Math::Sin((1.0f - V) * Theta) / SinTheta * Vector<2, float>(A) + Math::Sin(V * Theta) / SinTheta * Vector<2, float>(B)); }
		static float Dot(const Vector<2, T>& A, const Vector<2, T>& B) { return A.x * B.x + A.y * B.y + A.z * B.z; }
		static float Angle(const Vector<2, T>& A, const Vector<2, T>& B) { return  Math::Acos((Dot(A, B) / (Magnitude(A) * Magnitude(B)))); }
		static Vector<2, float> Cross(const Vector<2, T>& A, const Vector<2, T>& B) { return Vector<2, float>(A.x * B.y - B.x * A.y); }
		static Vector<2, float> ProjectOnVector(const Vector<2, T>& A, const Vector<2, T>& B) { return (Dot(A, B) / Dot(B, B)) * B; }
		static Vector<2, float> ProjectOnNormal(const Vector<2, T>& V, const Vector<2, T>& N) { return V - Dot(V, N) * N; }
		static Vector<2, float> Reflect(const Vector<2, T>& V, const Vector<2, T>& N) { return V - 2.0f * Dot(V, N) * N; }
		static Vector<2, float> Normalize(const Vector<2, T>& V) { float Size = SqrMagnitude(V); if (Math::Equals(Size, 0.0f)) return Vector<2, float>(); return 1.0f / Math::Sqrt(Size) * Vector<2, float>(V); }
		static float Distance(const Vector<2, T>& A, const Vector<2, T>& B) { return Magnitude(B - A); }
		static float Magnitude(const Vector<2, T>& V) { return Math::Sqrt(Dot(V, V)); }
		static float SqrMagnitude(const Vector<2, T>& V) { return Dot(V, V); }


		Vector() : x(0), y(0) {}
		template<typename P> explicit Vector(P V) : x(V), y(V) {}
		template<typename P> explicit Vector(P X, P Y) : x(X), y(Y) {}
		template<typename P> Vector(const Vector<2, P>& Other) : x(Other.x), y(Other.y) {}
		template<typename P> Vector(const Vector<3, P>& Other) : x(Other.x), y(Other.y) {}
		template<typename P> Vector(const Vector<4, P>& Other) : x(Other.x), y(Other.y) {}
		~Vector() {}

		Vector<2, T>& operator=(const Vector<2, T>& Other) { x = Other.x; y = Other.y; return *this; }
		bool operator==(const Vector<2, T>& Other) const { return x == Other.x && y == Other.y; }
		bool operator!=(const Vector<2, T>& Other) const { return !(*this == Other); }
		T& operator[](uint64 Index) { return Index == 0 ? x : Index == 1 ? y : x; }

		Vector<2, T>& operator+=(T Other) { x += Other; y += Other; return *this; }
		Vector<2, T>& operator-=(T Other) { x -= Other; y -= Other; return *this; }
		Vector<2, T>& operator*=(T Other) { x *= Other; y *= Other; return *this; }
		Vector<2, T>& operator/=(T Other) { x /= Other; y /= Other; return *this; }
		Vector<2, T>& operator+=(const Vector<2, T>& Other) { x += Other.x; y += Other.y; return *this; }
		Vector<2, T>& operator-=(const Vector<2, T>& Other) { x -= Other.x; y -= Other.y; return *this; }
		Vector<2, T>& operator*=(const Vector<2, T>& Other) { x *= Other.x; y *= Other.y; return *this; }
		Vector<2, T>& operator/=(const Vector<2, T>& Other) { x /= Other.x; y /= Other.y; return *this; }

		String ToString() const { return StringUtility::Format("(%.2f, %.2f)", (float)x, (float)y); }

	public:
		T x, y;
	};

	template<typename T> const Vector<2, T> Vector<2, T>::Zero = Vector<2, T>(0);
	template<typename T> const Vector<2, T> Vector<2, T>::One = Vector<2, T>(1);
	template<typename T> const Vector<2, T> Vector<2, T>::Infinity = Vector<2, T>(Decimal::InfinityF(), Decimal::InfinityF());
	template<typename T> const Vector<2, T> Vector<2, T>::Right = Vector<2, T>(1, 0);
	template<typename T> const Vector<2, T> Vector<2, T>::Up = Vector<2, T>(0, 1);

	template<typename T> Vector<2, T> operator+(T A, const Vector<2, T>& B) { return Vector<2, T>(A + B.x, A + B.y); }
	template<typename T> Vector<2, T> operator-(T A, const Vector<2, T>& B) { return Vector<2, T>(A - B.x, A - B.y); }
	template<typename T> Vector<2, T> operator*(T A, const Vector<2, T>& B) { return Vector<2, T>(A * B.x, A * B.y); }
	template<typename T> Vector<2, T> operator/(T A, const Vector<2, T>& B) { return Vector<2, T>(A / B.x, A / B.y); }
	template<typename T> Vector<2, T> operator+(const Vector<2, T>& A, T B) { return Vector<2, T>(A.x + B, A.y + B); }
	template<typename T> Vector<2, T> operator-(const Vector<2, T>& A, T B) { return Vector<2, T>(A.x - B, A.y - B); }
	template<typename T> Vector<2, T> operator*(const Vector<2, T>& A, T B) { return Vector<2, T>(A.x * B, A.y * B); }
	template<typename T> Vector<2, T> operator/(const Vector<2, T>& A, T B) { return Vector<2, T>(A.x / B, A.y / B); }
	template<typename T> Vector<2, T> operator+(const Vector<2, T>& A, const Vector<2, T>& B) { return Vector<2, T>(A.x + B.x, A.y + B.y); }
	template<typename T> Vector<2, T> operator-(const Vector<2, T>& A, const Vector<2, T>& B) { return Vector<2, T>(A.x - B.x, A.y - B.y); }
	template<typename T> Vector<2, T> operator*(const Vector<2, T>& A, const Vector<2, T>& B) { return Vector<2, T>(A.x * B.x, A.y * B.y); }
	template<typename T> Vector<2, T> operator/(const Vector<2, T>& A, const Vector<2, T>& B) { return Vector<2, T>(A.x / B.x, A.y / B.y); }

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

		static bool Equals(const Vector<3, T>& A, const Vector<3, T>& B) { return Math::Equals((float)A.x, (float)B.x) && Math::Equals((float)A.y, (float)B.y) && Math::Equals((float)A.z, (float)B.z); }
		static Vector<3, T> Clamp(const Vector<3, T>& V, const Vector<3, T>& A, const Vector<3, T>& B) { return Min(Max(V, A), B); }
		static Vector<3, T> Min(const Vector<3, T>& A, const Vector<3, T>& B) { return Vector<3, T>(Math::Min(A.x, B.x), Math::Min(A.y, B.y), Math::Min(A.z, B.z)); }
		static Vector<3, T> Max(const Vector<3, T>& A, const Vector<3, T>& B) { return Vector<3, T>(Math::Max(A.x, B.x), Math::Max(A.y, B.y), Math::Max(A.z, B.z)); }
		static Vector<3, T> Lerp(const Vector<3, T>& A, const Vector<3, T>& B, float V) { return (1.0f - V) * Vector<3, float>(A) + V * Vector<3, float>(B); }
		static Vector<3, float> SLerp(const Vector<3, T>& A, const Vector<3, T>& B, float V) { float Theta = Math::Acos(Dot(A, B)); float SinTheta = Math::Sin(Theta); return Normalize(Math::Sin((1.0f - V) * Theta) / SinTheta * Vector<3, float>(A) + Math::Sin(V * Theta) / SinTheta * Vector<3, float>(B)); }
		static float Dot(const Vector<3, T>& A, const Vector<3, T>& B) { return A.x * B.x + A.y * B.y + A.z * B.z; }
		static float Angle(const Vector<3, T>& A, const Vector<3, T>& B) { return  Math::Acos((Dot(A, B) / (Magnitude(A) * Magnitude(B)))); }
		static Vector<3, float> Cross(const Vector<3, T>& A, const Vector<3, T>& B) { return Vector<3, float>(A.y * B.z - B.y * A.z, A.z * B.x - B.z * A.x, A.x * B.y - B.x * A.y); }
		static Vector<3, float> ProjectOnVector(const Vector<3, T>& A, const Vector<3, T>& B) { return (Dot(A, B) / Dot(B, B)) * B; }
		static Vector<3, float> ProjectOnNormal(const Vector<3, T>& V, const Vector<3, T>& N) { return V - Dot(V, N) * N; }
		static Vector<3, float> Reflect(const Vector<3, T>& V, const Vector<3, T>& N) { return V - 2.0f * Dot(V, N) * N; }
		static Vector<3, float> Normalize(const Vector<3, T>& V) { float Size = SqrMagnitude(V); if (Math::Equals(Size, 0.0f)) return Vector<3, float>(); return 1.0f / Math::Sqrt(Size) * Vector<3, float>(V); }
		static float Distance(const Vector<3, T>& A, const Vector<3, T>& B) { return Magnitude(B - A); }
		static float Magnitude(const Vector<3, T>& V) { return Math::Sqrt(Dot(V, V)); }
		static float SqrMagnitude(const Vector<3, T>& V) { return Dot(V, V); }

		Vector() : x(0), y(0), z(0) {}
		template<typename P> explicit Vector(P V) : x(V), y(V), z(V) {}
		template<typename P> explicit Vector(P X, P Y) : x(X), y(Y), z(0) {}
		template<typename P> explicit Vector(P X, P Y, P Z) : x(X), y(Y), z(Z) {}
		template<typename P> explicit Vector(const Vector<2, P>& Other) : x(Other.x), y(Other.y), z(0) {}
		template<typename P> Vector(const Vector<3, P>& Other) : x(Other.x), y(Other.y), z(Other.z) {}
		template<typename P> Vector(const Vector<4, P>& Other) : x(Other.x), y(Other.y), z(Other.z) {}
		~Vector() {}

		Vector<3, T>& operator=(const Vector<3, T>& Other) { x = Other.x; y = Other.y; z = Other.z; return *this; }
		bool operator==(const Vector<3, T>& Other) const { return x == Other.x && y == Other.y && z == Other.z; }
		bool operator!=(const Vector<3, T>& Other) const { return !(*this == Other); }
		T& operator[](uint64 Index) { return Index == 0 ? x : Index == 1 ? y : Index == 2 ? z : x; }

		Vector<3, T>& operator+=(T Other) { x += Other; y += Other; z += Other; return *this; }
		Vector<3, T>& operator-=(T Other) { x -= Other; y -= Other; z -= Other; return *this; }
		Vector<3, T>& operator*=(T Other) { x *= Other; y *= Other; z *= Other; return *this; }
		Vector<3, T>& operator/=(T Other) { x /= Other; y /= Other; z /= Other; return *this; }
		Vector<3, T>& operator+=(const Vector<3, T>& Other) { x += Other.x; y += Other.y; z += Other.z; return *this; }
		Vector<3, T>& operator-=(const Vector<3, T>& Other) { x -= Other.x; y -= Other.y; z -= Other.z; return *this; }
		Vector<3, T>& operator*=(const Vector<3, T>& Other) { x *= Other.x; y *= Other.y; z *= Other.z; return *this; }
		Vector<3, T>& operator/=(const Vector<3, T>& Other) { x /= Other.x; y /= Other.y; z /= Other.z; return *this; }

		String ToString() const { return StringUtility::Format("(%.2f, %.2f, %.2f)", (float)x, (float)y, (float)z); }

	public:
		T x, y, z;
	};

	template<typename T> const Vector<3, T> Vector<3, T>::Zero = Vector<3, T>(0);
	template<typename T> const Vector<3, T> Vector<3, T>::One = Vector<3, T>(1);
	template<typename T> const Vector<3, T> Vector<3, T>::Infinity = Vector<3, T>(Decimal::InfinityF(), Decimal::InfinityF(), Decimal::InfinityF());
	template<typename T> const Vector<3, T> Vector<3, T>::Right = Vector<3, T>(1, 0, 0);
	template<typename T> const Vector<3, T> Vector<3, T>::Up = Vector<3, T>(0, 1, 0);
	template<typename T> const Vector<3, T> Vector<3, T>::Forward = Vector<3, T>(0, 0, 1);

	template<typename T> Vector<3, T> operator+(T A, const Vector<3, T>& B) { return Vector<3, T>(A + B.x, A + B.y, A + B.z); }
	template<typename T> Vector<3, T> operator-(T A, const Vector<3, T>& B) { return Vector<3, T>(A - B.x, A - B.y, A - B.z); }
	template<typename T> Vector<3, T> operator*(T A, const Vector<3, T>& B) { return Vector<3, T>(A * B.x, A * B.y, A * B.z); }
	template<typename T> Vector<3, T> operator/(T A, const Vector<3, T>& B) { return Vector<3, T>(A / B.x, A / B.y, A / B.z); }
	template<typename T> Vector<3, T> operator+(const Vector<3, T>& A, T B) { return Vector<3, T>(A.x + B, A.y + B, A.z + B); }
	template<typename T> Vector<3, T> operator-(const Vector<3, T>& A, T B) { return Vector<3, T>(A.x - B, A.y - B, A.z - B); }
	template<typename T> Vector<3, T> operator*(const Vector<3, T>& A, T B) { return Vector<3, T>(A.x * B, A.y * B, A.z * B); }
	template<typename T> Vector<3, T> operator/(const Vector<3, T>& A, T B) { return Vector<3, T>(A.x / B, A.y / B, A.z / B); }
	template<typename T> Vector<3, T> operator+(const Vector<3, T>& A, const Vector<3, T>& B) { return Vector<3, T>(A.x + B.x, A.y + B.y, A.z + B.z); }
	template<typename T> Vector<3, T> operator-(const Vector<3, T>& A, const Vector<3, T>& B) { return Vector<3, T>(A.x - B.x, A.y - B.y, A.z - B.z); }
	template<typename T> Vector<3, T> operator*(const Vector<3, T>& A, const Vector<3, T>& B) { return Vector<3, T>(A.x * B.x, A.y * B.y, A.z * B.z); }
	template<typename T> Vector<3, T> operator/(const Vector<3, T>& A, const Vector<3, T>& B) { return Vector<3, T>(A.x / B.x, A.y / B.y, A.z / B.z); }

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

		static bool Equals(const Vector<4, T>& A, const Vector<4, T>& B) { return Math::Equals((float)A.x, (float)B.x) && Math::Equals((float)A.y, (float)B.y) && Math::Equals((float)A.z, (float)B.z); }
		static Vector<4, T> Clamp(const Vector<4, T>& V, const Vector<4, T>& A, const Vector<4, T>& B) { return Min(Max(V, A), B); }
		static Vector<4, T> Min(const Vector<4, T>& A, const Vector<4, T>& B) { return Vector<4, T>(Math::Min(A.x, B.x), Math::Min(A.y, B.y), Math::Min(A.z, B.z)); }
		static Vector<4, T> Max(const Vector<4, T>& A, const Vector<4, T>& B) { return Vector<4, T>(Math::Max(A.x, B.x), Math::Max(A.y, B.y), Math::Max(A.z, B.z)); }
		static Vector<4, T> Lerp(const Vector<4, T>& A, const Vector<4, T>& B, float V) { return (1.0f - V) * Vector<4, float>(A) + V * Vector<4, float>(B); }
		static Vector<4, float> SLerp(const Vector<4, T>& A, const Vector<4, T>& B, float V) { float Theta = Math::Acos(Dot(A, B)); float SinTheta = Math::Sin(Theta); return Normalize(Math::Sin((1.0f - V) * Theta) / SinTheta * Vector<4, float>(A) + Math::Sin(V * Theta) / SinTheta * Vector<4, float>(B)); }
		static float Dot(const Vector<4, T>& A, const Vector<4, T>& B) { return A.x * B.x + A.y * B.y + A.z * B.z; }
		static float Angle(const Vector<4, T>& A, const Vector<4, T>& B) { return  Math::Acos((Dot(A, B) / (Magnitude(A) * Magnitude(B)))); }
		static Vector<4, T> Cross(const Vector<4, T>& A, const Vector<4, T>& B) { return Vector<4, float>(A.x * B.y - B.x * A.y); }
		static Vector<4, T> ProjectOnVector(const Vector<4, T>& A, const Vector<4, T>& B) { return (Dot(A, B) / Dot(B, B)) * B; }
		static Vector<4, T> ProjectOnNormal(const Vector<4, T>& V, const Vector<4, T>& N) { return V - Dot(V, N) * N; }
		static Vector<4, T> Reflect(const Vector<4, T>& V, const Vector<4, T>& N) { return V - 2.0f * Dot(V, N) * N; }
		static Vector<4, T> Normalize(const Vector<4, T>& V) { float Size = SqrMagnitude(V); if (Math::Equals(Size, 0.0f)) return Vector<4, float>(); return 1.0f / Math::Sqrt(Size) * Vector<4, float>(V); }
		static float Distance(const Vector<4, T>& A, const Vector<4, T>& B) { return Magnitude(B - A); }
		static float Magnitude(const Vector<4, T>& V) { return Math::Sqrt(Dot(V, V)); }
		static float SqrMagnitude(const Vector<4, T>& V) { return Dot(V, V); }

		Vector() : x(0), y(0), z(0), w(0) {}
		template<typename P> explicit Vector(P V) : x(V), y(V), z(V), w(V) {}
		template<typename P> explicit Vector(P X, P Y) : x(X), y(Y), z(0), w(0) {}
		template<typename P> explicit Vector(P X, P Y, P Z) : x(X), y(Y), z(Z), w(0) {}
		template<typename P> explicit Vector(P X, P Y, P Z, P W) : x(X), y(Y), z(Z), w(W) {}
		template<typename P> Vector(const Vector<2, P>& Other) : x(Other.x), y(Other.y), z(0), w(0) {}
		template<typename P> Vector(const Vector<3, P>& Other) : x(Other.x), y(Other.y), z(Other.z), w(0) {}
		template<typename P> Vector(const Vector<4, P>& Other) : x(Other.x), y(Other.y), z(Other.z), w(Other.w) {}
		~Vector() {}

		Vector<4, T>& operator=(const Vector<4, T>& Other) { x = Other.x; y = Other.y; z = Other.z; w = Other.w; return *this; }
		bool operator==(const Vector<4, T>& Other) const { return x == Other.x && y == Other.y && z == Other.z && w == Other.w; }
		bool operator!=(const Vector<4, T>& Other) const { return !(*this == Other); }
		T& operator[](uint64 Index) { return Index == 0 ? x : Index == 1 ? y : Index == 2 ? z : Index == 3 ? w : x; }

		Vector<4, T>& operator+=(T Other) { x += Other; y += Other; z += Other; w += Other; return *this; }
		Vector<4, T>& operator-=(T Other) { x -= Other; y -= Other; z -= Other; w -= Other; return *this; }
		Vector<4, T>& operator*=(T Other) { x *= Other; y *= Other; z *= Other; w *= Other; return *this; }
		Vector<4, T>& operator/=(T Other) { x /= Other; y /= Other; z /= Other; w /= Other; return *this; }
		Vector<4, T>& operator+=(const Vector<4, T>& Other) { x += Other.x; y += Other.y; z += Other.z; w += Other.w; return *this; }
		Vector<4, T>& operator-=(const Vector<4, T>& Other) { x -= Other.x; y -= Other.y; z -= Other.z; w -= Other.w; return *this; }
		Vector<4, T>& operator*=(const Vector<4, T>& Other) { x *= Other.x; y *= Other.y; z *= Other.z; w *= Other.w; return *this; }
		Vector<4, T>& operator/=(const Vector<4, T>& Other) { x /= Other.x; y /= Other.y; z /= Other.z; w /= Other.w; return *this; }

		String ToString() const { return StringUtility::Format("(%.2f, %.2f, %.2f, %.2f)", (float)x, (float)y, (float)z, (float)w); }

	public:
		T x, y, z, w;
	};

	template<typename T> const Vector<4, T> Vector<4, T>::Zero = Vector<4, T>(0);
	template<typename T> const Vector<4, T> Vector<4, T>::One = Vector<4, T>(1);
	template<typename T> const Vector<4, T> Vector<4, T>::Infinity = Vector<4, T>(Decimal::InfinityF(), Decimal::InfinityF(), Decimal::InfinityF(), Decimal::InfinityF());
	template<typename T> const Vector<4, T> Vector<4, T>::Right = Vector<4, T>(1, 0, 0);
	template<typename T> const Vector<4, T> Vector<4, T>::Up = Vector<4, T>(0, 1, 0);
	template<typename T> const Vector<4, T> Vector<4, T>::Forward = Vector<4, T>(0, 0, 1);

	template<typename T> Vector<4, T> operator+(T A, const Vector<4, T>& B) { return Vector<4, T>(A + B.x, A + B.y, A + B.z, A + B.w); }
	template<typename T> Vector<4, T> operator-(T A, const Vector<4, T>& B) { return Vector<4, T>(A - B.x, A - B.y, A - B.z, A - B.w); }
	template<typename T> Vector<4, T> operator*(T A, const Vector<4, T>& B) { return Vector<4, T>(A * B.x, A * B.y, A * B.z, A * B.w); }
	template<typename T> Vector<4, T> operator/(T A, const Vector<4, T>& B) { return Vector<4, T>(A / B.x, A / B.y, A / B.z, A / B.w); }
	template<typename T> Vector<4, T> operator+(const Vector<4, T>& A, T B) { return Vector<4, T>(A.x + B, A.y + B, A.z + B, A.w + B); }
	template<typename T> Vector<4, T> operator-(const Vector<4, T>& A, T B) { return Vector<4, T>(A.x - B, A.y - B, A.z - B, A.w - B); }
	template<typename T> Vector<4, T> operator*(const Vector<4, T>& A, T B) { return Vector<4, T>(A.x * B, A.y * B, A.z * B, A.w * B); }
	template<typename T> Vector<4, T> operator/(const Vector<4, T>& A, T B) { return Vector<4, T>(A.x / B, A.y / B, A.z / B, A.w / B); }
	template<typename T> Vector<4, T> operator+(const Vector<4, T>& A, const Vector<4, T>& B) { return Vector<4, T>(A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w); }
	template<typename T> Vector<4, T> operator-(const Vector<4, T>& A, const Vector<4, T>& B) { return Vector<4, T>(A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w); }
	template<typename T> Vector<4, T> operator*(const Vector<4, T>& A, const Vector<4, T>& B) { return Vector<4, T>(A.x * B.x, A.y * B.y, A.z * B.z, A.w * B.w); }
	template<typename T> Vector<4, T> operator/(const Vector<4, T>& A, const Vector<4, T>& B) { return Vector<4, T>(A.x / B.x, A.y / B.y, A.z / B.z, A.w / B.w); }

#pragma endregion

#pragma warning(pop)
}
