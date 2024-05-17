#include "Core/NexusEnginePch.h"
#include "Hash.h"

namespace NxEn
{
	XxHash32::XxHash32(uint32 Seed)
		: Seed(Seed), Size(0), BufferSize(0)
	{
		State[0] = Seed + PrimeA + PrimeB;
		State[1] = Seed + PrimeB;
		State[2] = Seed;
		State[3] = Seed - PrimeA;
		Buffer[0] = 0;
	}

	uint32 XxHash32::Hash(const void* Data, uint64 Length, uint32 Seed)
	{
		XxHash32 Hash = XxHash32(Seed);
		Hash.Accumulate(Data, Length);
		return Hash.Hash();
	}

	XxHash32& XxHash32::Accumulate(const void* Data, uint64 Length)
	{
		NEXUS_ASSERT(Data && Length > 0, "Invalid Data");

		Size += Length;
		const uint8* Pointer = reinterpret_cast<const uint8*>(Data);
		const uint8* Stop = Pointer + Length;
		const uint8* StopBlock = Stop - MaxBufferSize;

		// Save data for later until the buffer is completly filled
		if (BufferSize + Length < MaxBufferSize)
		{
			CopyIntoBuffer(Buffer, Pointer, (uint8)Length, BufferSize);
			return *this;
		}

		// Fill the buffer with a mix of previous and current data
		// Process the Pointer and move forward the pointer
		if (BufferSize > 0)
		{
			Pointer = CopyIntoBuffer(Buffer, Pointer, MaxBufferSize - BufferSize, BufferSize);
			Process(Buffer, State[0], State[1], State[2], State[3]);
			BufferSize = 0;
		}

		// Process the data by 16 bytes block
		uint32 S0 = State[0], S1 = State[1], S2 = State[2], S3 = State[3];
		while (Pointer < StopBlock)
		{
			Process(Pointer, S0, S1, S2, S3);
			Pointer += 16;
		}
		State[0] = S0; State[1] = S1; State[2] = S2; State[3] = S3;

		// Copy the remaining of the data in the temp buffer
		CopyIntoBuffer(Buffer, Pointer, (uint8)(Stop - Pointer), BufferSize);

		return *this;
	}

	uint32 XxHash32::Hash() const
	{
		const uint8* Pointer = Buffer;
		const uint8* Stop = Pointer + BufferSize;

		uint32 Result = Converge(Seed, (uint32)Size, State);
		Result = Remains(Result, Pointer, Stop);
		Result = Avalanche(Result);

		return Result;
	}

	inline const uint8* XxHash32::CopyIntoBuffer(uint8* Buffer, const uint8* Pointer, uint8 Length, uint8& BufferIndex)
	{
		while (Length-- > 0)
		{
			Buffer[BufferIndex++] = *Pointer++;
		}

		return Pointer;
	}

	inline void XxHash32::Process(const void* Pointer, uint32& State0, uint32& State1, uint32& State2, uint32& State3)
	{
		const uint32* Block = (const uint32*)Pointer;
		State0 = Process(State0, Block[0]);
		State1 = Process(State1, Block[1]);
		State2 = Process(State2, Block[2]);
		State3 = Process(State3, Block[3]);
	}

	inline uint32 XxHash32::Remains(uint32 Result, const uint8* Pointer, const uint8* Stop)
	{
		for (; Pointer + 4 <= Stop; Pointer += 4)
		{
			Result = RotateLeft(Result + (*(uint32*)Pointer) * PrimeC, 17) * PrimeD;
		}

		while (Pointer != Stop)
		{
			Result = RotateLeft(Result + (*Pointer++) * PrimeE, 11) * PrimeA;
		}

		return Result;
	}

	inline uint32 XxHash32::RotateLeft(uint32 Value, uint8 Bits)
	{
		return (Value << Bits) | (Value >> (32 - Bits));
	}

	inline uint32 XxHash32::Process(uint32 Value, uint32 Pointer)
	{
		Value += Pointer * PrimeB;
		Value = RotateLeft(Value, 13);
		Value *= PrimeA;
		return Value;
	}

	inline uint32 XxHash32::Converge(uint32 Seed, uint32 Size, const uint32* State)
	{
		uint32 Result = 0;
		if (Size >= MaxBufferSize)
		{
			Result += RotateLeft(State[0], 1) + RotateLeft(State[1], 7) + RotateLeft(State[2], 12) + RotateLeft(State[3], 18);
		}
		else
		{
			Result += Seed + PrimeE;
		}
		Result += Size;
		return Result;
	}

	inline uint32 XxHash32::Avalanche(uint32 Accumulator)
	{
		Accumulator ^= Accumulator >> 15;
		Accumulator *= PrimeB;
		Accumulator ^= Accumulator >> 13;
		Accumulator *= PrimeC;
		Accumulator ^= Accumulator >> 16;
		return Accumulator;
	}

	XxHash64::XxHash64(uint64 Seed)
		: Seed(Seed), Size(0), BufferSize(0)
	{
		State[0] = Seed + PrimeA + PrimeB;
		State[1] = Seed + PrimeB;
		State[2] = Seed;
		State[3] = Seed - PrimeA;
		Buffer[0] = 0;
	}

	uint64 XxHash64::Hash(const void* Data, uint64 Length, uint64 Seed)
	{
		XxHash64 Hash = XxHash64(Seed);
		Hash.Accumulate(Data, Length);
		return Hash.Hash();
	}
	
	XxHash64& XxHash64::Accumulate(const void* Data, uint64 Length)
	{
		NEXUS_ASSERT(Data && Length > 0, "Invalid Data");

		Size += Length;
		const uint8* Pointer = reinterpret_cast<const uint8*>(Data);
		const uint8* Stop = Pointer + Length;
		const uint8* StopBlock = Stop - MaxBufferSize;

		// Save data for later until the buffer is completly filled
		if (BufferSize + Length < MaxBufferSize)
		{
			CopyIntoBuffer(Buffer, Pointer, (uint8)Length, BufferSize);
			return *this;
		}

		// Fill the buffer with a mix of previous and current data
		// Process the Pointer and move forward the pointer
		if (BufferSize > 0)
		{
			Pointer = CopyIntoBuffer(Buffer, Pointer, MaxBufferSize - BufferSize, BufferSize);
			Process(Buffer, State[0], State[1], State[2], State[3]);
			BufferSize = 0;
		}

		// Process the data by 32 bytes block
		uint64 S0 = State[0], S1 = State[1], S2 = State[2], S3 = State[3];
		while (Pointer < StopBlock)
		{
			Process(Pointer, S0, S1, S2, S3);
			Pointer += 32;
		}
		State[0] = S0; State[1] = S1; State[2] = S2; State[3] = S3;

		// Copy the remaining of the data in the temp buffer
		CopyIntoBuffer(Buffer, Pointer, (uint8)(Stop - Pointer), BufferSize);

		return *this;
	}
	
	uint64 XxHash64::Hash() const
	{
		// Process remaining data
		const uint8* Pointer = Buffer;
		const uint8* Stop = Pointer + BufferSize;

		uint64 Result = Converge(Seed, Size, State);
		Result = Remains(Result, Pointer, Stop);
		Result = Avalanche(Result);

		return Result;
	}
	
	inline const uint8* XxHash64::CopyIntoBuffer(uint8* Buffer, const uint8* Pointer, uint8 Length, uint8& BufferIndex)
	{
		while (Length-- > 0)
		{
			Buffer[BufferIndex++] = *Pointer++;
		}

		return Pointer;
	}
	
	inline void XxHash64::Process(const void* Pointer, uint64& State0, uint64& State1, uint64& State2, uint64& State3)
	{
		const uint64* Block = (const uint64*)Pointer;
		State0 = Process(State0, Block[0]);
		State1 = Process(State1, Block[1]);
		State2 = Process(State2, Block[2]);
		State3 = Process(State3, Block[3]);
	}

	inline uint64 XxHash64::Remains(uint64 Result, const uint8* Pointer, const uint8* Stop)
	{
		for (; Pointer + 8 <= Stop; Pointer += 8)
		{
			Result = RotateLeft(Result ^ Process(0, *(uint64*)Pointer), 27) * PrimeA + PrimeD;
		}

		if (Pointer + 4 <= Stop)
		{
			Result = RotateLeft(Result ^ (*(uint32*)Pointer) * PrimeA, 23) * PrimeB + PrimeC;
			Pointer += 4;
		}

		while (Pointer != Stop)
		{
			Result = RotateLeft(Result ^ (*Pointer++) * PrimeE, 11) * PrimeA;
		}

		return Result;
	}

	inline uint64 XxHash64::RotateLeft(uint64 Value, uint8 Bits)
	{
		return (Value << Bits) | (Value >> (64 - Bits));
	}
	
	inline uint64 XxHash64::Process(uint64 Value, uint64 Pointer)
	{
		Value += Pointer * PrimeB;
		Value = RotateLeft(Value, 31);
		Value *= PrimeA;
		return Value;
	}

	inline uint64 XxHash64::Converge(uint64 Seed, uint64 Size, const uint64* State)
	{
		uint64 Result = 0;
		if (Size >= MaxBufferSize)
		{
			Result = RotateLeft(State[0], 1) + RotateLeft(State[1], 7) + RotateLeft(State[2], 12) + RotateLeft(State[3], 18);
			Result = Merge(Result, State[0]);
			Result = Merge(Result, State[1]);
			Result = Merge(Result, State[2]);
			Result = Merge(Result, State[3]);
		}
		else
		{
			Result += Seed + PrimeE;
		}
		Result += Size;
		return Result;
	}

	inline uint64 XxHash64::Merge(uint64 Value, uint64 Accumulator)
	{
		return (Value ^ Process(0, Accumulator)) * PrimeA + PrimeD;
	}
	
	inline uint64 XxHash64::Avalanche(uint64 Accumulator)
	{
		Accumulator ^= Accumulator >> 33;
		Accumulator *= PrimeB;
		Accumulator ^= Accumulator >> 29;
		Accumulator *= PrimeC;
		Accumulator ^= Accumulator >> 32;
		return Accumulator;
	}
}