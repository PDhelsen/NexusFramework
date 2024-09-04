#include "Core/NexusEnginePch.h"
#include "Math.h"

namespace NxEn
{
	namespace Math
	{
		float Acos(float X)
		{
			return acos(X);
		}

		double Acos(double X)
		{
			return acos(X);
		}

		float Asin(float X)
		{
			return asin(X);
		}

		double Asin(double X)
		{
			return asin(X);
		}

		float Atan(float X)
		{
			return atan(X);
		}

		double Atan(double X)
		{
			return atan(X);
		}

		float Atan(float Y, float X)
		{
			return atan2(Y, X);
		}

		double Atan(double Y, float X)
		{
			return atan2(Y, X);
		}

		float Cos(float X)
		{
			return cos(X);
		}

		double Cos(double X)
		{
			return cos(X);
		}

		float Cosh(float X)
		{
			return cosh(X);
		}

		double Cosh(double X)
		{
			return cosh(X);
		}

		float Sin(float X)
		{
			return sin(X);
		}

		double Sin(double X)
		{
			return sin(X);
		}

		float Sinh(float X)
		{
			return sinh(X);
		}

		double Sinh(double X)
		{
			return sinh(X);
		}

		float Tan(float X)
		{
			return tan(X);
		}

		double Tan(double X)
		{
			return tan(X);
		}

		float Tanh(float X)
		{
			return tanh(X);
		}

		double Tanh(double X)
		{
			return tanh(X);
		}

		float Exp(float X)
		{
			return exp(X);
		}

		double Exp(double X)
		{
			return exp(X);
		}

		float Log(float X)
		{
			return log(X);
		}

		double Log(double X)
		{
			return log(X);
		}

		float Log10(float X)
		{
			return log10(X);
		}

		double Log10(double X)
		{
			return log10(X);
		}

		float Pow(float X, float Y)
		{
			return pow(X, Y);
		}

		double Pow(double X, float Y)
		{
			return pow(X, Y);
		}

		int8 Pow(int8 X, int8 Y)
		{
			return (int8)pow(X, Y);
		}

		int16 Pow(int16 X, int16 Y)
		{
			return (int16)pow(X, Y);
		}

		int32 Pow(int32 X, int32 Y)
		{
			return (int32)pow(X, Y);
		}

		int64 Pow(int64 X, int64 Y)
		{
			return (int64)pow(X, Y);
		}

		uint8 Pow(uint8 X, uint8 Y)
		{
			return (uint8)pow(X, Y);
		}

		uint16 Pow(uint16 X, uint16 Y)
		{
			return (uint16)pow(X, Y);
		}

		uint32 Pow(uint32 X, uint32 Y)
		{
			return (uint32)pow(X, Y);
		}

		uint64 Pow(uint64 X, uint64 Y)
		{
			return (uint64)pow(X, Y);
		}

		float Sqrt(float X)
		{
			return sqrt(X);
		}

		double Sqrt(double X)
		{
			return sqrt(X);
		}

		constexpr float Square(float X)
		{
			return X * X;
		}

		constexpr double Square(double X)
		{
			return X * X;
		}

		constexpr int8 Square(int8 X)
		{
			return X * X;
		}

		constexpr int16 Square(int16 X)
		{
			return X * X;
		}

		constexpr int32 Square(int32 X)
		{
			return X * X;
		}

		constexpr int64 Square(int64 X)
		{
			return X * X;
		}

		constexpr uint8 Square(uint8 X)
		{
			return X * X;
		}

		constexpr uint16 Square(uint16 X)
		{
			return X * X;
		}

		constexpr uint32 Square(uint32 X)
		{
			return X * X;
		}

		constexpr uint64 Square(uint64 X)
		{
			return X * X;
		}

		float Abs(float X)
		{
			return abs(X);
		}

		double Abs(double X)
		{
			return abs(X);
		}

		int8 Abs(int8 X)
		{
			return  abs(X);
		}

		int16 Abs(int16 X)
		{
			return  abs(X);
		}

		int32 Abs(int32 X)
		{
			return  abs(X);
		}

		int64 Abs(int64 X)
		{
			return  abs(X);
		}

		float Ceil(float X)
		{
			return ceil(X);
		}

		double Ceil(double X)
		{
			return ceil(X);
		}

		int32 CeilToInt(float X)
		{
			return (int32)ceil(X);
		}

		int64 CeilToInt(double X)
		{
			return (int64)ceil(X);
		}

		float Floor(float X)
		{
			return floor(X);
		}

		double Floor(double X)
		{
			return floor(X);
		}

		int32 FloorToInt(float X)
		{
			return (int32)floor(X);
		}

		int64 FloorToInt(double X)
		{
			return (int64)floor(X);
		}

		float Frac(float X)
		{
			return X - Floor(X);
		}

		double Frac(double X)
		{
			return X - Floor(X);
		}

		float Round(float X)
		{
			return round(X);
		}

		double Round(double X)
		{
			return round(X);
		}

		int32 RoundToInt(float X)
		{
			return (int32)round(X);
		}

		int64 RoundToInt(double X)
		{
			return (int64)round(X);
		}

		float Modf(float X, float& Y)
		{
			return modf(X, &Y);
		}

		double Modf(double X, double& Y)
		{
			return modf(X, &Y);
		}

		float FMod(float X, float Y)
		{
			return fmod(X, Y);
		}

		double FMod(double X, double Y)
		{
			return fmod(X, Y);
		}

		int8 Modulo(int8 X, int8 Y)
		{
			NEXUS_ASSERT(Y != 0, "Modulo with Mode == 0 is undefined");
			int8 Remainder = X % Y;
			return Remainder < 0 ? Remainder + Y : Remainder;
		}

		int16 Modulo(int16 X, int16 Y)
		{
			NEXUS_ASSERT(Y != 0, "Modulo with Mode == 0 is undefined");
			int16 Remainder = X % Y;
			return Remainder < 0 ? Remainder + Y : Remainder;
		}

		int32 Modulo(int32 X, int32 Y)
		{
			NEXUS_ASSERT(Y != 0, "Modulo with Mode == 0 is undefined");
			int32 Remainder = X % Y;
			return Remainder < 0 ? Remainder + Y : Remainder;
		}

		int64 Modulo(int64 X, int64 Y)
		{
			NEXUS_ASSERT(Y != 0, "Modulo with Mode == 0 is undefined");
			int64 Remainder = X % Y;
			return Remainder < 0 ? Remainder + Y : Remainder;
		}

		uint8 Modulo(uint8 X, uint8 Y)
		{
			NEXUS_ASSERT(Y != 0, "Modulo with Mode == 0 is undefined");
			uint8 Remainder = X % Y;
			return Remainder < 0 ? Remainder + Y : Remainder;
		}

		uint16 Modulo(uint16 X, uint16 Y)
		{
			NEXUS_ASSERT(Y != 0, "Modulo with Mode == 0 is undefined");
			uint16 Remainder = X % Y;
			return Remainder < 0 ? Remainder + Y : Remainder;
		}

		uint32 Modulo(uint32 X, uint32 Y)
		{
			NEXUS_ASSERT(Y != 0, "Modulo with Mode == 0 is undefined");
			uint32 Remainder = X % Y;
			return Remainder < 0 ? Remainder + Y : Remainder;
		}

		uint64 Modulo(uint64 X, uint64 Y)
		{
			NEXUS_ASSERT(Y != 0, "Modulo with Mode == 0 is undefined");
			uint64 Remainder = X % Y;
			return Remainder < 0 ? Remainder + Y : Remainder;
		}

		constexpr bool IsPowerOfTwo(uint8 X)
		{
			return (X & (X - 1)) == 0;
		}

		constexpr bool IsPowerOfTwo(uint16 X)
		{
			return (X & (X - 1)) == 0;
		}

		constexpr bool IsPowerOfTwo(uint32 X)
		{
			return (X & (X - 1)) == 0;
		}

		constexpr bool IsPowerOfTwo(uint64 X)
		{
			return (X & (X - 1)) == 0;
		}

		constexpr uint8 NextPowerOfTwo(uint8 X)
		{
			if (X == 0)
			{
				return 1;
			}

			X--;
			X |= X >> 1;
			X |= X >> 2;
			X |= X >> 4;

			return X + 1;
		}

		constexpr uint16 NextPowerOfTwo(uint16 X)
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

			return X + 1;
		}

		constexpr uint32 NextPowerOfTwo(uint32 X)
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

			return X + 1;
		}

		constexpr uint64 NextPowerOfTwo(uint64 X)
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

			return X + 1;
		}

		int8 LogTwoPowerOfTwo(uint8 X)
		{
			NEXUS_ASSERT(IsPowerOfTwo(X), "X is not a power of 2");
			NEXUS_BITSCANFORWARD(X, Index, Result)
			return Result > 0 ? (int8)Index : -1;
		}

		int16 LogTwoPowerOfTwo(uint16 X)
		{
			NEXUS_ASSERT(IsPowerOfTwo(X), "X is not a power of 2");
			NEXUS_BITSCANFORWARD(X, Index, Result)
			return Result > 0 ? (int16)Index : -1;
		}

		int32 LogTwoPowerOfTwo(uint32 X)
		{
			NEXUS_ASSERT(IsPowerOfTwo(X), "X is not a power of 2");
			NEXUS_BITSCANFORWARD(X, Index, Result)
			return Result > 0 ? (int32)Index : -1;
		}

		int64 LogTwoPowerOfTwo(uint64 X)
		{
			NEXUS_ASSERT(IsPowerOfTwo(X), "X is not a power of 2");
			NEXUS_BITSCANFORWARD(X, Index, Result)
			return Result > 0 ? (int64)Index : -1;
		}

		constexpr bool IsPrime(uint8 X)
		{
			if (X <= 1) return false;
			for (uint8 N = 2; N <= sqrt(X); ++N)
			{
				if (X % N == 0) return false;
			}
			return true;
		}

		constexpr bool IsPrime(uint16 X)
		{
			if (X <= 1) return false;
			for (uint16 N = 2; N <= sqrt(X); ++N)
			{
				if (X % N == 0) return false;
			}
			return true;
		}

		constexpr bool IsPrime(uint32 X)
		{
			if (X <= 1) return false;
			for (uint32 N = 2; N <= sqrt(X); ++N)
			{
				if (X % N == 0) return false;
			}
			return true;
		}

		constexpr bool IsPrime(uint64 X)
		{
			if (X <= 1) return false;
			for (uint64 N = 2; N <= sqrt(X); ++N)
			{
				if (X % N == 0) return false;
			}
			return true;
		}

		constexpr uint8 NextPrime(uint8 X)
		{
			uint8 N = X + 1;
			while (!IsPrime(N))
			{
				++N;
			}
			return N;
		}

		constexpr uint16 NextPrime(uint16 X)
		{
			uint16 N = X + 1;
			while (!IsPrime(N))
			{
				++N;
			}
			return N;
		}

		constexpr uint32 NextPrime(uint32 X)
		{
			uint32 N = X + 1;
			while (!IsPrime(N))
			{
				++N;
			}
			return N;
		}

		constexpr uint64 NextPrime(uint64 X)
		{
			uint64 N = X + 1;
			while (!IsPrime(N))
			{
				++N;
			}
			return N;
		}
	}
}
