#include "Core/NexusEnginePch.h"
#include "Misc.h"

namespace NxEn
{
	bool IsPowerOfTwo(uint64 Number)
	{
		return (Number & (Number - 1)) == 0;
	}
	
	int32 LogTwoPowerOfTwo(uint32 Number)
	{
		NEXUS_ASSERT(IsPowerOfTwo(Number), "Number is not a power of 2");

		NEXUS_BITSCANFORWARD(Number, Index, Result)
		return Result > 0 ? Index : -1;
	}

#define MODULO(Type, Number, Mode)\
	NEXUS_ASSERT(Mod != 0, "Modulo with Mode == 0 is undefined");\
	Type Remainder = Number % Mod;\
	return Remainder < 0 ? Remainder + Mod : Remainder;

	int8 Modulo(int8 Number, int8 Mod)
	{
		MODULO(int8, Number, Mode);
	}

	int16 Modulo(int16 Number, int16 Mod)
	{
		MODULO(int16, Number, Mode);
	}

	int32 Modulo(int32 Number, int32 Mod)
	{
		MODULO(int32, Number, Mode);
	}

	int64 Modulo(int64 Number, int64 Mod)
	{
		MODULO(int64, Number, Mode);
	}

	uint8 Modulo(uint8 Number, uint8 Mod)
	{
		MODULO(uint8, Number, Mode);
	}

	uint16 Modulo(uint16 Number, uint16 Mod)
	{
		MODULO(uint16, Number, Mode);
	}
	
	uint32 Modulo(uint32 Number, uint32 Mod)
	{
		MODULO(uint32, Number, Mode);
	}

	uint64 Modulo(uint64 Number, uint64 Mod)
	{
		MODULO(uint64, Number, Mode);
	}

	bool IsPrime(uint64 Number)
	{
		if (Number <= 1) return false;

		for (uint64 N = 2; N <= sqrt(Number); N++)
		{
			if (Number % N == 0) return false;
		}

		return true;
	}

	uint64 NextPrime(uint64 Number)
	{
		uint64 N = Number + 1;
		while (!IsPrime(N))
		{
			N++;
		}

		return N;
	}
}
