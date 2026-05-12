#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Misc/Color/Types/sRGB.h"
#include "NexusFramework/Misc/Color/Types/Linear.h"
#include "NexusFramework/Misc/Color/Types/Bits.h"
#include "NexusFramework/Misc/Color/Types/Hsv.h"

namespace NxFr
{
	using Color = Colors::sRGB;

	namespace Colors
	{
		NEXUS_FRAMEWORK_API extern const Color Clear;
		NEXUS_FRAMEWORK_API extern const Color Black;
		NEXUS_FRAMEWORK_API extern const Color Gray;
		NEXUS_FRAMEWORK_API extern const Color White;
		NEXUS_FRAMEWORK_API extern const Color Red;
		NEXUS_FRAMEWORK_API extern const Color Green;
		NEXUS_FRAMEWORK_API extern const Color Blue;
		NEXUS_FRAMEWORK_API extern const Color Yellow;
		NEXUS_FRAMEWORK_API extern const Color Cyan;
		NEXUS_FRAMEWORK_API extern const Color Magenta;
		NEXUS_FRAMEWORK_API extern const Color Orange;
	}

	namespace ColorUtility
	{
		NEXUS_FRAMEWORK_API float ToGrayscale(Colors::sRGB Instance);
		NEXUS_FRAMEWORK_API Colors::sRGB TosRGB(Colors::Linear Instance);
		NEXUS_FRAMEWORK_API Colors::sRGB TosRGB(Colors::Bits Instance);
		NEXUS_FRAMEWORK_API Colors::sRGB TosRGB(Colors::Hsv Instance);
		NEXUS_FRAMEWORK_API Colors::Linear ToLinear(Colors::sRGB Instance);
		NEXUS_FRAMEWORK_API Colors::Bits ToBits(Colors::sRGB Instance);
		NEXUS_FRAMEWORK_API Colors::Hsv ToHsv(Colors::sRGB Instance);
		NEXUS_FRAMEWORK_API Colors::sRGB ToHue(float Hue);

		template<typename T>
		Vector3f ToVector3f(T Instance)
		{
			return Vector3f(Instance.GetX(), Instance.GetY(), Instance.GetZ());
		}

		template<typename T>
		Vector4f ToVector4f(T Instance)
		{
			return Vector4f(Instance.GetX(), Instance.GetY(), Instance.GetZ(), Instance.GetW());
		}

		template<typename T>
		typename T::Type& Access(T& Instance, uint64 Index)
		{
			return Index == 0 ? Instance.GetX() : Index == 1 ? Instance.GetY() : Index == 2 ? Instance.GetZ() : Index == 3 ? Instance.GetW() : T::BoundLow;
		}

		template<typename T>
		const typename T::Type& Access(const T& Instance, uint64 Index)
		{
			return Index == 0 ? Instance.GetX() : Index == 1 ? Instance.GetY() : Index == 2 ? Instance.GetZ() : Index == 3 ? Instance.GetW() : T::BoundLow;
		}

		template<typename T>
		bool Equals(T Instance, T Other)
		{
			return Math::Equals(Instance.GetX(), Other.GetX()) && Math::Equals(Instance.GetY(), Other.GetY()) && Math::Equals(Instance.GetZ(), Other.GetZ()) && Math::Equals(Instance.GetW(), Other.GetW());
		}

		template<typename T>
		typename T::Type Min(T Instance)
		{
			return Math::Min(Instance.GetX(), Math::Min(Instance.GetY(), Instance.GetZ()));
		}
			
		template<typename T>
		T Min(T Instance, T Other)
		{
			return T(Math::Min(Instance.GetX(), Other.GetX()), Math::Min(Instance.GetY(), Other.GetY()), Math::Min(Instance.GetZ(), Other.GetZ()), Math::Min(Instance.GetW(), Other.GetW()));
		}

		template<typename T>
		typename T::Type Max(T Instance)
		{
			return Math::Max(Instance.GetX(), Math::Max(Instance.GetY(), Instance.GetZ()));
		}

		template<typename T>
		T Max(T Instance, T Other)
		{
			return T(Math::Max(Instance.GetX(), Other.GetX()), Math::Max(Instance.GetY(), Other.GetY()), Math::Max(Instance.GetZ(), Other.GetZ()), Math::Max(Instance.GetW(), Other.GetW()));
		}

		template<typename T>
		T Clamp(T Instance, typename T::Type Min, typename T::Type Max)
		{
			return T(Math::Clamp(Instance.GetX(), Min, Max), Math::Clamp(Instance.GetY(), Min, Max), Math::Clamp(Instance.GetZ(), Min, Max), Math::Clamp(Instance.GetW(), Min, Max));
		}

		template<typename T>
		T Lerp(T A, T B, float Weight)
		{
			return VectorUtility::Lerp(A.ToVector4f(), B.ToVector4f(), Weight);
		}

		template<typename T>
		T Add(T Instance, typename T::Type Other)
		{
			return T(Instance.GetX() + Other, Instance.GetY() + Other, Instance.GetZ() + Other, Instance.GetW() + Other);
		}

		template<typename T>
		T Add(T Instance, T Other)
		{
			return T(Instance.GetX() + Other.GetX(), Instance.GetY() + Other.GetY(), Instance.GetZ() + Other.GetZ(), Instance.GetW() + Other.GetW());
		}

		template<typename T>
		T Subtract(T Instance, typename T::Type Other)
		{
			return T(Instance.GetX() - Other, Instance.GetY() - Other, Instance.GetZ() - Other, Instance.GetW() - Other);
		}

		template<typename T>
		T Subtract(T Instance, T Other)
		{
			return T(Instance.GetX() - Other.GetX(), Instance.GetY() - Other.GetY(), Instance.GetZ() - Other.GetZ(), Instance.GetW() - Other.GetW());
		}

		template<typename T>
		T Multiply(T Instance, typename T::Type Other)
		{
			return T(Instance.GetX() * Other, Instance.GetY() * Other, Instance.GetZ() * Other, Instance.GetW() * Other);
		}

		template<typename T>
		T Multiply(T Instance, T Other)
		{
			return T(Instance.GetX() * Other.GetX(), Instance.GetY() * Other.GetY(), Instance.GetZ() * Other.GetZ(), Instance.GetW() * Other.GetW());
		}

		template<typename T>
		T Divide(T Instance, typename T::Type Other)
		{
			return T(Instance.GetX() / Other, Instance.GetY() / Other, Instance.GetZ() / Other, Instance.GetW() / Other);
		}

		template<typename T>
		T Divide(T Instance, T Other)
		{
			return T(Instance.GetX() / Other.GetX(), Instance.GetY() / Other.GetY(), Instance.GetZ() / Other.GetZ(), Instance.GetW() / Other.GetW());
		}
	}
}
