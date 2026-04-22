#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Decimal.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Math/Vector.h"
#include "NexusFramework/Misc/Hash.h"
#include "NexusFramework/Serialization/Rbs.h"
#include "NexusFramework/Serialization/Yaml.h"

namespace NxFr
{
	namespace Colors
	{
		struct sRGB;
	}
	using Color = Colors::sRGB;

	namespace Colors
	{
		NEXUS_FRAMEWORK_API extern const Color Clear;
		NEXUS_FRAMEWORK_API extern const Color Black;
		NEXUS_FRAMEWORK_API extern const Color Gray;
		NEXUS_FRAMEWORK_API extern const Color White;
		NEXUS_FRAMEWORK_API extern const Color Red;
		NEXUS_FRAMEWORK_API extern const Color Green;
		NEXUS_FRAMEWORK_API extern const Color Blue;
		NEXUS_FRAMEWORK_API extern const Color Yellow;
		NEXUS_FRAMEWORK_API extern const Color Cyan;
		NEXUS_FRAMEWORK_API extern const Color Magenta;
		NEXUS_FRAMEWORK_API extern const Color Orange;

		struct sRGB;
		struct Linear;
		struct Bits;
		struct Hsv;

		struct sRGB
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			NEXUS_FRAMEWORK_API sRGB();
			NEXUS_FRAMEWORK_API sRGB(Type V);
			NEXUS_FRAMEWORK_API sRGB(Type R, Type G, Type B);
			NEXUS_FRAMEWORK_API sRGB(Type R, Type G, Type B, Type A);
			NEXUS_FRAMEWORK_API sRGB(Vector3f V);
			NEXUS_FRAMEWORK_API sRGB(Vector4f V);
			NEXUS_FRAMEWORK_API sRGB(const sRGB& Other);
			NEXUS_FRAMEWORK_API sRGB(const Linear& Other);
			NEXUS_FRAMEWORK_API sRGB(const Bits& Other);
			NEXUS_FRAMEWORK_API sRGB(const Hsv& Other);
			NEXUS_FRAMEWORK_API ~sRGB();

			NEXUS_FRAMEWORK_API sRGB& operator=(sRGB Other);
			NEXUS_FRAMEWORK_API bool operator==(sRGB Other) const;
			NEXUS_FRAMEWORK_API bool operator!=(sRGB Other) const;
			NEXUS_FRAMEWORK_API Type& operator[](uint64 Index);
			NEXUS_FRAMEWORK_API const Type& operator[](uint64 Index) const;

			NEXUS_FRAMEWORK_API sRGB& operator+=(Type Other) { r += Other;	 g += Other;	b += Other;		a += Other;		return *this; }
			NEXUS_FRAMEWORK_API sRGB& operator-=(Type Other) { r -= Other;	 g -= Other;	b -= Other;		a -= Other;		return *this; }
			NEXUS_FRAMEWORK_API sRGB& operator*=(Type Other) { r *= Other;	 g *= Other;	b *= Other;		a *= Other;		return *this; }
			NEXUS_FRAMEWORK_API sRGB& operator/=(Type Other) { r /= Other;	 g /= Other;	b /= Other;		a /= Other;		return *this; }
			NEXUS_FRAMEWORK_API sRGB& operator+=(sRGB Other) { r += Other.r; g += Other.g;	b += Other.b;	a += Other.a;	return *this; }
			NEXUS_FRAMEWORK_API sRGB& operator-=(sRGB Other) { r -= Other.r; g -= Other.g;	b -= Other.b;	a -= Other.a;	return *this; }
			NEXUS_FRAMEWORK_API sRGB& operator*=(sRGB Other) { r *= Other.r; g *= Other.g;	b *= Other.b;	a *= Other.a;	return *this; }
			NEXUS_FRAMEWORK_API sRGB& operator/=(sRGB Other) { r /= Other.r; g /= Other.g;	b /= Other.b;	a /= Other.a;	return *this; }

			NEXUS_FRAMEWORK_API Type Min() const;
			NEXUS_FRAMEWORK_API Type Max() const;
			NEXUS_FRAMEWORK_API sRGB Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			NEXUS_FRAMEWORK_API Linear ToLinear() const;
			NEXUS_FRAMEWORK_API Bits ToBits() const;
			NEXUS_FRAMEWORK_API Hsv ToHsv() const;
			NEXUS_FRAMEWORK_API Type ToGrayscale() const;
			NEXUS_FRAMEWORK_API Vector3f ToVector3f() const;
			NEXUS_FRAMEWORK_API Vector4f ToVector4f() const;

			NEXUS_FRAMEWORK_API Type GetX() const { return r; }
			NEXUS_FRAMEWORK_API Type GetY() const { return g; }
			NEXUS_FRAMEWORK_API Type GetZ() const { return b; }
			NEXUS_FRAMEWORK_API Type GetW() const { return a; }
			NEXUS_FRAMEWORK_API void SetX(Type x) { r = x; }
			NEXUS_FRAMEWORK_API void SetY(Type y) { g = y; }
			NEXUS_FRAMEWORK_API void SetZ(Type z) { b = z; }
			NEXUS_FRAMEWORK_API void SetW(Type w) { a = w; }

			Type r, g, b, a;
		};

		struct Linear
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			NEXUS_FRAMEWORK_API Linear();
			NEXUS_FRAMEWORK_API Linear(Type V);
			NEXUS_FRAMEWORK_API Linear(Type R, Type G, Type B);
			NEXUS_FRAMEWORK_API Linear(Type R, Type G, Type B, Type A);
			NEXUS_FRAMEWORK_API Linear(Vector3f V);
			NEXUS_FRAMEWORK_API Linear(Vector4f V);
			NEXUS_FRAMEWORK_API Linear(const sRGB& Other);
			NEXUS_FRAMEWORK_API Linear(const Linear& Other);
			NEXUS_FRAMEWORK_API Linear(const Bits& Other);
			NEXUS_FRAMEWORK_API Linear(const Hsv& Other);
			NEXUS_FRAMEWORK_API ~Linear();

			NEXUS_FRAMEWORK_API Linear& operator=(Linear Other);
			NEXUS_FRAMEWORK_API bool operator==(Linear Other) const;
			NEXUS_FRAMEWORK_API bool operator!=(Linear Other) const;
			NEXUS_FRAMEWORK_API Type& operator[](uint64 Index);
			NEXUS_FRAMEWORK_API const Type& operator[](uint64 Index) const;

			NEXUS_FRAMEWORK_API Linear& operator+=(Type Other) { r += Other;	 g += Other;	b += Other;		a += Other;		return *this; }
			NEXUS_FRAMEWORK_API Linear& operator-=(Type Other) { r -= Other;	 g -= Other;	b -= Other;		a -= Other;		return *this; }
			NEXUS_FRAMEWORK_API Linear& operator*=(Type Other) { r *= Other;	 g *= Other;	b *= Other;		a *= Other;		return *this; }
			NEXUS_FRAMEWORK_API Linear& operator/=(Type Other) { r /= Other;	 g /= Other;	b /= Other;		a /= Other;		return *this; }
			NEXUS_FRAMEWORK_API Linear& operator+=(Linear Other) { r += Other.r; g += Other.g;	b += Other.b;	a += Other.a;	return *this; }
			NEXUS_FRAMEWORK_API Linear& operator-=(Linear Other) { r -= Other.r; g -= Other.g;	b -= Other.b;	a -= Other.a;	return *this; }
			NEXUS_FRAMEWORK_API Linear& operator*=(Linear Other) { r *= Other.r; g *= Other.g;	b *= Other.b;	a *= Other.a;	return *this; }
			NEXUS_FRAMEWORK_API Linear& operator/=(Linear Other) { r /= Other.r; g /= Other.g;	b /= Other.b;	a /= Other.a;	return *this; }

			NEXUS_FRAMEWORK_API Type Min() const;
			NEXUS_FRAMEWORK_API Type Max() const;
			NEXUS_FRAMEWORK_API Linear Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			NEXUS_FRAMEWORK_API sRGB TosRGB() const;
			NEXUS_FRAMEWORK_API Bits ToBits() const;
			NEXUS_FRAMEWORK_API Hsv ToHsv() const;
			NEXUS_FRAMEWORK_API Type ToGrayscale() const;
			NEXUS_FRAMEWORK_API Vector3f ToVector3f() const;
			NEXUS_FRAMEWORK_API Vector4f ToVector4f() const;

			NEXUS_FRAMEWORK_API Type GetX() const { return r; }
			NEXUS_FRAMEWORK_API Type GetY() const { return g; }
			NEXUS_FRAMEWORK_API Type GetZ() const { return b; }
			NEXUS_FRAMEWORK_API Type GetW() const { return a; }
			NEXUS_FRAMEWORK_API void SetX(Type x) { r = x; }
			NEXUS_FRAMEWORK_API void SetY(Type y) { g = y; }
			NEXUS_FRAMEWORK_API void SetZ(Type z) { b = z; }
			NEXUS_FRAMEWORK_API void SetW(Type w) { a = w; }

			Type r, g, b, a;
		};

		struct Bits
		{
			using Type = uint8;

			static inline Type BoundLow = 0;
			static inline Type BoundHigh = 255;

			NEXUS_FRAMEWORK_API Bits();
			NEXUS_FRAMEWORK_API Bits(Type V);
			NEXUS_FRAMEWORK_API Bits(Type R, Type G, Type B);
			NEXUS_FRAMEWORK_API Bits(Type R, Type G, Type B, Type A);
			NEXUS_FRAMEWORK_API Bits(Vector3i V);
			NEXUS_FRAMEWORK_API Bits(Vector4i V);
			NEXUS_FRAMEWORK_API Bits(const sRGB& Other);
			NEXUS_FRAMEWORK_API Bits(const Linear& Other);
			NEXUS_FRAMEWORK_API Bits(const Bits& Other);
			NEXUS_FRAMEWORK_API Bits(const Hsv& Other);
			NEXUS_FRAMEWORK_API ~Bits();

			NEXUS_FRAMEWORK_API Bits& operator=(Bits Other);
			NEXUS_FRAMEWORK_API bool operator==(Bits Other) const;
			NEXUS_FRAMEWORK_API bool operator!=(Bits Other) const;
			NEXUS_FRAMEWORK_API Type& operator[](uint64 Index);
			NEXUS_FRAMEWORK_API const Type& operator[](uint64 Index) const;

			NEXUS_FRAMEWORK_API Bits& operator+=(Type Other) { r += Other;	 g += Other;	b += Other;		a += Other;		return *this; }
			NEXUS_FRAMEWORK_API Bits& operator-=(Type Other) { r -= Other;	 g -= Other;	b -= Other;		a -= Other;		return *this; }
			NEXUS_FRAMEWORK_API Bits& operator*=(Type Other) { r *= Other;	 g *= Other;	b *= Other;		a *= Other;		return *this; }
			NEXUS_FRAMEWORK_API Bits& operator/=(Type Other) { r /= Other;	 g /= Other;	b /= Other;		a /= Other;		return *this; }
			NEXUS_FRAMEWORK_API Bits& operator+=(Bits Other) { r += Other.r; g += Other.g;	b += Other.b;	a += Other.a;	return *this; }
			NEXUS_FRAMEWORK_API Bits& operator-=(Bits Other) { r -= Other.r; g -= Other.g;	b -= Other.b;	a -= Other.a;	return *this; }
			NEXUS_FRAMEWORK_API Bits& operator*=(Bits Other) { r *= Other.r; g *= Other.g;	b *= Other.b;	a *= Other.a;	return *this; }
			NEXUS_FRAMEWORK_API Bits& operator/=(Bits Other) { r /= Other.r; g /= Other.g;	b /= Other.b;	a /= Other.a;	return *this; }

			NEXUS_FRAMEWORK_API Type Min() const;
			NEXUS_FRAMEWORK_API Type Max() const;
			NEXUS_FRAMEWORK_API Bits Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			NEXUS_FRAMEWORK_API sRGB TosRGB() const;
			NEXUS_FRAMEWORK_API Linear ToLinear() const;
			NEXUS_FRAMEWORK_API Hsv ToHsv() const;
			NEXUS_FRAMEWORK_API Type ToGrayscale() const;
			NEXUS_FRAMEWORK_API Vector3f ToVector3f() const;
			NEXUS_FRAMEWORK_API Vector4f ToVector4f() const;

			NEXUS_FRAMEWORK_API Type GetX() const { return r; }
			NEXUS_FRAMEWORK_API Type GetY() const { return g; }
			NEXUS_FRAMEWORK_API Type GetZ() const { return b; }
			NEXUS_FRAMEWORK_API Type GetW() const { return a; }
			NEXUS_FRAMEWORK_API void SetX(Type x) { r = x; }
			NEXUS_FRAMEWORK_API void SetY(Type y) { g = y; }
			NEXUS_FRAMEWORK_API void SetZ(Type z) { b = z; }
			NEXUS_FRAMEWORK_API void SetW(Type w) { a = w; }

			Type r, g, b, a;
		};

		struct Hsv
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			NEXUS_FRAMEWORK_API Hsv();
			NEXUS_FRAMEWORK_API Hsv(Type V);
			NEXUS_FRAMEWORK_API Hsv(Type R, Type G, Type B);
			NEXUS_FRAMEWORK_API Hsv(Type R, Type G, Type B, Type A);
			NEXUS_FRAMEWORK_API Hsv(Vector3f V);
			NEXUS_FRAMEWORK_API Hsv(Vector4f V);
			NEXUS_FRAMEWORK_API Hsv(const sRGB& Other);
			NEXUS_FRAMEWORK_API Hsv(const Linear& Other);
			NEXUS_FRAMEWORK_API Hsv(const Bits& Other);
			NEXUS_FRAMEWORK_API Hsv(const Hsv& Other);
			NEXUS_FRAMEWORK_API ~Hsv();

			NEXUS_FRAMEWORK_API Hsv& operator=(Hsv Other);
			NEXUS_FRAMEWORK_API bool operator==(Hsv Other) const;
			NEXUS_FRAMEWORK_API bool operator!=(Hsv Other) const;
			NEXUS_FRAMEWORK_API Type& operator[](uint64 Index);
			NEXUS_FRAMEWORK_API const Type& operator[](uint64 Index) const;

			NEXUS_FRAMEWORK_API Hsv& operator+=(Hsv Other) { h += Other.h; s += Other.s;	v += Other.v;	a += Other.a;	return *this; }
			NEXUS_FRAMEWORK_API Hsv& operator-=(Hsv Other) { h -= Other.h; s -= Other.s;	v -= Other.v;	a -= Other.a;	return *this; }
			NEXUS_FRAMEWORK_API Hsv& operator*=(Hsv Other) { h *= Other.h; s *= Other.s;	v *= Other.v;	a *= Other.a;	return *this; }
			NEXUS_FRAMEWORK_API Hsv& operator/=(Hsv Other) { h /= Other.h; s /= Other.s;	v /= Other.v;	a /= Other.a;	return *this; }

			NEXUS_FRAMEWORK_API Type Min() const;
			NEXUS_FRAMEWORK_API Type Max() const;
			NEXUS_FRAMEWORK_API Hsv Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			NEXUS_FRAMEWORK_API sRGB TosRGB() const;
			NEXUS_FRAMEWORK_API Linear ToLinear() const;
			NEXUS_FRAMEWORK_API Hsv ToHsv() const;
			NEXUS_FRAMEWORK_API Type ToGrayscale() const;
			NEXUS_FRAMEWORK_API Vector3f ToVector3f() const;
			NEXUS_FRAMEWORK_API Vector4f ToVector4f() const;

			NEXUS_FRAMEWORK_API Type GetX() const { return h; }
			NEXUS_FRAMEWORK_API Type GetY() const { return s; }
			NEXUS_FRAMEWORK_API Type GetZ() const { return v; }
			NEXUS_FRAMEWORK_API Type GetW() const { return a; }
			NEXUS_FRAMEWORK_API void SetX(Type x) { h = x; }
			NEXUS_FRAMEWORK_API void SetY(Type y) { s = y; }
			NEXUS_FRAMEWORK_API void SetZ(Type z) { v = z; }
			NEXUS_FRAMEWORK_API void SetW(Type w) { a = w; }

			Type h, s, v, a;
		};

		namespace Utility
		{
			template<typename T>
			bool Equals(const T& Instance, const T& Other)
			{
				return Math::Equals(Instance.GetX(), Other.GetX()) && Math::Equals(Instance.GetY(), Other.GetY()) && Math::Equals(Instance.GetZ(), Other.GetZ()) && Math::Equals(Instance.GetZ(), Other.GetZ());
			}

			template<typename T>
			typename T::Type Min(const T& Instance)
			{
				return (typename T::Type)Math::Min(Instance.GetX(), Math::Min(Instance.GetY(), Instance.GetZ()));
			}

			template<typename T>
			T Min(const T& Instance, const T& Other)
			{
				return T((typename T::Type)Math::Min(Instance.GetX(), Other.GetX()), (typename T::Type)Math::Min(Instance.GetY(), Other.GetY()), (typename T::Type)Math::Min(Instance.GetZ(), Other.GetZ()), (typename T::Type)Math::Min(Instance.GetW(), Other.GetW()));
			}

			template<typename T>
			typename T::Type Max(const T& Instance)
			{
				return (typename T::Type)Math::Max(Instance.GetX(), Math::Max(Instance.GetY(), Instance.GetZ()));
			}

			template<typename T>
			T Max(const T& Instance, const T& Other)
			{
				return T((typename T::Type)Math::Max(Instance.GetX(), Other.GetX()), (typename T::Type)Math::Max(Instance.GetY(), Other.GetY()), (typename T::Type)Math::Max(Instance.GetZ(), Other.GetZ()), (typename T::Type)Math::Max(Instance.GetW(), Other.GetW()));
			}

			template<typename T>
			T Clamp(const T& Instance, typename T::Type Min, typename T::Type Max)
			{
				return T((typename T::Type)Math::Clamp(Instance.GetX(), Min, Max), (typename T::Type)Math::Clamp(Instance.GetY(), Min, Max), (typename T::Type)Math::Clamp(Instance.GetZ(), Min, Max), (typename T::Type)Math::Clamp(Instance.GetZ()));
			}

			NEXUS_FRAMEWORK_API sRGB TosRGB(Linear Color);
			NEXUS_FRAMEWORK_API sRGB TosRGB(Bits Color);
			NEXUS_FRAMEWORK_API sRGB TosRGB(Hsv Color);
			NEXUS_FRAMEWORK_API Linear ToLinear(sRGB Color);
			NEXUS_FRAMEWORK_API Bits ToBits(sRGB Color);
			NEXUS_FRAMEWORK_API Hsv ToHsv(sRGB Color);
			NEXUS_FRAMEWORK_API float ToGrayscale(Linear Color);
			NEXUS_FRAMEWORK_API sRGB ToHue(float Hue);

			NEXUS_FRAMEWORK_API Linear Add(Linear A, float B);
			NEXUS_FRAMEWORK_API Linear Add(Linear A, Linear B);
			NEXUS_FRAMEWORK_API Linear Subtract(Linear A, float B);
			NEXUS_FRAMEWORK_API Linear Subtract(Linear A, Linear B);
			NEXUS_FRAMEWORK_API Linear Multiply(Linear A, float B);
			NEXUS_FRAMEWORK_API Linear Multiply(Linear A, Linear B);
			NEXUS_FRAMEWORK_API Linear Divide(Linear A, float B);
			NEXUS_FRAMEWORK_API Linear Divide(Linear A, Linear B);

			NEXUS_FRAMEWORK_API sRGB Lerp(sRGB A, sRGB B, float T);
			NEXUS_FRAMEWORK_API Linear Lerp(Linear A, Linear B, float T);
			NEXUS_FRAMEWORK_API Bits Lerp(Bits A, Bits B, float T);
			NEXUS_FRAMEWORK_API Hsv Lerp(Hsv A, Hsv B, float T);
		}
	}

	inline Colors::sRGB operator-(Colors::sRGB A) { return Colors::sRGB(A.r, A.g, A.b, A.a); }
	inline Colors::sRGB operator+(float A, Colors::sRGB B) { return Colors::sRGB(A + B.r, A + B.g, A + B.b, A + B.a); }
	inline Colors::sRGB operator-(float A, Colors::sRGB B) { return Colors::sRGB(A - B.r, A - B.g, A - B.b, A - B.a); }
	inline Colors::sRGB operator*(float A, Colors::sRGB B) { return Colors::sRGB(A * B.r, A * B.g, A * B.b, A * B.a); }
	inline Colors::sRGB operator/(float A, Colors::sRGB B) { return Colors::sRGB(A / B.r, A / B.g, A / B.b, A / B.a); }
	inline Colors::sRGB operator+(Colors::sRGB A, float B) { return Colors::sRGB(A.r + B, A.g + B, A.b + B, A.a + B); }
	inline Colors::sRGB operator-(Colors::sRGB A, float B) { return Colors::sRGB(A.r - B, A.g - B, A.b - B, A.a - B); }
	inline Colors::sRGB operator*(Colors::sRGB A, float B) { return Colors::sRGB(A.r * B, A.g * B, A.b * B, A.a * B); }
	inline Colors::sRGB operator/(Colors::sRGB A, float B) { return Colors::sRGB(A.r / B, A.g / B, A.b / B, A.a / B); }
	inline Colors::sRGB operator+(Colors::sRGB A, Colors::sRGB B) { return Colors::sRGB((A.r + B.r), (A.g + B.g), (A.b + B.b), (A.a + B.a)); }
	inline Colors::sRGB operator-(Colors::sRGB A, Colors::sRGB B) { return Colors::sRGB((A.r - B.r), (A.g - B.g), (A.b - B.b), (A.a - B.a)); }
	inline Colors::sRGB operator*(Colors::sRGB A, Colors::sRGB B) { return Colors::sRGB((A.r * B.r), (A.g * B.g), (A.b * B.b), (A.a * B.a)); }
	inline Colors::sRGB operator/(Colors::sRGB A, Colors::sRGB B) { return Colors::sRGB((A.r / B.r), (A.g / B.g), (A.b / B.b), (A.a / B.a)); }

	inline Colors::Linear operator-(Colors::Linear A) { return Colors::Linear(A.r, A.g, A.b, A.a); }
	inline Colors::Linear operator+(float A, Colors::Linear B) { return Colors::Linear(A + B.r, A + B.g, A + B.b, A + B.a); }
	inline Colors::Linear operator-(float A, Colors::Linear B) { return Colors::Linear(A - B.r, A - B.g, A - B.b, A - B.a); }
	inline Colors::Linear operator*(float A, Colors::Linear B) { return Colors::Linear(A * B.r, A * B.g, A * B.b, A * B.a); }
	inline Colors::Linear operator/(float A, Colors::Linear B) { return Colors::Linear(A / B.r, A / B.g, A / B.b, A / B.a); }
	inline Colors::Linear operator+(Colors::Linear A, float B) { return Colors::Linear(A.r + B, A.g + B, A.b + B, A.a + B); }
	inline Colors::Linear operator-(Colors::Linear A, float B) { return Colors::Linear(A.r - B, A.g - B, A.b - B, A.a - B); }
	inline Colors::Linear operator*(Colors::Linear A, float B) { return Colors::Linear(A.r * B, A.g * B, A.b * B, A.a * B); }
	inline Colors::Linear operator/(Colors::Linear A, float B) { return Colors::Linear(A.r / B, A.g / B, A.b / B, A.a / B); }
	inline Colors::Linear operator+(Colors::Linear A, Colors::Linear B) { return Colors::Linear((A.r + B.r), (A.g + B.g), (A.b + B.b), (A.a + B.a)); }
	inline Colors::Linear operator-(Colors::Linear A, Colors::Linear B) { return Colors::Linear((A.r - B.r), (A.g - B.g), (A.b - B.b), (A.a - B.a)); }
	inline Colors::Linear operator*(Colors::Linear A, Colors::Linear B) { return Colors::Linear((A.r * B.r), (A.g * B.g), (A.b * B.b), (A.a * B.a)); }
	inline Colors::Linear operator/(Colors::Linear A, Colors::Linear B) { return Colors::Linear((A.r / B.r), (A.g / B.g), (A.b / B.b), (A.a / B.a)); }

	inline Colors::Bits operator-(Colors::Bits A) { return Colors::Bits(A.r, A.g, A.b, A.a); }
	inline Colors::Bits operator+(uint32 A, Colors::Bits B) { return Colors::Bits(A + B.r, A + B.g, A + B.b, A + B.a); }
	inline Colors::Bits operator-(uint32 A, Colors::Bits B) { return Colors::Bits(A - B.r, A - B.g, A - B.b, A - B.a); }
	inline Colors::Bits operator*(uint32 A, Colors::Bits B) { return Colors::Bits(A * B.r, A * B.g, A * B.b, A * B.a); }
	inline Colors::Bits operator/(uint32 A, Colors::Bits B) { return Colors::Bits(A / B.r, A / B.g, A / B.b, A / B.a); }
	inline Colors::Bits operator+(Colors::Bits A, uint32 B) { return Colors::Bits(A.r + B, A.g + B, A.b + B, A.a + B); }
	inline Colors::Bits operator-(Colors::Bits A, uint32 B) { return Colors::Bits(A.r - B, A.g - B, A.b - B, A.a - B); }
	inline Colors::Bits operator*(Colors::Bits A, uint32 B) { return Colors::Bits(A.r * B, A.g * B, A.b * B, A.a * B); }
	inline Colors::Bits operator/(Colors::Bits A, uint32 B) { return Colors::Bits(A.r / B, A.g / B, A.b / B, A.a / B); }
	inline Colors::Bits operator+(Colors::Bits A, Colors::Bits B) { return Colors::Bits((A.r + B.r), (A.g + B.g), (A.b + B.b), (A.a + B.a)); }
	inline Colors::Bits operator-(Colors::Bits A, Colors::Bits B) { return Colors::Bits((A.r - B.r), (A.g - B.g), (A.b - B.b), (A.a - B.a)); }
	inline Colors::Bits operator*(Colors::Bits A, Colors::Bits B) { return Colors::Bits((A.r * B.r), (A.g * B.g), (A.b * B.b), (A.a * B.a)); }
	inline Colors::Bits operator/(Colors::Bits A, Colors::Bits B) { return Colors::Bits((A.r / B.r), (A.g / B.g), (A.b / B.b), (A.a / B.a)); }

	inline Colors::Hsv operator-(Colors::Hsv A) { return Colors::Hsv(A.h, A.s, A.v, A.a); }
	inline Colors::Hsv operator+(float A, Colors::Hsv B) { return Colors::Hsv(A + B.h, A + B.s, A + B.v, A + B.a); }
	inline Colors::Hsv operator-(float A, Colors::Hsv B) { return Colors::Hsv(A - B.h, A - B.s, A - B.v, A - B.a); }
	inline Colors::Hsv operator*(float A, Colors::Hsv B) { return Colors::Hsv(A * B.h, A * B.s, A * B.v, A * B.a); }
	inline Colors::Hsv operator/(float A, Colors::Hsv B) { return Colors::Hsv(A / B.h, A / B.s, A / B.v, A / B.a); }
	inline Colors::Hsv operator+(Colors::Hsv A, float B) { return Colors::Hsv(A.h + B, A.s + B, A.v + B, A.a + B); }
	inline Colors::Hsv operator-(Colors::Hsv A, float B) { return Colors::Hsv(A.h - B, A.s - B, A.v - B, A.a - B); }
	inline Colors::Hsv operator*(Colors::Hsv A, float B) { return Colors::Hsv(A.h * B, A.s * B, A.v * B, A.a * B); }
	inline Colors::Hsv operator/(Colors::Hsv A, float B) { return Colors::Hsv(A.h / B, A.s / B, A.v / B, A.a / B); }
	inline Colors::Hsv operator+(Colors::Hsv A, Colors::Hsv B) { return Colors::Hsv((A.h + B.h), (A.s + B.s), (A.v + B.v), (A.a + B.a)); }
	inline Colors::Hsv operator-(Colors::Hsv A, Colors::Hsv B) { return Colors::Hsv((A.h - B.h), (A.s - B.s), (A.v - B.v), (A.a - B.a)); }
	inline Colors::Hsv operator*(Colors::Hsv A, Colors::Hsv B) { return Colors::Hsv((A.h * B.h), (A.s * B.s), (A.v * B.v), (A.a * B.a)); }
	inline Colors::Hsv operator/(Colors::Hsv A, Colors::Hsv B) { return Colors::Hsv((A.h / B.h), (A.s / B.s), (A.v / B.v), (A.a / B.a)); }

	namespace Hashing
	{
		template<typename H>
		class HashProcess<Color, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Color& Data)
			{
				HashProcess<float, H>::Accumulate(State, Data.r);
				HashProcess<float, H>::Accumulate(State, Data.g);
				HashProcess<float, H>::Accumulate(State, Data.b);
				HashProcess<float, H>::Accumulate(State, Data.a);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Color& Data)
			{
				HashProcess<Color, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};
	}

	template<>
	struct RBSConverter<Color>
	{
		static Color Decode(RBS& Rbs)
		{
			Color Result;
			Result.r = Rbs.ReadObject<float>();
			Result.g = Rbs.ReadObject<float>();
			Result.b = Rbs.ReadObject<float>();
			Result.a = Rbs.ReadObject<float>();
			return Result;
		}

		static void Encode(RBS& Rbs, const Color& Object)
		{
			Rbs.WriteObject(Object.r);
			Rbs.WriteObject(Object.g);
			Rbs.WriteObject(Object.b);
			Rbs.WriteObject(Object.a);
		}
	};

	template<>
	struct StringConverter<Color>
	{
		static StringView GetFormat(bool Pretty)
		{
			return Pretty ? "(%.2f, %.2f, %.2f, %.2f)" : "(%f, %f, %f, %f)";
		}

		static void ToString(const Color& Data, String& Result, StringView Format = "")
		{
			Result.Format(StringUtility::ConvertionFormat<Color>(Format, true), Data.r, Data.g, Data.b, Data.a);
		}

		static void FromString(StringView Data, Color& Result, StringView Format = "")
		{
			StringUtility::Scan(Data, StringUtility::ConvertionFormat<Color>(Format, false), &Result.r, &Result.g, &Result.b, &Result.a);
		}
	};
}

namespace YAML
{
	template<>
	struct convert<NxFr::Color>
	{
		static Node encode(const NxFr::Color& rhs)
		{
			Node node;
			node.SetStyle(YAML::EmitterStyle::Flow);

			node.push_back(rhs.r);
			node.push_back(rhs.g);
			node.push_back(rhs.b);
			node.push_back(rhs.a);

			return node;
		}

		static bool decode(const Node& node, NxFr::Color& rhs)
		{
			if (!node.IsSequence())
			{
				return false;
			}

			rhs.r = node[0].as<float>();
			rhs.g = node[1].as<float>();
			rhs.b = node[2].as<float>();
			rhs.a = node[3].as<float>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Color& rhs)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq;

		out << rhs.r;
		out << rhs.g;
		out << rhs.b;
		out << rhs.a;

		out << YAML::EndSeq;

		return out;
	}
}
