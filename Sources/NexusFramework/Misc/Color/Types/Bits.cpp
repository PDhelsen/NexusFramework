#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Misc/Color/Types/Bits.h"

#include "NexusFramework/Misc/Color/Color.h"

namespace NxFr
{
	namespace Colors
	{
		Bits::Bits() : r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow) {}
		Bits::Bits(Type V) : r(V), g(V), b(V), a(BoundHigh) {}
		Bits::Bits(Type R, Type G, Type B) : r(R), g(G), b(B), a(BoundHigh) {}
		Bits::Bits(Type R, Type G, Type B, Type A) : r(R), g(G), b(B), a(A) {}
		Bits::Bits(Vector3f V) : r(V.x), g(V.y), b(V.z), a(BoundHigh) {}
		Bits::Bits(Vector4f V) : r(V.x), g(V.y), b(V.z), a(V.w) {}
		Bits::Bits(sRGB Other) { *this = ColorUtility::ToBits(Other); }
		Bits::Bits(Linear Other) { *this = ColorUtility::ToBits(ColorUtility::TosRGB(Other)); }
		Bits::Bits(Hsv Other) { *this = ColorUtility::ToBits(ColorUtility::TosRGB(Other)); }

		Bits::operator Vector3f() const { return ColorUtility::ToVector3f(*this); }
		Bits::operator Vector4f() const { return ColorUtility::ToVector4f(*this); }

		Vector3f Bits::ToVector3f() const { return ColorUtility::ToVector3f(*this); }
		Vector4f Bits::ToVector4f() const { return ColorUtility::ToVector4f(*this); }
		float Bits::ToGrayscale() const { return ColorUtility::ToGrayscale(*this); }
		sRGB Bits::TosRGB() const { return ColorUtility::TosRGB(*this); }
		Linear Bits::ToLinear() const { return ColorUtility::ToLinear(*this); }
		Hsv Bits::ToHsv() const { return ColorUtility::ToHsv(*this); }

		Bits::Type& Bits::operator[](uint64 Index) { return ColorUtility::Access(*this, Index); }
		const Bits::Type& Bits::operator[](uint64 Index) const { return ColorUtility::Access(*this, Index); }
		bool Bits::operator==(Bits Other) const { return ColorUtility::Equals(*this, Other); }
		bool Bits::operator!=(Bits Other) const { return !ColorUtility::Equals(*this, Other); }

		Bits& Bits::operator+=(Type Other) { *this = ColorUtility::Add(*this, Other); return *this; }
		Bits& Bits::operator-=(Type Other) { *this = ColorUtility::Subtract(*this, Other); return *this; }
		Bits& Bits::operator*=(Type Other) { *this = ColorUtility::Multiply(*this, Other); return *this; }
		Bits& Bits::operator/=(Type Other) { *this = ColorUtility::Divide(*this, Other); return *this; }
		Bits& Bits::operator+=(Bits Other) { *this = ColorUtility::Add(*this, Other);	return *this; }
		Bits& Bits::operator-=(Bits Other) { *this = ColorUtility::Subtract(*this, Other); return *this; }
		Bits& Bits::operator*=(Bits Other) { *this = ColorUtility::Multiply(*this, Other); return *this; }
		Bits& Bits::operator/=(Bits Other) { *this = ColorUtility::Divide(*this, Other); return *this; }

		Bits::Type Bits::Min() const { return ColorUtility::Min(*this); }
		Bits::Type Bits::Max() const { return ColorUtility::Max(*this); }
		Bits Bits::Clamp(Type Min, Type Max) const { return ColorUtility::Clamp(*this, Min, Max); }

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

		Bits operator+(Bits Instance, Bits::Type Other) { return ColorUtility::Add(Instance, Other); }
		Bits operator-(Bits Instance, Bits::Type Other) { return ColorUtility::Subtract(Instance, Other); }
		Bits operator*(Bits Instance, Bits::Type Other) { return ColorUtility::Multiply(Instance, Other); }
		Bits operator/(Bits Instance, Bits::Type Other) { return ColorUtility::Divide(Instance, Other); }
		Bits operator+(Bits Instance, Bits Other) { return ColorUtility::Add(Instance, Other); }
		Bits operator-(Bits Instance, Bits Other) { return ColorUtility::Subtract(Instance, Other); }
		Bits operator*(Bits Instance, Bits Other) { return ColorUtility::Multiply(Instance, Other); }
		Bits operator/(Bits Instance, Bits Other) { return ColorUtility::Divide(Instance, Other); }
	}
}
