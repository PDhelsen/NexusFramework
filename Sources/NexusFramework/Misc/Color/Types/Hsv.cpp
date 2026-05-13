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
		Hsv::Hsv(sRGB Other) { *this = ColorUtility::ToHsv(Other); }
		Hsv::Hsv(Linear Other) { *this = ColorUtility::ToHsv(ColorUtility::TosRGB(Other)); }
		Hsv::Hsv(Bits Other) { *this = ColorUtility::ToHsv(ColorUtility::TosRGB(Other)); }

		Hsv::operator Vector3f() const { return ColorUtility::ToVector3f(*this); }
		Hsv::operator Vector4f() const { return ColorUtility::ToVector4f(*this); }

		Vector3f Hsv::ToVector3f() const { return ColorUtility::ToVector3f(*this); }
		Vector4f Hsv::ToVector4f() const { return ColorUtility::ToVector4f(*this); }
		float Hsv::ToGrayscale() const { return ColorUtility::ToGrayscale(*this); }
		sRGB Hsv::TosRGB() const { return ColorUtility::TosRGB(*this); }
		Linear Hsv::ToLinear() const { return ColorUtility::ToLinear(*this); }
		Bits Hsv::ToBits() const { return ColorUtility::ToBits(*this); }

		Hsv::Type& Hsv::operator[](uint64 Index) { return ColorUtility::Access(*this, Index); }
		const Hsv::Type& Hsv::operator[](uint64 Index) const { return ColorUtility::Access(*this, Index); }
		bool Hsv::operator==(Hsv Other) const { return ColorUtility::Equals(*this, Other); }
		bool Hsv::operator!=(Hsv Other) const { return !ColorUtility::Equals(*this, Other); }

		Hsv& Hsv::operator+=(Type Other) { *this = ColorUtility::Add(*this, Other); return *this; }
		Hsv& Hsv::operator-=(Type Other) { *this = ColorUtility::Subtract(*this, Other); return *this; }
		Hsv& Hsv::operator*=(Type Other) { *this = ColorUtility::Multiply(*this, Other); return *this; }
		Hsv& Hsv::operator/=(Type Other) { *this = ColorUtility::Divide(*this, Other); return *this; }
		Hsv& Hsv::operator+=(Hsv Other) { *this = ColorUtility::Add(*this, Other);	return *this; }
		Hsv& Hsv::operator-=(Hsv Other) { *this = ColorUtility::Subtract(*this, Other); return *this; }
		Hsv& Hsv::operator*=(Hsv Other) { *this = ColorUtility::Multiply(*this, Other); return *this; }
		Hsv& Hsv::operator/=(Hsv Other) { *this = ColorUtility::Divide(*this, Other); return *this; }

		Hsv::Type Hsv::Min() const { return ColorUtility::Min(*this); }
		Hsv::Type Hsv::Max() const { return ColorUtility::Max(*this); }
		Hsv Hsv::Clamp(Type Min, Type Max) const { return ColorUtility::Clamp(*this, Min, Max); }

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

		Hsv operator+(Hsv Instance, Hsv::Type Other) { return ColorUtility::Add(Instance, Other); }
		Hsv operator-(Hsv Instance, Hsv::Type Other) { return ColorUtility::Subtract(Instance, Other); }
		Hsv operator*(Hsv Instance, Hsv::Type Other) { return ColorUtility::Multiply(Instance, Other); }
		Hsv operator/(Hsv Instance, Hsv::Type Other) { return ColorUtility::Divide(Instance, Other); }
		Hsv operator+(Hsv Instance, Hsv Other) { return ColorUtility::Add(Instance, Other); }
		Hsv operator-(Hsv Instance, Hsv Other) { return ColorUtility::Subtract(Instance, Other); }
		Hsv operator*(Hsv Instance, Hsv Other) { return ColorUtility::Multiply(Instance, Other); }
		Hsv operator/(Hsv Instance, Hsv Other) { return ColorUtility::Divide(Instance, Other); }
	}
}
