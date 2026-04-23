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

		sRGB::sRGB() : r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow) {}
		sRGB::sRGB(Type V) : r(V), g(V), b(V), a(BoundHigh) {}
		sRGB::sRGB(Type R, Type G, Type B) : r(R), g(G), b(B), a(BoundHigh) {}
		sRGB::sRGB(Type R, Type G, Type B, Type A) : r(R), g(G), b(B), a(A) {}
		sRGB::sRGB(Vector3f V) : r(V.x), g(V.y), b(V.z), a(BoundHigh) {}
		sRGB::sRGB(Vector4f V) : r(V.x), g(V.y), b(V.z), a(V.w) {}
		sRGB::sRGB(Linear Other) { *this = Utility::TosRGB(Other); }
		sRGB::sRGB(Bits Other) { *this = Utility::TosRGB(Other); }
		sRGB::sRGB(Hsv Other) { *this = Utility::TosRGB(Other); }

		sRGB::operator Vector3f() const { return Utility::ToVector3f(*this); }
		sRGB::operator Vector4f() const { return Utility::ToVector4f(*this); }

		Vector3f sRGB::ToVector3f() const { return Utility::ToVector3f(*this); }
		Vector4f sRGB::ToVector4f() const { return Utility::ToVector4f(*this); }
		float sRGB::ToGrayscale() const { return Utility::ToGrayscale(*this); }
		Linear sRGB::ToLinear() const { return Utility::ToLinear(*this); }
		Bits sRGB::ToBits() const { return Utility::ToBits(*this); }
		Hsv sRGB::ToHsv() const { return Utility::ToHsv(*this); }

		bool sRGB::operator==(sRGB Other) const { return Utility::Equals(*this, Other); }
		bool sRGB::operator!=(sRGB Other) const { return !Utility::Equals(*this, Other); }
		sRGB::Type& sRGB::operator[](uint64 Index) { return Utility::Access(*this, Index); }
		const sRGB::Type& sRGB::operator[](uint64 Index) const { return Utility::Access(*this, Index); }

		sRGB& sRGB::operator+=(Type Other) { *this = Utility::Add(*this, Other); return *this; }
		sRGB& sRGB::operator-=(Type Other) { *this = Utility::Subtract(*this, Other); return *this; }
		sRGB& sRGB::operator*=(Type Other) { *this = Utility::Multiply(*this, Other); return *this; }
		sRGB& sRGB::operator/=(Type Other) { *this = Utility::Divide(*this, Other); return *this; }
		sRGB& sRGB::operator+=(sRGB Other) { *this = Utility::Add(*this, Other);	return *this; }
		sRGB& sRGB::operator-=(sRGB Other) { *this = Utility::Subtract(*this, Other); return *this; }
		sRGB& sRGB::operator*=(sRGB Other) { *this = Utility::Multiply(*this, Other); return *this; }
		sRGB& sRGB::operator/=(sRGB Other) { *this = Utility::Divide(*this, Other); return *this; }

		sRGB::Type sRGB::Min() const { return Utility::Min(*this); }
		sRGB::Type sRGB::Max() const { return Utility::Max(*this); }
		sRGB sRGB::Clamp(Type Min, Type Max) const { return Utility::Clamp(*this, Min, Max); }

		sRGB::Type& sRGB::GetX() { return r; }
		sRGB::Type& sRGB::GetY() { return g; }
		sRGB::Type& sRGB::GetZ() { return b; }
		sRGB::Type& sRGB::GetW() { return a; }
		const sRGB::Type& sRGB::GetX() const { return r; }
		const sRGB::Type& sRGB::GetY() const { return g; }
		const sRGB::Type& sRGB::GetZ() const { return b; }
		const sRGB::Type& sRGB::GetW() const { return a; }
		void sRGB::SetX(Type X) { r = X; }
		void sRGB::SetY(Type Y) { g = Y; }
		void sRGB::SetZ(Type Z) { b = Z; }
		void sRGB::SetW(Type W) { a = W; }

		sRGB operator+(sRGB Instance, sRGB::Type Other) { return Utility::Add(Instance, Other); }
		sRGB operator-(sRGB Instance, sRGB::Type Other) { return Utility::Subtract(Instance, Other); }
		sRGB operator*(sRGB Instance, sRGB::Type Other) { return Utility::Multiply(Instance, Other); }
		sRGB operator/(sRGB Instance, sRGB::Type Other) { return Utility::Divide(Instance, Other); }
		sRGB operator+(sRGB Instance, sRGB Other) { return Utility::Add(Instance, Other); }
		sRGB operator-(sRGB Instance, sRGB Other) { return Utility::Subtract(Instance, Other); }
		sRGB operator*(sRGB Instance, sRGB Other) { return Utility::Multiply(Instance, Other); }
		sRGB operator/(sRGB Instance, sRGB Other) { return Utility::Divide(Instance, Other); }

#pragma endregion

#pragma region Linear

		Linear::Linear() : r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow) {}
		Linear::Linear(Type V) : r(V), g(V), b(V), a(BoundHigh) {}
		Linear::Linear(Type R, Type G, Type B) : r(R), g(G), b(B), a(BoundHigh) {}
		Linear::Linear(Type R, Type G, Type B, Type A) : r(R), g(G), b(B), a(A) {}
		Linear::Linear(Vector3f V) : r(V.x), g(V.y), b(V.z), a(BoundHigh) {}
		Linear::Linear(Vector4f V) : r(V.x), g(V.y), b(V.z), a(V.w) {}
		Linear::Linear(sRGB Other) { *this = Utility::ToLinear(Other); }
		Linear::Linear(Bits Other) { *this = Utility::ToLinear(Utility::TosRGB(Other)); }
		Linear::Linear(Hsv Other) { *this = Utility::ToLinear(Utility::TosRGB(Other)); }

		Linear::operator Vector3f() const { return Utility::ToVector3f(*this); }
		Linear::operator Vector4f() const { return Utility::ToVector4f(*this); }

		Vector3f Linear::ToVector3f() const { return Utility::ToVector3f(*this); }
		Vector4f Linear::ToVector4f() const { return Utility::ToVector4f(*this); }
		float Linear::ToGrayscale() const { return Utility::ToGrayscale(*this); }
		sRGB Linear::TosRGB() const { return Utility::TosRGB(*this); }
		Bits Linear::ToBits() const { return Utility::ToBits(*this); }
		Hsv Linear::ToHsv() const { return Utility::ToHsv(*this); }

		bool Linear::operator==(Linear Other) const { return Utility::Equals(*this, Other); }
		bool Linear::operator!=(Linear Other) const { return !Utility::Equals(*this, Other); }
		Linear::Type& Linear::operator[](uint64 Index) { return Utility::Access(*this, Index); }
		const Linear::Type& Linear::operator[](uint64 Index) const { return Utility::Access(*this, Index); }

		Linear& Linear::operator+=(Type Other) { *this = Utility::Add(*this, Other); return *this; }
		Linear& Linear::operator-=(Type Other) { *this = Utility::Subtract(*this, Other); return *this; }
		Linear& Linear::operator*=(Type Other) { *this = Utility::Multiply(*this, Other); return *this; }
		Linear& Linear::operator/=(Type Other) { *this = Utility::Divide(*this, Other); return *this; }
		Linear& Linear::operator+=(Linear Other) { *this = Utility::Add(*this, Other);	return *this; }
		Linear& Linear::operator-=(Linear Other) { *this = Utility::Subtract(*this, Other); return *this; }
		Linear& Linear::operator*=(Linear Other) { *this = Utility::Multiply(*this, Other); return *this; }
		Linear& Linear::operator/=(Linear Other) { *this = Utility::Divide(*this, Other); return *this; }

		Linear::Type Linear::Min() const { return Utility::Min(*this); }
		Linear::Type Linear::Max() const { return Utility::Max(*this); }
		Linear Linear::Clamp(Type Min, Type Max) const { return Utility::Clamp(*this, Min, Max); }

		Linear::Type& Linear::GetX() { return r; }
		Linear::Type& Linear::GetY() { return g; }
		Linear::Type& Linear::GetZ() { return b; }
		Linear::Type& Linear::GetW() { return a; }
		const Linear::Type& Linear::GetX() const { return r; }
		const Linear::Type& Linear::GetY() const { return g; }
		const Linear::Type& Linear::GetZ() const { return b; }
		const Linear::Type& Linear::GetW() const { return a; }
		void Linear::SetX(Type X) { r = X; }
		void Linear::SetY(Type Y) { g = Y; }
		void Linear::SetZ(Type Z) { b = Z; }
		void Linear::SetW(Type W) { a = W; }

		Linear operator+(Linear Instance, Linear::Type Other) { return Utility::Add(Instance, Other); }
		Linear operator-(Linear Instance, Linear::Type Other) { return Utility::Subtract(Instance, Other); }
		Linear operator*(Linear Instance, Linear::Type Other) { return Utility::Multiply(Instance, Other); }
		Linear operator/(Linear Instance, Linear::Type Other) { return Utility::Divide(Instance, Other); }
		Linear operator+(Linear Instance, Linear Other) { return Utility::Add(Instance, Other); }
		Linear operator-(Linear Instance, Linear Other) { return Utility::Subtract(Instance, Other); }
		Linear operator*(Linear Instance, Linear Other) { return Utility::Multiply(Instance, Other); }
		Linear operator/(Linear Instance, Linear Other) { return Utility::Divide(Instance, Other); }

#pragma endregion

#pragma region Bits

		Bits::Bits() : r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow) {}
		Bits::Bits(Type V) : r(V), g(V), b(V), a(BoundHigh) {}
		Bits::Bits(Type R, Type G, Type B) : r(R), g(G), b(B), a(BoundHigh) {}
		Bits::Bits(Type R, Type G, Type B, Type A) : r(R), g(G), b(B), a(A) {}
		Bits::Bits(Vector3f V) : r(V.x), g(V.y), b(V.z), a(BoundHigh) {}
		Bits::Bits(Vector4f V) : r(V.x), g(V.y), b(V.z), a(V.w) {}
		Bits::Bits(sRGB Other) { *this = Utility::ToBits(Other); }
		Bits::Bits(Linear Other) { *this = Utility::ToBits(Utility::TosRGB(Other)); }
		Bits::Bits(Hsv Other) { *this = Utility::ToBits(Utility::TosRGB(Other)); }

		Bits::operator Vector3f() const { return Utility::ToVector3f(*this); }
		Bits::operator Vector4f() const { return Utility::ToVector4f(*this); }

		Vector3f Bits::ToVector3f() const { return Utility::ToVector3f(*this); }
		Vector4f Bits::ToVector4f() const { return Utility::ToVector4f(*this); }
		float Bits::ToGrayscale() const { return Utility::ToGrayscale(*this); }
		sRGB Bits::TosRGB() const { return Utility::TosRGB(*this); }
		Linear Bits::ToLinear() const { return Utility::ToLinear(*this); }
		Hsv Bits::ToHsv() const { return Utility::ToHsv(*this); }

		bool Bits::operator==(Bits Other) const { return Utility::Equals(*this, Other); }
		bool Bits::operator!=(Bits Other) const { return !Utility::Equals(*this, Other); }
		Bits::Type& Bits::operator[](uint64 Index) { return Utility::Access(*this, Index); }
		const Bits::Type& Bits::operator[](uint64 Index) const { return Utility::Access(*this, Index); }

		Bits& Bits::operator+=(Type Other) { *this = Utility::Add(*this, Other); return *this; }
		Bits& Bits::operator-=(Type Other) { *this = Utility::Subtract(*this, Other); return *this; }
		Bits& Bits::operator*=(Type Other) { *this = Utility::Multiply(*this, Other); return *this; }
		Bits& Bits::operator/=(Type Other) { *this = Utility::Divide(*this, Other); return *this; }
		Bits& Bits::operator+=(Bits Other) { *this = Utility::Add(*this, Other);	return *this; }
		Bits& Bits::operator-=(Bits Other) { *this = Utility::Subtract(*this, Other); return *this; }
		Bits& Bits::operator*=(Bits Other) { *this = Utility::Multiply(*this, Other); return *this; }
		Bits& Bits::operator/=(Bits Other) { *this = Utility::Divide(*this, Other); return *this; }

		Bits::Type Bits::Min() const { return Utility::Min(*this); }
		Bits::Type Bits::Max() const { return Utility::Max(*this); }
		Bits Bits::Clamp(Type Min, Type Max) const { return Utility::Clamp(*this, Min, Max); }

		Bits::Type& Bits::GetX() { return r; }
		Bits::Type& Bits::GetY() { return g; }
		Bits::Type& Bits::GetZ() { return b; }
		Bits::Type& Bits::GetW() { return a; }
		const Bits::Type& Bits::GetX() const { return r; }
		const Bits::Type& Bits::GetY() const { return g; }
		const Bits::Type& Bits::GetZ() const { return b; }
		const Bits::Type& Bits::GetW() const { return a; }
		void Bits::SetX(Type X) { r = X; }
		void Bits::SetY(Type Y) { g = Y; }
		void Bits::SetZ(Type Z) { b = Z; }
		void Bits::SetW(Type W) { a = W; }

		Bits operator+(Bits Instance, Bits::Type Other) { return Utility::Add(Instance, Other); }
		Bits operator-(Bits Instance, Bits::Type Other) { return Utility::Subtract(Instance, Other); }
		Bits operator*(Bits Instance, Bits::Type Other) { return Utility::Multiply(Instance, Other); }
		Bits operator/(Bits Instance, Bits::Type Other) { return Utility::Divide(Instance, Other); }
		Bits operator+(Bits Instance, Bits Other) { return Utility::Add(Instance, Other); }
		Bits operator-(Bits Instance, Bits Other) { return Utility::Subtract(Instance, Other); }
		Bits operator*(Bits Instance, Bits Other) { return Utility::Multiply(Instance, Other); }
		Bits operator/(Bits Instance, Bits Other) { return Utility::Divide(Instance, Other); }

#pragma endregion

#pragma region Hsv

		Hsv::Hsv() : h(BoundLow), s(BoundLow), v(BoundLow), a(BoundLow) {}
		Hsv::Hsv(Type V) : h(V), s(V), v(V), a(BoundHigh) {}
		Hsv::Hsv(Type H, Type S, Type V) : h(H), s(S), v(V), a(BoundHigh) {}
		Hsv::Hsv(Type H, Type S, Type V, Type A) : h(H), s(S), v(V), a(A) {}
		Hsv::Hsv(Vector3f V) : h(V.x), s(V.y), v(V.z), a(BoundHigh) {}
		Hsv::Hsv(Vector4f V) : h(V.x), s(V.y), v(V.z), a(V.w) {}
		Hsv::Hsv(sRGB Other) { *this = Utility::ToHsv(Other); }
		Hsv::Hsv(Linear Other) { *this = Utility::ToHsv(Utility::TosRGB(Other)); }
		Hsv::Hsv(Bits Other) { *this = Utility::ToHsv(Utility::TosRGB(Other)); }

		Hsv::operator Vector3f() const { return Utility::ToVector3f(*this); }
		Hsv::operator Vector4f() const { return Utility::ToVector4f(*this); }

		Vector3f Hsv::ToVector3f() const { return Utility::ToVector3f(*this); }
		Vector4f Hsv::ToVector4f() const { return Utility::ToVector4f(*this); }
		float Hsv::ToGrayscale() const { return Utility::ToGrayscale(*this); }
		sRGB Hsv::TosRGB() const { return Utility::TosRGB(*this); }
		Linear Hsv::ToLinear() const { return Utility::ToLinear(*this); }
		Bits Hsv::ToBits() const { return Utility::ToBits(*this); }

		bool Hsv::operator==(Hsv Other) const { return Utility::Equals(*this, Other); }
		bool Hsv::operator!=(Hsv Other) const { return !Utility::Equals(*this, Other); }
		Hsv::Type& Hsv::operator[](uint64 Index) { return Utility::Access(*this, Index); }
		const Hsv::Type& Hsv::operator[](uint64 Index) const { return Utility::Access(*this, Index); }

		Hsv& Hsv::operator+=(Type Other) { *this = Utility::Add(*this, Other); return *this; }
		Hsv& Hsv::operator-=(Type Other) { *this = Utility::Subtract(*this, Other); return *this; }
		Hsv& Hsv::operator*=(Type Other) { *this = Utility::Multiply(*this, Other); return *this; }
		Hsv& Hsv::operator/=(Type Other) { *this = Utility::Divide(*this, Other); return *this; }
		Hsv& Hsv::operator+=(Hsv Other) { *this = Utility::Add(*this, Other);	return *this; }
		Hsv& Hsv::operator-=(Hsv Other) { *this = Utility::Subtract(*this, Other); return *this; }
		Hsv& Hsv::operator*=(Hsv Other) { *this = Utility::Multiply(*this, Other); return *this; }
		Hsv& Hsv::operator/=(Hsv Other) { *this = Utility::Divide(*this, Other); return *this; }

		Hsv::Type Hsv::Min() const { return Utility::Min(*this); }
		Hsv::Type Hsv::Max() const { return Utility::Max(*this); }
		Hsv Hsv::Clamp(Type Min, Type Max) const { return Utility::Clamp(*this, Min, Max); }

		Hsv::Type& Hsv::GetX() { return h; }
		Hsv::Type& Hsv::GetY() { return s; }
		Hsv::Type& Hsv::GetZ() { return v; }
		Hsv::Type& Hsv::GetW() { return a; }
		const Hsv::Type& Hsv::GetX() const { return h; }
		const Hsv::Type& Hsv::GetY() const { return s; }
		const Hsv::Type& Hsv::GetZ() const { return v; }
		const Hsv::Type& Hsv::GetW() const { return a; }
		void Hsv::SetX(Type X) { h = X; }
		void Hsv::SetY(Type Y) { s = Y; }
		void Hsv::SetZ(Type Z) { v = Z; }
		void Hsv::SetW(Type W) { a = W; }

		Hsv operator+(Hsv Instance, Hsv::Type Other) { return Utility::Add(Instance, Other); }
		Hsv operator-(Hsv Instance, Hsv::Type Other) { return Utility::Subtract(Instance, Other); }
		Hsv operator*(Hsv Instance, Hsv::Type Other) { return Utility::Multiply(Instance, Other); }
		Hsv operator/(Hsv Instance, Hsv::Type Other) { return Utility::Divide(Instance, Other); }
		Hsv operator+(Hsv Instance, Hsv Other) { return Utility::Add(Instance, Other); }
		Hsv operator-(Hsv Instance, Hsv Other) { return Utility::Subtract(Instance, Other); }
		Hsv operator*(Hsv Instance, Hsv Other) { return Utility::Multiply(Instance, Other); }
		Hsv operator/(Hsv Instance, Hsv Other) { return Utility::Divide(Instance, Other); }

#pragma endregion

#pragma region Utility

		float Utility::ToGrayscale(sRGB Instance)
		{
			return 0.2126f * Instance.r + 0.7152f * Instance.g + 0.0722f * Instance.b;
		}

		sRGB Utility::TosRGB(Linear Instance)
		{
			sRGB Result;
			Result.r = Instance.r <= 0.0031308f ? Instance.r * 12.92f : 1.055f * Math::Pow(Instance.r, 1.0f / 2.4f) - 0.055f;
			Result.g = Instance.g <= 0.0031308f ? Instance.g * 12.92f : 1.055f * Math::Pow(Instance.g, 1.0f / 2.4f) - 0.055f;
			Result.b = Instance.b <= 0.0031308f ? Instance.b * 12.92f : 1.055f * Math::Pow(Instance.b, 1.0f / 2.4f) - 0.055f;
			Result.a = Instance.a;
			return Result.Clamp();
		}

		sRGB Utility::TosRGB(Bits Instance)
		{
			sRGB Result;
			Result.r = Instance.r / (Bits::Type)Bits::BoundHigh;
			Result.g = Instance.g / (Bits::Type)Bits::BoundHigh;
			Result.b = Instance.b / (Bits::Type)Bits::BoundHigh;
			Result.a = Instance.a / (Bits::Type)Bits::BoundHigh;
			return Result.Clamp();
		}

		sRGB Utility::TosRGB(Hsv Instance)
		{
			sRGB Result = ToHue(Instance.h);
			Result = VectorUtility::Lerp(Vector4f::One, Result.ToVector4f(), Instance.s);
			Result = Result * Instance.v;
			Result.a = Instance.a;
			return Result;
		}

		Linear Utility::ToLinear(sRGB Instance)
		{
			Linear Result;
			Result.r = Instance.r <= 0.0031308f ? Instance.r / 12.92f : Math::Pow((Instance.r + 0.055f) / 1.055f, 2.4f);
			Result.g = Instance.g <= 0.0031308f ? Instance.g / 12.92f : Math::Pow((Instance.g + 0.055f) / 1.055f, 2.4f);
			Result.b = Instance.b <= 0.0031308f ? Instance.b / 12.92f : Math::Pow((Instance.b + 0.055f) / 1.055f, 2.4f);
			Result.a = Instance.a;
			return Result.Clamp();
		}

		Bits Utility::ToBits(sRGB Instance)
		{
			Bits Result;
			Result.r = (sRGB::Type)(Instance.r * Bits::BoundHigh);
			Result.g = (sRGB::Type)(Instance.g * Bits::BoundHigh);
			Result.b = (sRGB::Type)(Instance.b * Bits::BoundHigh);
			Result.a = (sRGB::Type)(Instance.a * Bits::BoundHigh);
			return Result.Clamp();
		}

		Hsv Utility::ToHsv(sRGB Instance)
		{
			Hsv Result;
			
			Bits::Type MaxComponent = Instance.Max();
			Bits::Type MinComponent = Instance.Min();
			Bits::Type Delta = MaxComponent - MinComponent;
			
			Bits::Type Hue = 0;
			if (Instance.r == MaxComponent)
			{
				Hue = 0 + (Instance.g - Instance.b) / Delta;
			}
			else if (Instance.g == MaxComponent)
			{
				Hue = 2 + (Instance.b - Instance.r) / Delta;
			}
			else if (Instance.b == MaxComponent)
			{
				Hue = 4 + (Instance.r - Instance.g) / Delta;
			}
			
			Result.h = Math::Frac(Hue / 6.0f);
			Result.s = Delta / MaxComponent;
			Result.v = MaxComponent;
			Result.a = Instance.a;
			
			return Result.Clamp();
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

#pragma endregion

	}
}

