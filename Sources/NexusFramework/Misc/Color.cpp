#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Misc/Color.h"

namespace NxFr
{
	namespace Colors
	{
#pragma region Const

		const Color Clear = Color(0.0f, 0.0f, 0.0f, 0.0f);
		const Color Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
		const Color Gray = Color(0.5f, 0.5f, 0.5f, 1.0f);
		const Color White = Color(1.0f, 1.0f, 1.0f, 1.0f);
		const Color Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
		const Color Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
		const Color Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
		const Color Yellow = Color(1.0f, 1.0f, 0.0f, 1.0f);
		const Color Cyan = Color(0.0f, 1.0f, 1.0f, 1.0f);
		const Color Magenta = Color(1.0f, 0.0f, 1.0f, 1.0f);
		const Color Orange = Color(1.0f, 0.5f, 0.0f, 1.0f);

#pragma endregion

#pragma region sRGB

		sRGB::sRGB()
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundHigh)
		{
		}

		sRGB::sRGB(Type V)
			: r(V), g(V), b(V), a(BoundHigh)
		{
		}

		sRGB::sRGB(Type R, Type G, Type B)
			: r(R), g(G), b(B), a(BoundHigh)
		{
		}

		sRGB::sRGB(Type R, Type G, Type B, Type A)
			: r(R), g(G), b(B), a(A)
		{
		}

		sRGB::sRGB(Vector3f V)
			: r(V.x), g(V.y), b(V.z), a(BoundHigh)
		{
		}

		sRGB::sRGB(Vector4f V)
			: r(V.x), g(V.y), b(V.z), a(V.w)
		{
		}

		sRGB::sRGB(const sRGB& Other)
			: r(Other.r), g(Other.g), b(Other.b), a(Other.a)
		{
		}

		sRGB::sRGB(const Linear& Other)
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow)
		{
			*this = Utility::TosRGB(Other);
		}

		sRGB::sRGB(const Bits& Other)
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow)
		{
			*this = Utility::TosRGB(Other);
		}

		sRGB::sRGB(const Hsv& Other)
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow)
		{
			*this = Utility::TosRGB(Other);
		}

		sRGB::~sRGB()
		{
		}

		sRGB& sRGB::operator=(sRGB Other)
		{
			r = Other.r;
			g = Other.g;
			b = Other.b;
			a = Other.a;
			return *this;
		}

		bool sRGB::operator==(sRGB Other) const
		{
			return Utility::Equals(*this, Other);
		}

		bool sRGB::operator!=(sRGB Other) const
		{
			return !Utility::Equals(*this, Other);
		}

		sRGB::Type& sRGB::operator[](uint64 Index)
		{
			return Index == BoundLow ? r : Index == 1 ? g : Index == 2 ? b : Index == 3 ? a : r;
		}

		const sRGB::Type& sRGB::operator[](uint64 Index) const
		{
			return Index == BoundLow ? r : Index == 1 ? g : Index == 2 ? b : Index == 3 ? a : r;
		}

		sRGB::Type sRGB::Min() const
		{
			return Utility::Min(*this);
		}

		sRGB::Type sRGB::Max() const
		{
			return Utility::Max(*this);
		}

		sRGB sRGB::Clamp(Type Min, Type Max) const
		{
			return Utility::Clamp(*this, Min, Max);
		}

		Linear sRGB::ToLinear() const
		{
			return Utility::ToLinear(*this);
		}

		Bits sRGB::ToBits() const
		{
			return Utility::ToBits(*this);
		}

		Hsv sRGB::ToHsv() const
		{
			return Utility::ToHsv(*this);
		}

		sRGB::Type sRGB::ToGrayscale() const
		{
			return Utility::ToGrayscale(*this);
		}

		Vector3f sRGB::ToVector3f() const
		{
			return Vector3f(r, g, b);
		}

		Vector4f sRGB::ToVector4f() const
		{
			return Vector4f(r, g, b, a);
		}

		String sRGB::ToString() const
		{
			return Utility::ToString(*this);
		}

#pragma endregion

#pragma region Linear

		Linear::Linear()
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundHigh)
		{
		}

		Linear::Linear(Type V)
			: r(V), g(V), b(V), a(BoundHigh)
		{
		}

		Linear::Linear(Type R, Type G, Type B)
			: r(R), g(G), b(B), a(BoundHigh)
		{
		}

		Linear::Linear(Type R, Type G, Type B, Type A)
			: r(R), g(G), b(B), a(A)
		{
		}

		Linear::Linear(Vector3f V)
			: r(V.x), g(V.y), b(V.z), a(BoundHigh)
		{
		}

		Linear::Linear(Vector4f V)
			: r(V.x), g(V.y), b(V.z), a(V.w)
		{
		}

		Linear::Linear(const sRGB& Other)
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow)
		{
			*this = Utility::ToLinear(Other);
		}

		Linear::Linear(const Linear& Other)
			: r(Other.r), g(Other.g), b(Other.b), a(Other.a)
		{
		}

		Linear::Linear(const Bits& Other)
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow)
		{
			*this = Utility::ToLinear(Utility::TosRGB(Other));
		}

		Linear::Linear(const Hsv& Other)
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow)
		{
			*this = Utility::ToLinear(Utility::TosRGB(Other));
		}

		Linear::~Linear()
		{
		}

		Linear& Linear::operator=(Linear Other)
		{
			r = Other.r;
			g = Other.g;
			b = Other.b;
			a = Other.a;
			return *this;
		}

		bool Linear::operator==(Linear Other) const
		{
			return Utility::Equals(*this, Other);
		}

		bool Linear::operator!=(Linear Other) const
		{
			return !Utility::Equals(*this, Other);
		}

		Linear::Type& Linear::operator[](uint64 Index)
		{
			return Index == BoundLow ? r : Index == 1 ? g : Index == 2 ? b : Index == 3 ? a : r;
		}

		const Linear::Type& Linear::operator[](uint64 Index) const
		{
			return Index == BoundLow ? r : Index == 1 ? g : Index == 2 ? b : Index == 3 ? a : r;
		}

		Linear::Type Linear::Min() const
		{
			return Utility::Min(*this);
		}

		Linear::Type Linear::Max() const
		{
			return Utility::Max(*this);
		}

		Linear Linear::Clamp(Type Min, Type Max) const
		{
			return Utility::Clamp(*this, Min, Max);
		}

		sRGB Linear::TosRGB() const
		{
			return Utility::TosRGB(*this);
		}

		Bits Linear::ToBits() const
		{
			return Utility::ToBits(*this);
		}

		Hsv Linear::ToHsv() const
		{
			return Utility::ToHsv(*this);
		}

		Linear::Type Linear::ToGrayscale() const
		{
			return Utility::ToGrayscale(*this);
		}

		Vector3f Linear::ToVector3f() const
		{
			return Vector3f(r, g, b);
		}

		Vector4f Linear::ToVector4f() const
		{
			return Vector4f(r, g, b, a);
		}

		String Linear::ToString() const
		{
			return Utility::ToString(*this);
		}

#pragma endregion

#pragma region Bits

		Bits::Bits()
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundHigh)
		{
		}

		Bits::Bits(Type V)
			: r(V), g(V), b(V), a(BoundHigh)
		{
		}

		Bits::Bits(Type R, Type G, Type B)
			: r(R), g(G), b(B), a(BoundHigh)
		{
		}

		Bits::Bits(Type R, Type G, Type B, Type A)
			: r(R), g(G), b(B), a(A)
		{
		}

		Bits::Bits(Vector3i V)
			: r(V.x), g(V.y), b(V.z), a(BoundHigh)
		{
		}

		Bits::Bits(Vector4i V)
			: r(V.x), g(V.y), b(V.z), a(V.w)
		{
		}

		Bits::Bits(const sRGB& Other)
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow)
		{
			*this = Utility::ToBits(Other);
		}

		Bits::Bits(const Linear& Other)
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow)
		{
			*this = Utility::ToBits(Utility::TosRGB(Other));
		}

		Bits::Bits(const Bits& Other)
			: r(Other.r), g(Other.g), b(Other.b), a(Other.a)
		{
		}

		Bits::Bits(const Hsv& Other)
			: r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow)
		{
			*this = Utility::ToBits(Utility::TosRGB(Other));
		}

		Bits::~Bits()
		{
		}

		Bits& Bits::operator=(Bits Other)
		{
			r = Other.r;
			g = Other.g;
			b = Other.b;
			a = Other.a;
			return *this;
		}

		bool Bits::operator==(Bits Other) const
		{
			return Utility::Equals(*this, Other);
		}

		bool Bits::operator!=(Bits Other) const
		{
			return !Utility::Equals(*this, Other);
		}

		Bits::Type& Bits::operator[](uint64 Index)
		{
			return Index == BoundLow ? r : Index == 1 ? g : Index == 2 ? b : Index == 3 ? a : r;
		}

		const Bits::Type& Bits::operator[](uint64 Index) const
		{
			return Index == BoundLow ? r : Index == 1 ? g : Index == 2 ? b : Index == 3 ? a : r;
		}

		Bits::Type Bits::Min() const
		{
			return Utility::Min(*this);
		}

		Bits::Type Bits::Max() const
		{
			return Utility::Max(*this);
		}

		Bits Bits::Clamp(Type Min, Type Max) const
		{
			return Utility::Clamp(*this, Min, Max);
		}

		sRGB Bits::TosRGB() const
		{
			return Utility::TosRGB(*this);
		}

		Linear Bits::ToLinear() const
		{
			return Utility::ToLinear(*this);
		}

		Hsv Bits::ToHsv() const
		{
			return Utility::ToHsv(*this);
		}

		Bits::Type Bits::ToGrayscale() const
		{
			return (Bits::Type)(Utility::ToGrayscale(*this) * BoundHigh);
		}

		Vector3f Bits::ToVector3f() const
		{
			return Vector3f((float)r, (float)g, (float)b);
		}

		Vector4f Bits::ToVector4f() const
		{
			return Vector4f((float)r, (float)g, (float)b, (float)a);
		}

		String Bits::ToString() const
		{
			return Utility::ToString(*this);
		}

#pragma endregion

#pragma region Hsv

		Hsv::Hsv()
			: h(BoundLow), s(BoundLow), v(BoundLow), a(BoundHigh)
		{
		}

		Hsv::Hsv(Type V)
			: h(V), s(V), v(V), a(BoundHigh)
		{
		}

		Hsv::Hsv(Type R, Type G, Type B)
			: h(R), s(G), v(B), a(BoundHigh)
		{
		}

		Hsv::Hsv(Type R, Type G, Type B, Type A)
			: h(R), s(G), v(B), a(A)
		{
		}

		Hsv::Hsv(Vector3f V)
			: h(V.x), s(V.y), v(V.z), a(BoundHigh)
		{
		}

		Hsv::Hsv(Vector4f V)
			: h(V.x), s(V.y), v(V.z), a(V.w)
		{
		}

		Hsv::Hsv(const sRGB& Other)
			: h(BoundLow), s(BoundLow), v(BoundLow), a(BoundLow)
		{
			*this = Utility::ToHsv(Other);
		}

		Hsv::Hsv(const Linear& Other)
			: h(BoundLow), s(BoundLow), v(BoundLow), a(BoundLow)
		{
			*this = Utility::ToHsv(Utility::TosRGB(Other));
		}

		Hsv::Hsv(const Bits& Other)
			: h(BoundLow), s(BoundLow), v(BoundLow), a(BoundLow)
		{
			*this = Utility::ToHsv(Utility::TosRGB(Other));
		}

		Hsv::Hsv(const Hsv& Other)
			: h(Other.h), s(Other.s), v(Other.v), a(Other.a)
		{
		}

		Hsv::~Hsv()
		{
		}

		Hsv& Hsv::operator=(Hsv Other)
		{
			h = Other.h;
			s = Other.s;
			v = Other.v;
			a = Other.a;
			return *this;
		}

		bool Hsv::operator==(Hsv Other) const
		{
			return Utility::Equals(*this, Other);
		}

		bool Hsv::operator!=(Hsv Other) const
		{
			return !Utility::Equals(*this, Other);
		}

		Hsv::Type& Hsv::operator[](uint64 Index)
		{
			return Index == BoundLow ? h : Index == 1 ? s : Index == 2 ? v : Index == 3 ? a : h;
		}

		const Hsv::Type& Hsv::operator[](uint64 Index) const
		{
			return Index == BoundLow ? h : Index == 1 ? s : Index == 2 ? v : Index == 3 ? a : h;
		}

		Hsv::Type Hsv::Min() const
		{
			return Utility::Min(*this);
		}

		Hsv::Type Hsv::Max() const
		{
			return Utility::Max(*this);
		}

		Hsv Hsv::Clamp(Type Min, Type Max) const
		{
			return Utility::Clamp(*this, Min, Max);
		}

		sRGB Hsv::TosRGB() const
		{
			return Utility::TosRGB(*this);
		}

		Linear Hsv::ToLinear() const
		{
			return Utility::ToLinear(*this);
		}

		Hsv Hsv::ToHsv() const
		{
			return Utility::ToHsv(*this);
		}

		Hsv::Type Hsv::ToGrayscale() const
		{
			return Utility::ToGrayscale(*this);
		}

		Vector3f Hsv::ToVector3f() const
		{
			return Vector3f(h, s, v);
		}

		Vector4f Hsv::ToVector4f() const
		{
			return Vector4f(h, s, v, a);
		}

		String Hsv::ToString() const
		{
			return Utility::ToString(*this);
		}

#pragma endregion

#pragma region Convertion

		sRGB NxFr::Colors::Utility::TosRGB(Linear Color)
		{
			sRGB Result;
			Result.r = Color.r <= 0.0031308f ? Color.r * 12.92f : 1.055f * Math::Pow(Color.r, 1.0f / 2.4f) - 0.055f;
			Result.g = Color.g <= 0.0031308f ? Color.g * 12.92f : 1.055f * Math::Pow(Color.g, 1.0f / 2.4f) - 0.055f;
			Result.b = Color.b <= 0.0031308f ? Color.b * 12.92f : 1.055f * Math::Pow(Color.b, 1.0f / 2.4f) - 0.055f;
			Result.a = Color.a;
			return Result.Clamp();
		}

		sRGB NxFr::Colors::Utility::TosRGB(Bits Color)
		{
			sRGB Result;
			Result.r = Color.r / (sRGB::Type)Bits::BoundHigh;
			Result.g = Color.g / (sRGB::Type)Bits::BoundHigh;
			Result.b = Color.b / (sRGB::Type)Bits::BoundHigh;
			Result.a = Color.a / (sRGB::Type)Bits::BoundHigh;
			return Result.Clamp();
		}

		sRGB NxFr::Colors::Utility::TosRGB(Hsv Color)
		{
			sRGB Result = ToHue(Color.h);
			Result = VectorUtility::Lerp(Vector4f::One, Result.ToVector4f(), Color.s);
			Result = Result * Color.v;
			Result.a = Color.a;
			return Result;
		}

		Linear NxFr::Colors::Utility::ToLinear(sRGB Color)
		{
			Linear Result;
			Result.r = Color.r <= 0.0031308f ? Color.r / 12.92f : Math::Pow((Color.r + 0.055f) / 1.055f, 2.4f);
			Result.g = Color.g <= 0.0031308f ? Color.g / 12.92f : Math::Pow((Color.g + 0.055f) / 1.055f, 2.4f);
			Result.b = Color.b <= 0.0031308f ? Color.b / 12.92f : Math::Pow((Color.b + 0.055f) / 1.055f, 2.4f);
			Result.a = Color.a;
			return Result.Clamp();
		}

		Bits NxFr::Colors::Utility::ToBits(sRGB Color)
		{
			Bits Result;
			Result.r = (Bits::Type)(Color.r * Bits::BoundHigh);
			Result.g = (Bits::Type)(Color.g * Bits::BoundHigh);
			Result.b = (Bits::Type)(Color.b * Bits::BoundHigh);
			Result.a = (Bits::Type)(Color.a * Bits::BoundHigh);
			return Result.Clamp();
		}

		Hsv NxFr::Colors::Utility::ToHsv(sRGB Color)
		{
			Hsv Result;

			sRGB::Type MaxComponent = Color.Max();
			sRGB::Type MinComponent = Color.Min();
			sRGB::Type Delta = MaxComponent - MinComponent;

			sRGB::Type Hue = 0;
			if		(Color.r == MaxComponent)
			{
				Hue = 0 + (Color.g - Color.b) / Delta;
			}
			else if (Color.g == MaxComponent)
			{
				Hue = 2 + (Color.b - Color.r) / Delta;
			}
			else if (Color.b == MaxComponent)
			{
				Hue = 4 + (Color.r - Color.g) / Delta;
			}

			Result.h = Math::Frac(Hue / 6.0f);
			Result.s = Delta / MaxComponent;
			Result.v = MaxComponent;
			Result.a = Color.a;

			return Result.Clamp();
		}

		float NxFr::Colors::Utility::ToGrayscale(Linear Color)
		{
			return 0.2126f * Color.r + 0.7152f * Color.g + 0.0722f * Color.b;
		}

		sRGB Utility::ToHue(float Hue)
		{
			Hue = Math::Frac(Hue);

			sRGB Result;
			Result.r = Math::Abs(Hue * 6.0f - 3.0f) - 1.0f;
			Result.g = 2.0f - Math::Abs(Hue * 6.0f - 2.0f);
			Result.b = 2.0f - Math::Abs(Hue * 6.0f - 4.0f);
			return Result.Clamp();
		}

		Linear Utility::Add(Linear A, float B)
		{
			return A + B;
		}

		Linear Utility::Add(Linear A, Linear B)
		{
			return A + B;
		}

		Linear Utility::Subtract(Linear A, float B)
		{
			return A - B;
		}

		Linear Utility::Subtract(Linear A, Linear B)
		{
			return A - B;
		}

		Linear Utility::Multiply(Linear A, float B)
		{
			return A * B;
		}

		Linear Utility::Multiply(Linear A, Linear B)
		{
			return A * B;
		}

		Linear Utility::Divide(Linear A, float B)
		{
			return A / B;
		}

		Linear Utility::Divide(Linear A, Linear B)
		{
			return A / B;
		}

		sRGB Utility::Lerp(sRGB A, sRGB B, float T)
		{
			return A * (1.0f - T) + B * T;
		}

		Linear Utility::Lerp(Linear A, Linear B, float T)
		{
			return A * (1.0f - T) + B * T;
		}

		Bits Utility::Lerp(Bits A, Bits B, float T)
		{
			return Lerp(A.TosRGB(), B.TosRGB(), T);
		}

		Hsv Utility::Lerp(Hsv A, Hsv B, float T)
		{
			return Lerp(A.ToLinear(), B.ToLinear(), T);
		}

#pragma endregion
	}
}
