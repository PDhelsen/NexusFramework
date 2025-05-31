#pragma once

#include "NexusFramework/External/Intrinsics.h"
#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"

namespace NxFr
{
	namespace Math
	{
		constexpr float Pi = 3.14159f;
		constexpr float Degree = 180.0f / Pi;
		constexpr float Radians = Pi / 180.0f;

		template<typename T>
		T Abs(T X)
		{
			return abs(X);
		}

		template<typename T>
		T Sign(T X)
		{
			return (T)(X == (T)0 ? (T)0 : X > (T)0 ? (T)1 : -(T)1);
		}

		template<typename T>
		T CopySign(T V, T X)
		{
			return X >= (T)0 ? Abs(V) : -Abs(V);
		}

		template<typename T>
		T Ceil(T X)
		{
			return ceil(X);
		}

		template<typename T>
		int64 CeilToInt(T X)
		{
			return (int64)ceil(X);
		}

		template<typename T>
		T Floor(T X)
		{
			return floor(X);
		}

		template<typename T>
		int64 FloorToInt(T X)
		{
			return (int64)floor(X);
		}

		template<typename T>
		float Frac(float X)
		{
			return X - Floor(X);
		}

		template<typename T>
		float Round(float X)
		{
			return round(X);
		}

		template<typename T>
		int64 RoundToInt(T X)
		{
			return (int64)round(X);
		}

		template<typename T>
		T Modf(T X, T& Y)
		{
			return modf(X, &Y);
		}

		template<typename T>
		T FMod(T X, T Y)
		{
			return fmod(X, Y);
		}

		template<typename T>
		T Modulo(T X, T Y)
		{
			NEXUS_ASSERT(Y != (T)0, Default, "Modulo with Mode == 0 is undefined");
			T Remainder = X % Y;
			return Remainder < (T)0 ? Remainder + Y : Remainder;
		}

		template<typename T>
		T IsMultiple(T X, T Y)
		{
			return Y != (T)0 && X % Y == (T)0;
		}

		template<typename T>
		T Min(T A, T B)
		{
			return A < B ? A : B;
		}

		template<typename T>
		T Max(T A, T B)
		{
			return A > B ? A : B;
		}

		template<typename T>
		T Clamp(T X, T A = 0, T B = 0)
		{
			return Min(Max(X, A), B);
		}

		template<typename T>
		T Lerp(T A, T B, T V)
		{
			return ((T)1 - V) * A + V * B;
		}

		template<typename T>
		T InverseLerp(T A, T B, T V)
		{
			return (V - A) / (B - A);
		}

		template<typename T>
		T Remap(T X, T InMin, T InMax, T OutMin, T OutMax)
		{
			return OutMin + ((X - InMin) * (OutMax - OutMin)) / (InMax - InMin);
		}

		template<typename T>
		bool Equals(T X, T Y, float Epsilon = Decimal::EpsilonF)
		{
			return Abs(X - Y) <= Max(Max(X, Y), (T)1) * Epsilon;
		}

		template<typename T>
		T Acos(T X)
		{
			return acos(X);
		}

		template<typename T>
		T Asin(T X)
		{
			return asin(X);
		}

		template<typename T>
		T Atan(T X)
		{
			return atan(X);
		}

		template<typename T>
		T Atan(T X, T Y)
		{
			return atan2(X, Y);
		}

		template<typename T>
		T Cos(T X)
		{
			return cos(X);
		}

		template<typename T>
		T Cosh(T X)
		{
			return cosh(X);
		}

		template<typename T>
		T Sin(T X)
		{
			return sin(X);
		}

		template<typename T>
		T Sinh(T X)
		{
			return sinh(X);
		}

		template<typename T>
		T Tan(T X)
		{
			return tan(X);
		}

		template<typename T>
		T Tanh(T X)
		{
			return tanh(X);
		}

		template<typename T>
		T Exp(T X)
		{
			return exp(X);
		}

		template<typename T>
		T Log(T X)
		{
			return log(X);
		}

		template<typename T>
		T Log10(T X)
		{
			return log10(X);
		}

		template<typename T>
		T Pow(T X, T Y)
		{
			return pow(X, Y);
		}

		template<typename T>
		T Sqrt(T X)
		{
			return sqrt(X);
		}

		template<typename T>
		T Square(T X)
		{
			return X * X;
		}

		template<typename T>
		bool IsPowerOfTwo(T X)
		{
			return (X & (X - 1)) == 0;
		}

		inline uint64 NextPowerOfTwo(uint64 X)
		{
			if (X == 0)
			{
				return 1;
			}

			X--;
			X |= X >> 1;
			X |= X >> 2;
			X |= X >> 4;
			X |= X >> 8;
			X |= X >> 16;
			X |= X >> 32;

			return (X + 1);
		}

		template<typename T>
		T LogTwoPowerOfTwo(T X)
		{
			NEXUS_ASSERT(IsPowerOfTwo(X), Default, "X is not a power of 2");
			NEXUS_BITSCANFORWARD(X, Index, Result)
			return Result > 0 ? (T)Index : -1;
		}

		template<typename T>
		bool IsPrime(T X)
		{
			if (X <= 1) return false;
			for (T N = 2; N <= sqrt(X); ++N)
			{
				if (X % N == 0) return false;
			}
			return true;
		}
		
		template<typename T>
		T NextPrime(T X)
		{
			T N = X + 1;
			while (!IsPrime(N))
			{
				++N;
			}
			return N;
		}

		inline float Pythagoras(float A, float B)
		{
			return Sqrt(Square(A) + Square(B));
		}
	}
}
