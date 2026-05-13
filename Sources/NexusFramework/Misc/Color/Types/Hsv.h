#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Decimal.h"

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
		struct NEXUS_FRAMEWORK_API Hsv
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			Hsv();
			Hsv(Type V);
			Hsv(Type H, Type S, Type V);
			Hsv(Type H, Type S, Type V, Type A);
			Hsv(Vector3f V);
			Hsv(Vector4f V);
			Hsv(sRGB Other);
			Hsv(Linear Other);
			Hsv(Bits Other);

			operator Vector3f() const;
			operator Vector4f() const;

			Vector3f ToVector3f() const;
			Vector4f ToVector4f() const;
			float ToGrayscale() const;
			sRGB TosRGB() const;
			Linear ToLinear() const;
			Bits ToBits() const;

			Type& operator[](uint64 Index);
			const Type& operator[](uint64 Index) const;
			bool operator==(Hsv Other) const;
			bool operator!=(Hsv Other) const;

			Hsv& operator+=(Type Other);
			Hsv& operator-=(Type Other);
			Hsv& operator*=(Type Other);
			Hsv& operator/=(Type Other);
			Hsv& operator+=(Hsv Other);
			Hsv& operator-=(Hsv Other);
			Hsv& operator*=(Hsv Other);
			Hsv& operator/=(Hsv Other);

			Type Min() const;
			Type Max() const;
			Hsv Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

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

			Type h, s, v, a;
		};

		NEXUS_FRAMEWORK_API Hsv operator+(Hsv Instance, Hsv::Type Other);
		NEXUS_FRAMEWORK_API Hsv operator-(Hsv Instance, Hsv::Type Other);
		NEXUS_FRAMEWORK_API Hsv operator*(Hsv Instance, Hsv::Type Other);
		NEXUS_FRAMEWORK_API Hsv operator/(Hsv Instance, Hsv::Type Other);
		NEXUS_FRAMEWORK_API Hsv operator+(Hsv Instance, Hsv Other);
		NEXUS_FRAMEWORK_API Hsv operator-(Hsv Instance, Hsv Other);
		NEXUS_FRAMEWORK_API Hsv operator*(Hsv Instance, Hsv Other);
		NEXUS_FRAMEWORK_API Hsv operator/(Hsv Instance, Hsv Other);
	}
}
