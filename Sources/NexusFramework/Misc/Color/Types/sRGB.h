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
		struct NEXUS_FRAMEWORK_API sRGB
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			sRGB();
			sRGB(Type V);
			sRGB(Type R, Type G, Type B);
			sRGB(Type R, Type G, Type B, Type A);
			sRGB(Vector3f V);
			sRGB(Vector4f V);
			sRGB(Linear Other);
			sRGB(Bits Other);
			sRGB(Hsv Other);

			operator Vector3f() const;
			operator Vector4f() const;

			Vector3f ToVector3f() const;
			Vector4f ToVector4f() const;
			float ToGrayscale() const;
			Linear ToLinear() const;
			Bits ToBits() const;
			Hsv ToHsv() const;

			Type& operator[](uint64 Index);
			const Type& operator[](uint64 Index) const;
			bool operator==(sRGB Other) const;
			bool operator!=(sRGB Other) const;

			sRGB& operator+=(Type Other);
			sRGB& operator-=(Type Other);
			sRGB& operator*=(Type Other);
			sRGB& operator/=(Type Other);
			sRGB& operator+=(sRGB Other);
			sRGB& operator-=(sRGB Other);
			sRGB& operator*=(sRGB Other);
			sRGB& operator/=(sRGB Other);

			Type Min() const;
			Type Max() const;
			sRGB Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

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

		NEXUS_FRAMEWORK_API sRGB operator+(sRGB Instance, sRGB::Type Other);
		NEXUS_FRAMEWORK_API sRGB operator-(sRGB Instance, sRGB::Type Other);
		NEXUS_FRAMEWORK_API sRGB operator*(sRGB Instance, sRGB::Type Other);
		NEXUS_FRAMEWORK_API sRGB operator/(sRGB Instance, sRGB::Type Other);
		NEXUS_FRAMEWORK_API sRGB operator+(sRGB Instance, sRGB Other);
		NEXUS_FRAMEWORK_API sRGB operator-(sRGB Instance, sRGB Other);
		NEXUS_FRAMEWORK_API sRGB operator*(sRGB Instance, sRGB Other);
		NEXUS_FRAMEWORK_API sRGB operator/(sRGB Instance, sRGB Other);
	}
}
