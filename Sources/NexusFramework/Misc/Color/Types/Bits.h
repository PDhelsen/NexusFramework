#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"

namespace NxFr
{
	template<uint8 D, typename T>
	struct Vector;
	using Vector3f = Vector<3, float>;
	using Vector4f = Vector<4, float>;
	namespace Colors
	{
		struct sRGB;
		struct Linear;
		struct Bits;
		struct Hsv;
	}

	namespace Colors
	{
		struct NX_FRAMEWORK_API Bits
		{
			using Type = uint8;

			static inline Type BoundLow = 0;
			static inline Type BoundHigh = 255;

			Bits();
			Bits(Type V);
			Bits(Type R, Type G, Type B);
			Bits(Type R, Type G, Type B, Type A);
			Bits(Vector3f V);
			Bits(Vector4f V);
			Bits(sRGB Other);
			Bits(Linear Other);
			Bits(Hsv Other);

			operator Vector3f() const;
			operator Vector4f() const;

			Vector3f ToVector3f() const;
			Vector4f ToVector4f() const;
			float ToGrayscale() const;
			sRGB TosRGB() const;
			Linear ToLinear() const;
			Hsv ToHsv() const;

			Type& operator[](uint64 Index);
			const Type& operator[](uint64 Index) const;
			bool operator==(Bits Other) const;
			bool operator!=(Bits Other) const;

			Bits& operator+=(Type Other);
			Bits& operator-=(Type Other);
			Bits& operator*=(Type Other);
			Bits& operator/=(Type Other);
			Bits& operator+=(Bits Other);
			Bits& operator-=(Bits Other);
			Bits& operator*=(Bits Other);
			Bits& operator/=(Bits Other);

			Type Min() const;
			Type Max() const;
			Bits Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

			Type& GetX();
			Type& GetY();
			Type& GetZ();
			Type& GetW();
			const Type& GetX() const;
			const Type& GetY() const;
			const Type& GetZ() const;
			const Type& GetW() const;
			void SetX(Type X);
			void SetY(Type Y);
			void SetZ(Type Z);
			void SetW(Type W);

			Type r, g, b, a;
		};

		NX_FRAMEWORK_API Bits operator+(Bits Instance, Bits::Type Other);
		NX_FRAMEWORK_API Bits operator-(Bits Instance, Bits::Type Other);
		NX_FRAMEWORK_API Bits operator*(Bits Instance, Bits::Type Other);
		NX_FRAMEWORK_API Bits operator/(Bits Instance, Bits::Type Other);
		NX_FRAMEWORK_API Bits operator+(Bits Instance, Bits Other);
		NX_FRAMEWORK_API Bits operator-(Bits Instance, Bits Other);
		NX_FRAMEWORK_API Bits operator*(Bits Instance, Bits Other);
		NX_FRAMEWORK_API Bits operator/(Bits Instance, Bits Other);
	}
}
