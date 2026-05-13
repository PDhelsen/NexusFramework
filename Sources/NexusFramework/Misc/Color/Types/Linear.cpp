#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Misc/Color/Types/Linear.h"

#include "NexusFramework/Misc/Color/Color.h"
#include "NexusFramework/Math/Vector.h"

namespace NxFr
{
	namespace Colors
	{
		Linear::Linear() : r(BoundLow), g(BoundLow), b(BoundLow), a(BoundLow) {}
		Linear::Linear(Type V) : r(V), g(V), b(V), a(BoundHigh) {}
		Linear::Linear(Type R, Type G, Type B) : r(R), g(G), b(B), a(BoundHigh) {}
		Linear::Linear(Type R, Type G, Type B, Type A) : r(R), g(G), b(B), a(A) {}
		Linear::Linear(Vector3f V) : r(V.x), g(V.y), b(V.z), a(BoundHigh) {}
		Linear::Linear(Vector4f V) : r(V.x), g(V.y), b(V.z), a(V.w) {}
		Linear::Linear(sRGB Other) { *this = ColorUtility::ToLinear(Other); }
		Linear::Linear(Bits Other) { *this = ColorUtility::ToLinear(ColorUtility::TosRGB(Other)); }
		Linear::Linear(Hsv Other) { *this = ColorUtility::ToLinear(ColorUtility::TosRGB(Other)); }

		Linear::operator Vector3f() const { return ColorUtility::ToVector3f(*this); }
		Linear::operator Vector4f() const { return ColorUtility::ToVector4f(*this); }

		Vector3f Linear::ToVector3f() const { return ColorUtility::ToVector3f(*this); }
		Vector4f Linear::ToVector4f() const { return ColorUtility::ToVector4f(*this); }
		float Linear::ToGrayscale() const { return ColorUtility::ToGrayscale(*this); }
		sRGB Linear::TosRGB() const { return ColorUtility::TosRGB(*this); }
		Bits Linear::ToBits() const { return ColorUtility::ToBits(*this); }
		Hsv Linear::ToHsv() const { return ColorUtility::ToHsv(*this); }

		Linear::Type& Linear::operator[](uint64 Index) { return ColorUtility::Access(*this, Index); }
		const Linear::Type& Linear::operator[](uint64 Index) const { return ColorUtility::Access(*this, Index); }
		bool Linear::operator==(Linear Other) const { return ColorUtility::Equals(*this, Other); }
		bool Linear::operator!=(Linear Other) const { return !ColorUtility::Equals(*this, Other); }

		Linear& Linear::operator+=(Type Other) { *this = ColorUtility::Add(*this, Other); return *this; }
		Linear& Linear::operator-=(Type Other) { *this = ColorUtility::Subtract(*this, Other); return *this; }
		Linear& Linear::operator*=(Type Other) { *this = ColorUtility::Multiply(*this, Other); return *this; }
		Linear& Linear::operator/=(Type Other) { *this = ColorUtility::Divide(*this, Other); return *this; }
		Linear& Linear::operator+=(Linear Other) { *this = ColorUtility::Add(*this, Other);	return *this; }
		Linear& Linear::operator-=(Linear Other) { *this = ColorUtility::Subtract(*this, Other); return *this; }
		Linear& Linear::operator*=(Linear Other) { *this = ColorUtility::Multiply(*this, Other); return *this; }
		Linear& Linear::operator/=(Linear Other) { *this = ColorUtility::Divide(*this, Other); return *this; }

		Linear::Type Linear::Min() const { return ColorUtility::Min(*this); }
		Linear::Type Linear::Max() const { return ColorUtility::Max(*this); }
		Linear Linear::Clamp(Type Min, Type Max) const { return ColorUtility::Clamp(*this, Min, Max); }

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

		Linear operator+(Linear Instance, Linear::Type Other) { return ColorUtility::Add(Instance, Other); }
		Linear operator-(Linear Instance, Linear::Type Other) { return ColorUtility::Subtract(Instance, Other); }
		Linear operator*(Linear Instance, Linear::Type Other) { return ColorUtility::Multiply(Instance, Other); }
		Linear operator/(Linear Instance, Linear::Type Other) { return ColorUtility::Divide(Instance, Other); }
		Linear operator+(Linear Instance, Linear Other) { return ColorUtility::Add(Instance, Other); }
		Linear operator-(Linear Instance, Linear Other) { return ColorUtility::Subtract(Instance, Other); }
		Linear operator*(Linear Instance, Linear Other) { return ColorUtility::Multiply(Instance, Other); }
		Linear operator/(Linear Instance, Linear Other) { return ColorUtility::Divide(Instance, Other); }
	}
}
