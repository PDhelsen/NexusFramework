#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Misc/Color/Types/Linear.h"

#include "NexusFramework/Misc/Color/Color.h"

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
	}
}
