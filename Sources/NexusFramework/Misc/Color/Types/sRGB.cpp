#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Misc/Color/Types/sRGB.h"

#include "NexusFramework/Misc/Color/Color.h"

namespace NxFr
{
	namespace Colors
	{
		sRGB::sRGB() : r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow) {}
		sRGB::sRGB(Type V) : r(V), g(V), b(V), a(BoundHigh) {}
		sRGB::sRGB(Type R, Type G, Type B) : r(R), g(G), b(B), a(BoundHigh) {}
		sRGB::sRGB(Type R, Type G, Type B, Type A) : r(R), g(G), b(B), a(A) {}
		sRGB::sRGB(Vector3f V) : r(V.x), g(V.y), b(V.z), a(BoundHigh) {}
		sRGB::sRGB(Vector4f V) : r(V.x), g(V.y), b(V.z), a(V.w) {}
		sRGB::sRGB(Linear Other) { *this = ColorUtility::TosRGB(Other); }
		sRGB::sRGB(Bits Other) { *this = ColorUtility::TosRGB(Other); }
		sRGB::sRGB(Hsv Other) { *this = ColorUtility::TosRGB(Other); }

		sRGB::operator Vector3f() const { return ColorUtility::ToVector3f(*this); }
		sRGB::operator Vector4f() const { return ColorUtility::ToVector4f(*this); }

		Vector3f sRGB::ToVector3f() const { return ColorUtility::ToVector3f(*this); }
		Vector4f sRGB::ToVector4f() const { return ColorUtility::ToVector4f(*this); }
		float sRGB::ToGrayscale() const { return ColorUtility::ToGrayscale(*this); }
		Linear sRGB::ToLinear() const { return ColorUtility::ToLinear(*this); }
		Bits sRGB::ToBits() const { return ColorUtility::ToBits(*this); }
		Hsv sRGB::ToHsv() const { return ColorUtility::ToHsv(*this); }

		sRGB::Type& sRGB::operator[](uint64 Index) { return ColorUtility::Access(*this, Index); }
		const sRGB::Type& sRGB::operator[](uint64 Index) const { return ColorUtility::Access(*this, Index); }
		bool sRGB::operator==(sRGB Other) const { return ColorUtility::Equals(*this, Other); }
		bool sRGB::operator!=(sRGB Other) const { return !ColorUtility::Equals(*this, Other); }

		sRGB& sRGB::operator+=(Type Other) { *this = ColorUtility::Add(*this, Other); return *this; }
		sRGB& sRGB::operator-=(Type Other) { *this = ColorUtility::Subtract(*this, Other); return *this; }
		sRGB& sRGB::operator*=(Type Other) { *this = ColorUtility::Multiply(*this, Other); return *this; }
		sRGB& sRGB::operator/=(Type Other) { *this = ColorUtility::Divide(*this, Other); return *this; }
		sRGB& sRGB::operator+=(sRGB Other) { *this = ColorUtility::Add(*this, Other);	return *this; }
		sRGB& sRGB::operator-=(sRGB Other) { *this = ColorUtility::Subtract(*this, Other); return *this; }
		sRGB& sRGB::operator*=(sRGB Other) { *this = ColorUtility::Multiply(*this, Other); return *this; }
		sRGB& sRGB::operator/=(sRGB Other) { *this = ColorUtility::Divide(*this, Other); return *this; }

		sRGB::Type sRGB::Min() const { return ColorUtility::Min(*this); }
		sRGB::Type sRGB::Max() const { return ColorUtility::Max(*this); }
		sRGB sRGB::Clamp(Type Min, Type Max) const { return ColorUtility::Clamp(*this, Min, Max); }

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

		sRGB operator+(sRGB Instance, sRGB::Type Other) { return ColorUtility::Add(Instance, Other); }
		sRGB operator-(sRGB Instance, sRGB::Type Other) { return ColorUtility::Subtract(Instance, Other); }
		sRGB operator*(sRGB Instance, sRGB::Type Other) { return ColorUtility::Multiply(Instance, Other); }
		sRGB operator/(sRGB Instance, sRGB::Type Other) { return ColorUtility::Divide(Instance, Other); }
		sRGB operator+(sRGB Instance, sRGB Other) { return ColorUtility::Add(Instance, Other); }
		sRGB operator-(sRGB Instance, sRGB Other) { return ColorUtility::Subtract(Instance, Other); }
		sRGB operator*(sRGB Instance, sRGB Other) { return ColorUtility::Multiply(Instance, Other); }
		sRGB operator/(sRGB Instance, sRGB Other) { return ColorUtility::Divide(Instance, Other); }
	}
}
