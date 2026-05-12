#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Misc/Color/Color.h"

namespace NxFr
{
	namespace Colors
	{
#pragma region Const
		
		const Color Clear = Color(0.0f, 0.0f, 0.0f, 0.0f);
		const Color Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
		const Color Gray = Color(0.5f, 0.5f, 0.5f, 1.0f);
		const Color White = Color(1.0f, 1.0f, 1.0f, 1.0f);
		const Color Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
		const Color Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
		const Color Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
		const Color Yellow = Color(1.0f, 1.0f, 0.0f, 1.0f);
		const Color Cyan = Color(0.0f, 1.0f, 1.0f, 1.0f);
		const Color Magenta = Color(1.0f, 0.0f, 1.0f, 1.0f);
		const Color Orange = Color(1.0f, 0.5f, 0.0f, 1.0f);
		
#pragma endregion

#pragma region Utility

		float Utility::ToGrayscale(sRGB Instance)
		{
			return 0.2126f * Instance.r + 0.7152f * Instance.g + 0.0722f * Instance.b;
		}

		sRGB Utility::TosRGB(Linear Instance)
		{
			sRGB Result;
			Result.r = Instance.r <= 0.0031308f ? Instance.r * 12.92f : 1.055f * Math::Pow(Instance.r, 1.0f / 2.4f) - 0.055f;
			Result.g = Instance.g <= 0.0031308f ? Instance.g * 12.92f : 1.055f * Math::Pow(Instance.g, 1.0f / 2.4f) - 0.055f;
			Result.b = Instance.b <= 0.0031308f ? Instance.b * 12.92f : 1.055f * Math::Pow(Instance.b, 1.0f / 2.4f) - 0.055f;
			Result.a = Instance.a;
			return Result.Clamp();
		}

		sRGB Utility::TosRGB(Bits Instance)
		{
			sRGB Result;
			Result.r = Instance.r / (Bits::Type)Bits::BoundHigh;
			Result.g = Instance.g / (Bits::Type)Bits::BoundHigh;
			Result.b = Instance.b / (Bits::Type)Bits::BoundHigh;
			Result.a = Instance.a / (Bits::Type)Bits::BoundHigh;
			return Result.Clamp();
		}

		sRGB Utility::TosRGB(Hsv Instance)
		{
			sRGB Result = ToHue(Instance.h);
			Result = VectorUtility::Lerp(Vector4f::One, Result.ToVector4f(), Instance.s);
			Result = Result * Instance.v;
			Result.a = Instance.a;
			return Result;
		}

		Linear Utility::ToLinear(sRGB Instance)
		{
			Linear Result;
			Result.r = Instance.r <= 0.0031308f ? Instance.r / 12.92f : Math::Pow((Instance.r + 0.055f) / 1.055f, 2.4f);
			Result.g = Instance.g <= 0.0031308f ? Instance.g / 12.92f : Math::Pow((Instance.g + 0.055f) / 1.055f, 2.4f);
			Result.b = Instance.b <= 0.0031308f ? Instance.b / 12.92f : Math::Pow((Instance.b + 0.055f) / 1.055f, 2.4f);
			Result.a = Instance.a;
			return Result.Clamp();
		}

		Bits Utility::ToBits(sRGB Instance)
		{
			Bits Result;
			Result.r = (sRGB::Type)(Instance.r * Bits::BoundHigh);
			Result.g = (sRGB::Type)(Instance.g * Bits::BoundHigh);
			Result.b = (sRGB::Type)(Instance.b * Bits::BoundHigh);
			Result.a = (sRGB::Type)(Instance.a * Bits::BoundHigh);
			return Result.Clamp();
		}

		Hsv Utility::ToHsv(sRGB Instance)
		{
			Hsv Result;
			
			Bits::Type MaxComponent = Instance.Max();
			Bits::Type MinComponent = Instance.Min();
			Bits::Type Delta = MaxComponent - MinComponent;
			
			Bits::Type Hue = 0;
			if (Instance.r == MaxComponent)
			{
				Hue = 0 + (Instance.g - Instance.b) / Delta;
			}
			else if (Instance.g == MaxComponent)
			{
				Hue = 2 + (Instance.b - Instance.r) / Delta;
			}
			else if (Instance.b == MaxComponent)
			{
				Hue = 4 + (Instance.r - Instance.g) / Delta;
			}
			
			Result.h = Math::Frac(Hue / 6.0f);
			Result.s = Delta / MaxComponent;
			Result.v = MaxComponent;
			Result.a = Instance.a;
			
			return Result.Clamp();
		}

		sRGB Utility::ToHue(float Hue)
		{
			Hue = Math::Frac(Hue);
			
			sRGB Result;
			Result.r = Math::Abs(Hue * 6.0f - 3.0f) - 1.0f;
			Result.g = 2.0f - Math::Abs(Hue * 6.0f - 2.0f);
			Result.b = 2.0f - Math::Abs(Hue * 6.0f - 4.0f);
			return Result.Clamp();
		}

#pragma endregion

	}
}

