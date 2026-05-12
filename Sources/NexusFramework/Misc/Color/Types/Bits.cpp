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
	}
}
