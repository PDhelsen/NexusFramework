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
		struct NEXUS_FRAMEWORK_API Linear
		{
			using Type = float;

			static inline Type BoundLow = 0.0f;
			static inline Type BoundHigh = 1.0f;

			Linear();
			Linear(Type V);
			Linear(Type R, Type G, Type B);
			Linear(Type R, Type G, Type B, Type A);
			Linear(Vector3f V);
			Linear(Vector4f V);
			Linear(sRGB Other);
			Linear(Bits Other);
			Linear(Hsv Other);

			operator Vector3f() const;
			operator Vector4f() const;

			Vector3f ToVector3f() const;
			Vector4f ToVector4f() const;
			float ToGrayscale() const;
			sRGB TosRGB() const;
			Bits ToBits() const;
			Hsv ToHsv() const;

			Type& operator[](uint64 Index);
			const Type& operator[](uint64 Index) const;
			bool operator==(Linear Other) const;
			bool operator!=(Linear Other) const;

			Linear& operator+=(Type Other);
			Linear& operator-=(Type Other);
			Linear& operator*=(Type Other);
			Linear& operator/=(Type Other);
			Linear& operator+=(Linear Other);
			Linear& operator-=(Linear Other);
			Linear& operator*=(Linear Other);
			Linear& operator/=(Linear Other);

			Type Min() const;
			Type Max() const;
			Linear Clamp(Type Min = BoundLow, Type Max = BoundHigh) const;

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

		NEXUS_FRAMEWORK_API Linear operator+(Linear Instance, Linear::Type Other);
		NEXUS_FRAMEWORK_API Linear operator-(Linear Instance, Linear::Type Other);
		NEXUS_FRAMEWORK_API Linear operator*(Linear Instance, Linear::Type Other);
		NEXUS_FRAMEWORK_API Linear operator/(Linear Instance, Linear::Type Other);
		NEXUS_FRAMEWORK_API Linear operator+(Linear Instance, Linear Other);
		NEXUS_FRAMEWORK_API Linear operator-(Linear Instance, Linear Other);
		NEXUS_FRAMEWORK_API Linear operator*(Linear Instance, Linear Other);
		NEXUS_FRAMEWORK_API Linear operator/(Linear Instance, Linear Other);
	}
}
