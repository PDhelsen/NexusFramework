#include "Core/NexusEnginePch.h"
#include "Hash.h"

namespace NxEn
{
	Hash32::Hash32(uint32 Seed)
		: Seed(Seed), Size(0), BufferSize(0)
	{
		State[0] = Seed + PrimeA + PrimeB;
		State[1] = Seed + PrimeB;
		State[2] = Seed;
		State[3] = Seed - PrimeA;
		Buffer[0] = 0;
	}

	uint32 Hash32::Hash(const void* Input, uint64 Length, uint32 Seed)
	{
		Hash32 Hash = Hash32(Seed);
		Hash.Accumulate(Input, Length);
		return Hash.Hash();
	}

	Hash32& Hash32::Accumulate(const void* Input, uint64 Length)
	{
		NEXUS_ASSERT(Input && Length > 0, "Invalid Input");

		Size += Length;
		const uint8* Data = reinterpret_cast<const uint8*>(Input);
		const uint8* Stop = Data + Length;
		const uint8* StopBlock = Stop - MaxBufferSize;

		// Save data for later until the buffer is completly filled
		if (BufferSize + Length < MaxBufferSize)
		{
			CopyIntoBuffer(Buffer, Data, (uint8)Length, BufferSize);
			return *this;
		}

		// Fill the buffer with a mix of previous and current data
		// Process the Data and move forward the pointer
		if (BufferSize > 0)
		{
			Data = CopyIntoBuffer(Buffer, Data, MaxBufferSize - BufferSize, BufferSize);
			Process(Buffer, State[0], State[1], State[2], State[3]);
			BufferSize = 0;
		}

		// Process the data by 16 bytes block
		uint32 S0 = State[0], S1 = State[1], S2 = State[2], S3 = State[3];
		while (Data < StopBlock)
		{
			Process(Data, S0, S1, S2, S3);
			Data += 16;
		}
		State[0] = S0; State[1] = S1; State[2] = S2; State[3] = S3;

		// Copy the remaining of the data in the temp buffer
		CopyIntoBuffer(Buffer, Data, (uint8)(Stop - Data), BufferSize);

		return *this;
	}

	uint32 Hash32::Hash() const
	{
		const uint8* Data = Buffer;
		const uint8* Stop = Data + BufferSize;

		uint32 Result = Converge(Seed, Size, State);
		Result = Remains(Result, Data, Stop);
		Result = Avalanche(Result);

		return Result;
	}

	inline const uint8* Hash32::CopyIntoBuffer(uint8* Buffer, const uint8* Data, uint8 Length, uint8& BufferIndex)
	{
		while (Length-- > 0)
		{
			Buffer[BufferIndex++] = *Data++;
		}

		return Data;
	}

	inline void Hash32::Process(const void* Data, uint32& State0, uint32& State1, uint32& State2, uint32& State3)
	{
		const uint32* Block = (const uint32*)Data;
		State0 = Process(State0, Block[0]);
		State1 = Process(State1, Block[1]);
		State2 = Process(State2, Block[2]);
		State3 = Process(State3, Block[3]);
	}

	inline uint32 Hash32::Remains(uint32 Result, const uint8* Data, const uint8* Stop)
	{
		for (; Data + 4 <= Stop; Data += 4)
		{
			Result = RotateLeft(Result + (*(uint32*)Data) * PrimeC, 17) * PrimeD;
		}

		while (Data != Stop)
		{
			Result = RotateLeft(Result + (*Data++) * PrimeE, 11) * PrimeA;
		}

		return Result;
	}

	inline uint32 Hash32::RotateLeft(uint32 Value, uint8 Bits)
	{
		return (Value << Bits) | (Value >> (32 - Bits));
	}

	inline uint32 Hash32::Process(uint32 Value, uint32 Data)
	{
		Value += Data * PrimeB;
		Value = RotateLeft(Value, 13);
		Value *= PrimeA;
		return Value;
	}

	inline uint32 Hash32::Converge(uint32 Seed, uint32 Size, const uint32* State)
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

	inline uint32 Hash32::Avalanche(uint32 Accumulator)
	{
		Accumulator ^= Accumulator >> 15;
		Accumulator *= PrimeB;
		Accumulator ^= Accumulator >> 13;
		Accumulator *= PrimeC;
		Accumulator ^= Accumulator >> 16;
		return Accumulator;
	}

	Hash64::Hash64(uint64 Seed)
		: Seed(Seed), Size(0), BufferSize(0)
	{
		State[0] = Seed + PrimeA + PrimeB;
		State[1] = Seed + PrimeB;
		State[2] = Seed;
		State[3] = Seed - PrimeA;
		Buffer[0] = 0;
	}

	uint64 Hash64::Hash(const void* Input, uint64 Length, uint64 Seed)
	{
		Hash64 Hash = Hash64(Seed);
		Hash.Accumulate(Input, Length);
		return Hash.Hash();
	}
	
	Hash64& Hash64::Accumulate(const void* Input, uint64 Length)
	{
		NEXUS_ASSERT(Input && Length > 0, "Invalid Input");

		Size += Length;
		const uint8* Data = reinterpret_cast<const uint8*>(Input);
		const uint8* Stop = Data + Length;
		const uint8* StopBlock = Stop - MaxBufferSize;

		// Save data for later until the buffer is completly filled
		if (BufferSize + Length < MaxBufferSize)
		{
			CopyIntoBuffer(Buffer, Data, (uint8)Length, BufferSize);
			return *this;
		}

		// Fill the buffer with a mix of previous and current data
		// Process the Data and move forward the pointer
		if (BufferSize > 0)
		{
			Data = CopyIntoBuffer(Buffer, Data, MaxBufferSize - BufferSize, BufferSize);
			Process(Buffer, State[0], State[1], State[2], State[3]);
			BufferSize = 0;
		}

		// Process the data by 32 bytes block
		uint64 S0 = State[0], S1 = State[1], S2 = State[2], S3 = State[3];
		while (Data < StopBlock)
		{
			Process(Data, S0, S1, S2, S3);
			Data += 32;
		}
		State[0] = S0; State[1] = S1; State[2] = S2; State[3] = S3;

		// Copy the remaining of the data in the temp buffer
		CopyIntoBuffer(Buffer, Data, (uint8)(Stop - Data), BufferSize);

		return *this;
	}
	
	uint64 Hash64::Hash() const
	{
		// Process remaining data
		const uint8* Data = Buffer;
		const uint8* Stop = Data + BufferSize;

		uint64 Result = Converge(Seed, Size, State);
		Result = Remains(Result, Data, Stop);
		Result = Avalanche(Result);

		return Result;
	}
	
	inline const uint8* Hash64::CopyIntoBuffer(uint8* Buffer, const uint8* Data, uint8 Length, uint8& BufferIndex)
	{
		while (Length-- > 0)
		{
			Buffer[BufferIndex++] = *Data++;
		}

		return Data;
	}
	
	inline void Hash64::Process(const void* Data, uint64& State0, uint64& State1, uint64& State2, uint64& State3)
	{
		const uint64* Block = (const uint64*)Data;
		State0 = Process(State0, Block[0]);
		State1 = Process(State1, Block[1]);
		State2 = Process(State2, Block[2]);
		State3 = Process(State3, Block[3]);
	}

	inline uint64 Hash64::Remains(uint64 Result, const uint8* Data, const uint8* Stop)
	{
		for (; Data + 8 <= Stop; Data += 8)
		{
			Result = RotateLeft(Result ^ Process(0, *(uint64*)Data), 27) * PrimeA + PrimeD;
		}

		if (Data + 4 <= Stop)
		{
			Result = RotateLeft(Result ^ (*(uint32*)Data) * PrimeA, 23) * PrimeB + PrimeC;
			Data += 4;
		}

		while (Data != Stop)
		{
			Result = RotateLeft(Result ^ (*Data++) * PrimeE, 11) * PrimeA;
		}

		return Result;
	}

	inline uint64 Hash64::RotateLeft(uint64 Value, uint8 Bits)
	{
		return (Value << Bits) | (Value >> (64 - Bits));
	}
	
	inline uint64 Hash64::Process(uint64 Value, uint64 Data)
	{
		Value += Data * PrimeB;
		Value = RotateLeft(Value, 31);
		Value *= PrimeA;
		return Value;
	}

	inline uint64 Hash64::Converge(uint64 Seed, uint64 Size, const uint64* State)
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

	inline uint64 Hash64::Merge(uint64 Value, uint64 Accumulator)
	{
		return (Value ^ Process(0, Accumulator)) * PrimeA + PrimeD;
	}
	
	inline uint64 Hash64::Avalanche(uint64 Accumulator)
	{
		Accumulator ^= Accumulator >> 33;
		Accumulator *= PrimeB;
		Accumulator ^= Accumulator >> 29;
		Accumulator *= PrimeC;
		Accumulator ^= Accumulator >> 32;
		return Accumulator;
	}
}