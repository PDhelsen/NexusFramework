#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Numbers/Decimal.h"
#include "NexusFramework/Math/Math.h"
#include "NexusFramework/Types/Strings/String.h"

namespace NxFr
{
#pragma region Vector

	template<uint8 D, typename T>
	struct Vector
	{
	public:
		static T Angle(const Vector<D, T>& A, const Vector<D, T>& B) { return Math::Acos(Dot(A, B) / (Magnitude(A) * Magnitude(B))); }
		static Vector<D, T> Clamp(const Vector<D, T>& V, const Vector<D, T>& A, const Vector<D, T>& B) { return Min(Max(V, A), B); }
		static T Distance(const Vector<D, T>& A, const Vector<D, T>& B) { return Magnitude(B - A); }
		static Vector<D, T> Lerp(const Vector<D, T>& A, const Vector<D, T>& B, T V) { return (T(1) - V) * A + V * B; }
		static T Magnitude(const Vector<D, T>& V) { return (T)Math::Sqrt((float)Dot(V, V)); }
		static T SqrMagnitude(const Vector<D, T>& V) { return Dot(V, V); }
		static Vector<D, T> Normalize(const Vector<D, T>& V) { T Size = SqrMagnitude(V); if (Math::Equals(Size, T(0))) return Vector<D, T>(); return T(1.0f / Math::Sqrt((float)Size)) * V; }
		static Vector<D, T> ProjectOnVector(const Vector<D, T>& A, const Vector<D, T>& B) { return B * (Dot(A, B) / Dot(B, B)); }
		static Vector<D, T> ProjectOnNormal(const Vector<D, T>& V, const Vector<D, T>& N) { return V - N * Dot(V, N); }
		static Vector<D, T> Reflect(const Vector<D, T>& V, const Vector<D, T>& N) { return V - T(2) * Dot(V, N) * N; }
		static Vector<D, T> SLerp(const Vector<D, T>& A, const Vector<D, T>& B, T V) { T Theta = Math::Acos(Dot(A, B)); T SinTheta = Math::Sin(Theta); return Normalize(A * (Math::Sin((T(1) - V) * Theta) / SinTheta) + B * (Math::Sin(V * Theta) / SinTheta)); }

		static Vector<D, T> Cross(const Vector<D, T>& A, const Vector<D, T>& B) { return Vector<D, T>(); }
		static T Dot(const Vector<D, T>& A, const Vector<D, T>& B) { return 0; }
		static bool Equals(const Vector<D, T>& A, const Vector<D, T>& B) { return false; }
		static Vector<D, T> Min(const Vector<D, T>& A, const Vector<D, T>& B) { return Vector<D, T>(); }
		static Vector<D, T> Max(const Vector<D, T>& A, const Vector<D, T>& B) { return Vector<D, T>(); }

		Vector() {}
		Vector(const Vector<D, T>& Other) {}
		Vector(Vector<D, T>&& Other) noexcept {}
		~Vector() {}

		Vector<D, T>& operator=(const Vector<D, T>& Other) { return *this; }
		Vector<D, T>& operator=(Vector<D, T>&& Other) noexcept { return *this; }

		bool operator==(const Vector<D, T>& Other) const { return false; }
		bool operator!=(const Vector<D, T>& Other) const { return !(*this == Other); }

		T& operator[](uint64 Index) { return 0; }

		Vector<D, T>& operator+=(T Other) { return *this; }
		Vector<D, T>& operator-=(T Other) { return *this; }
		Vector<D, T>& operator*=(T Other) { return *this; }
		Vector<D, T>& operator/=(T Other) { return *this; }
		Vector<D, T>& operator+=(const Vector<D, T>& Other) { return *this; }
		Vector<D, T>& operator-=(const Vector<D, T>& Other) { return *this; }
		Vector<D, T>& operator*=(const Vector<D, T>& Other) { return *this; }
		Vector<D, T>& operator/=(const Vector<D, T>& Other) { return *this; }

		Vector<D, T> Normalized() const { return Normalize(*this); }
		T Magnitude() const { return Magnitude(*this); }
		T SqrMagnitude() const { return SqrMagnitude(*this); }

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

		static T Angle(const Vector<2, T>& A, const Vector<2, T>& B) { return Math::Acos(Dot(A, B) / (Magnitude(A) * Magnitude(B))); }
		static Vector<2, T> Clamp(const Vector<2, T>& V, const Vector<2, T>& A, const Vector<2, T>& B) { return Min(Max(V, A), B); }
		static T Distance(const Vector<2, T>& A, const Vector<2, T>& B) { return Magnitude(B - A); }
		static Vector<2, T> Lerp(const Vector<2, T>& A, const Vector<2, T>& B, T V) { return (T(1) - V) * A + V * B; }
		static T Magnitude(const Vector<2, T>& V) { return (T)Math::Sqrt((float)Dot(V, V)); }
		static T SqrMagnitude(const Vector<2, T>& V) { return Dot(V, V); }
		static Vector<2, T> Normalize(const Vector<2, T>& V) { T Size = SqrMagnitude(V); if (Math::Equals(Size, T(0))) return Vector<2, T>(); return T(1.0f / Math::Sqrt((float)Size)) * V; }
		static Vector<2, T> ProjectOnVector(const Vector<2, T>& A, const Vector<2, T>& B) { return B * (Dot(A, B) / Dot(B, B)); }
		static Vector<2, T> ProjectOnNormal(const Vector<2, T>& V, const Vector<2, T>& N) { return V - N * Dot(V, N); }
		static Vector<2, T> Reflect(const Vector<2, T>& V, const Vector<2, T>& N) { return V - T(2) * Dot(V, N) * N; }
		static Vector<2, T> SLerp(const Vector<2, T>& A, const Vector<2, T>& B, T V) { T Theta = Math::Acos(Dot(A, B)); T SinTheta = Math::Sin(Theta); return Normalize(A * (Math::Sin((T(1) - V) * Theta) / SinTheta) + B * (Math::Sin(V * Theta) / SinTheta)); }

		static Vector<2, T> Cross(const Vector<2, T>& A, const Vector<2, T>& B) { return Vector<2, T>(A.x * B.y - A.y * B.x); }
		static T Dot(const Vector<2, T>& A, const Vector<2, T>& B) { return A.x * B.x + A.y * B.y; }
		static bool Equals(const Vector<2, T>& A, const Vector<2, T>& B) { return Math::Equals(A.x, B.x) && Math::Equals(A.y, B.y); }
		static Vector<2, T> Min(const Vector<2, T>& A, const Vector<2, T>& B) { return Vector<2, T>(Math::Min(A.x, B.x), Math::Min(A.y, B.y)); }
		static Vector<2, T> Max(const Vector<2, T>& A, const Vector<2, T>& B) { return Vector<2, T>(Math::Max(A.x, B.x), Math::Max(A.y, B.y)); }


		Vector() : x(0), y(0) {}
		Vector(T V) : x(V), y(V) {}
		Vector(T X, T Y) : x(X), y(Y) {}
		Vector(const Vector<2, T>& Other) : x(Other.x), y(Other.y) {}
		Vector(const Vector<3, T>& Other) : x(Other.x), y(Other.y) {}
		Vector(const Vector<4, T>& Other) : x(Other.x), y(Other.y) {}
		Vector(Vector<2, T>&& Other) noexcept : x(Other.x), y(Other.y) {}
		~Vector() {}

		Vector<2, T>& operator=(const Vector<2, T>& Other) { x = Other.x; y = Other.y; return *this; }
		Vector<2, T>& operator=(Vector<2, T>&& Other) noexcept { x = Other.x; y = Other.y; return *this; }

		bool operator==(const Vector<2, T>& Other) const { return x == Other.x && y == Other.y; }

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

	template<typename T> const Vector<2, T> Vector<2, T>::Zero = Vector<2, T>(0);
	template<typename T> const Vector<2, T> Vector<2, T>::One = Vector<2, T>(1);
	template<typename T> const Vector<2, T> Vector<2, T>::Infinity = Vector<2, T>(Decimal::InfinityF(), Decimal::InfinityF());
	template<typename T> const Vector<2, T> Vector<2, T>::Right = Vector<2, T>(1, 0);
	template<typename T> const Vector<2, T> Vector<2, T>::Up = Vector<2, T>(0, 1);

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

		static T Angle(const Vector<3, T>& A, const Vector<3, T>& B) { return Math::Acos(Dot(A, B) / (Magnitude(A) * Magnitude(B))); }
		static Vector<3, T> Clamp(const Vector<3, T>& V, const Vector<3, T>& A, const Vector<3, T>& B) { return Min(Max(V, A), B); }
		static T Distance(const Vector<3, T>& A, const Vector<3, T>& B) { return Magnitude(B - A); }
		static Vector<3, T> Lerp(const Vector<3, T>& A, const Vector<3, T>& B, T V) { return (T(1) - V) * A + V * B; }
		static T Magnitude(const Vector<3, T>& V) { return (T)Math::Sqrt((float)Dot(V, V)); }
		static T SqrMagnitude(const Vector<3, T>& V) { return Dot(V, V); }
		static Vector<3, T> Normalize(const Vector<3, T>& V) { T Size = SqrMagnitude(V); if (Math::Equals(Size, T(0))) return Vector<3, T>(); return T(1.0f / Math::Sqrt((float)Size)) * V; }
		static Vector<3, T> ProjectOnVector(const Vector<3, T>& A, const Vector<3, T>& B) { return B * (Dot(A, B) / Dot(B, B)); }
		static Vector<3, T> ProjectOnNormal(const Vector<3, T>& V, const Vector<3, T>& N) { return V - N * Dot(V, N); }
		static Vector<3, T> Reflect(const Vector<3, T>& V, const Vector<3, T>& N) { return V - T(2) * Dot(V, N) * N; }
		static Vector<3, T> SLerp(const Vector<3, T>& A, const Vector<3, T>& B, T V) { T Theta = Math::Acos(Dot(A, B)); T SinTheta = Math::Sin(Theta); return Normalize(A * (Math::Sin((T(1) - V) * Theta) / SinTheta) + B * (Math::Sin(V * Theta) / SinTheta)); }

		static Vector<3, T> Cross(const Vector<3, T>& A, const Vector<3, T>& B) { return Vector<3, T>(A.y * B.z - B.y * A.z, A.z * B.x - B.z * A.x, A.x * B.y - B.x * A.y); }
		static T Dot(const Vector<3, T>& A, const Vector<3, T>& B) { return A.x * B.x + A.y * B.y + A.z * B.z; }
		static bool Equals(const Vector<3, T>& A, const Vector<3, T>& B) { return Math::Equals(A.x, B.x) && Math::Equals(A.y, B.y) && Math::Equals(A.z, B.z); }
		static Vector<3, T> Min(const Vector<3, T>& A, const Vector<3, T>& B) { return Vector<3, T>(Math::Min(A.x, B.x), Math::Min(A.y, B.y), Math::Min(A.z, B.z)); }
		static Vector<3, T> Max(const Vector<3, T>& A, const Vector<3, T>& B) { return Vector<3, T>(Math::Max(A.x, B.x), Math::Max(A.y, B.y), Math::Max(A.z, B.z)); }

		Vector() : x(0), y(0), z(0) {}
		Vector(T V) : x(V), y(V), z(V) {}
		Vector(T X, T Y) : x(X), y(Y), z(0) {}
		Vector(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
		explicit Vector(const Vector<2, T>& Other) : x(Other.x), y(Other.y), z(0) {}
		Vector(const Vector<3, T>& Other) : x(Other.x), y(Other.y), z(Other.z) {}
		Vector(const Vector<4, T>& Other) : x(Other.x), y(Other.y), z(Other.z) {}
		Vector(Vector<3, T>&& Other) noexcept : x(Other.x), y(Other.y), z(Other.z) {}
		~Vector() {}

		Vector<3, T>& operator=(const Vector<3, T>& Other) { x = Other.x; y = Other.y; z = Other.z; return *this; }
		Vector<3, T>& operator=(Vector<3, T>&& Other) noexcept { x = Other.x; y = Other.y; z = Other.z; return *this; }

		bool operator==(const Vector<3, T>& Other) const { return x == Other.x && y == Other.y && z == Other.z; }

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

	template<typename T> const Vector<3, T> Vector<3, T>::Zero = Vector<3, T>(0);
	template<typename T> const Vector<3, T> Vector<3, T>::One = Vector<3, T>(1);
	template<typename T> const Vector<3, T> Vector<3, T>::Infinity = Vector<3, T>(Decimal::InfinityF(), Decimal::InfinityF(), Decimal::InfinityF());
	template<typename T> const Vector<3, T> Vector<3, T>::Right = Vector<3, T>(1, 0, 0);
	template<typename T> const Vector<3, T> Vector<3, T>::Up = Vector<3, T>(0, 1, 0);
	template<typename T> const Vector<3, T> Vector<3, T>::Forward = Vector<3, T>(0, 0, 1);

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

		static T Angle(const Vector<4, T>& A, const Vector<4, T>& B) { return Math::Acos(Dot(A, B) / (Magnitude(A) * Magnitude(B))); }
		static Vector<4, T> Clamp(const Vector<4, T>& V, const Vector<4, T>& A, const Vector<4, T>& B) { return Min(Max(V, A), B); }
		static T Distance(const Vector<4, T>& A, const Vector<4, T>& B) { return Magnitude(B - A); }
		static Vector<4, T> Lerp(const Vector<4, T>& A, const Vector<4, T>& B, T V) { return (T(1) - V) * A + V * B; }
		static T Magnitude(const Vector<4, T>& V) { return (T)Math::Sqrt((float)Dot(V, V)); }
		static T SqrMagnitude(const Vector<4, T>& V) { return Dot(V, V); }
		static Vector<4, T> Normalize(const Vector<4, T>& V) { T Size = SqrMagnitude(V); if (Math::Equals(Size, T(0))) return Vector<4, T>(); return T(1.0f / Math::Sqrt((float)Size)) * V; }
		static Vector<4, T> ProjectOnVector(const Vector<4, T>& A, const Vector<4, T>& B) { return B * (Dot(A, B) / Dot(B, B)); }
		static Vector<4, T> ProjectOnNormal(const Vector<4, T>& V, const Vector<4, T>& N) { return V - N * Dot(V, N); }
		static Vector<4, T> Reflect(const Vector<4, T>& V, const Vector<4, T>& N) { return V - T(2) * Dot(V, N) * N; }
		static Vector<4, T> SLerp(const Vector<4, T>& A, const Vector<4, T>& B, T V) { T Theta = Math::Acos(Dot(A, B)); T SinTheta = Math::Sin(Theta); return Normalize(A * (Math::Sin((T(1) - V) * Theta) / SinTheta) + B * (Math::Sin(V * Theta) / SinTheta)); }

		static Vector<4, T> Cross(const Vector<4, T>& A, const Vector<4, T>& B) { return Vector<4, T>(); }
		static T Dot(const Vector<4, T>& A, const Vector<4, T>& B) { return A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w; }
		static bool Equals(const Vector<4, T>& A, const Vector<4, T>& B) { return Math::Equals(A.x, B.x) && Math::Equals(A.y, B.y) && Math::Equals(A.z, B.z) && Math::Equals(A.w, B.w); }
		static Vector<4, T> Min(const Vector<4, T>& A, const Vector<4, T>& B) { return Vector<4, T>(Math::Min(A.x, B.x), Math::Min(A.y, B.y), Math::Min(A.z, B.z), Math::Min(A.w, B.w)); }
		static Vector<4, T> Max(const Vector<4, T>& A, const Vector<4, T>& B) { return Vector<4, T>(Math::Max(A.x, B.x), Math::Max(A.y, B.y), Math::Max(A.z, B.z), Math::Max(A.w, B.w)); }

		Vector() : x(0), y(0), z(0), w(0) {}
		Vector(T V) : x(V), y(V), z(V), w(V) {}
		Vector(T X, T Y) : x(X), y(Y), z(0), w(0) {}
		Vector(T X, T Y, T Z) : x(X), y(Y), z(Z), w(0) {}
		Vector(T X, T Y, T Z, T W) : x(X), y(Y), z(Z), w(W) {}
		explicit Vector(const Vector<2, T>& Other) : x(Other.x), y(Other.y), z(0), w(0) {}
		explicit Vector(const Vector<3, T>& Other) : x(Other.x), y(Other.y), z(Other.z), w(0) {}
		Vector(const Vector<4, T>& Other) : x(Other.x), y(Other.y), z(Other.z), w(Other.w) {}
		Vector(Vector<4, T>&& Other) noexcept : x(Other.x), y(Other.y), z(Other.z), w(Other.w) {}
		~Vector() {}

		Vector<4, T>& operator=(const Vector<4, T>& Other) { x = Other.x; y = Other.y; z = Other.z; w = Other.w; return *this; }
		Vector<4, T>& operator=(Vector<4, T>&& Other) noexcept { x = Other.x; y = Other.y; z = Other.z;  w = Other.w; return *this; }

		bool operator==(const Vector<4, T>& Other) const { return x == Other.x && y == Other.y && z == Other.z && w == Other.w; }

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

	template<typename T> const Vector<4, T> Vector<4, T>::Zero = Vector<4, T>(0);
	template<typename T> const Vector<4, T> Vector<4, T>::One = Vector<4, T>(1);
	template<typename T> const Vector<4, T> Vector<4, T>::Infinity = Vector<4, T>(Decimal::InfinityF(), Decimal::InfinityF(), Decimal::InfinityF(), Decimal::InfinityF());
	template<typename T> const Vector<4, T> Vector<4, T>::Right = Vector<4, T>(1, 0, 0);
	template<typename T> const Vector<4, T> Vector<4, T>::Up = Vector<4, T>(0, 1, 0);
	template<typename T> const Vector<4, T> Vector<4, T>::Forward = Vector<4, T>(0, 0, 1);

#pragma endregion

#pragma region Using

	using Vec2f = Vector<2, float>;
	using Vec2i = Vector<2, int32>;
	using Vec3f = Vector<3, float>;
	using Vec3i = Vector<3, int32>;
	using Vec4f = Vector<4, float>;
	using Vec4i = Vector<4, int32>;

#pragma endregion
}
