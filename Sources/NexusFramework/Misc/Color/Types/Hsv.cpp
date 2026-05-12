#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Misc/Color/Types/Hsv.h"

#include "NexusFramework/Misc/Color/Color.h"

namespace NxFr
{
	namespace Colors
	{
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
	}
}
