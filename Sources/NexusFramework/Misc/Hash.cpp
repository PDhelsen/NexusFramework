#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Misc/Hash.h"

namespace NxFr
{
	namespace Hashing
	{
		//-----------------------------------------------------------------------------------------------------------------------
		// XxHash32
		//-----------------------------------------------------------------------------------------------------------------------

		XxHash32::XxHash32(HashLength Seed)
			: HashAlgorithm<HashLength>(Seed), Seed(Seed), Size(0), BufferSize(0)
		{
			State[0] = Seed + PrimeA + PrimeB;
			State[1] = Seed + PrimeB;
			State[2] = Seed;
			State[3] = Seed - PrimeA;
			Buffer[0] = 0;
		}

		XxHash32& XxHash32::Accumulate(const void* Data, uint64 Length)
		{
			NEXUS_ASSERT(Data && Length > 0, Default, "Invalid Data");

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
			HashLength S0 = State[0], S1 = State[1], S2 = State[2], S3 = State[3];
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

		XxHash32::HashLength XxHash32::Hash() const
		{
			const uint8* Pointer = Buffer;
			const uint8* Stop = Pointer + BufferSize;

			HashLength Result = Converge(Seed, (HashLength)Size, State);
			Result = Remains(Result, Pointer, Stop);
			Result = Avalanche(Result);

			return Finalize(Result);
		}

		XxHash32::HashLength XxHash32::Combine(HashLength HashA, HashLength HashB) const
		{
			HashLength HashCombined = HashA;
			HashCombined ^= HashB + 0x9e3779b9 + (HashCombined << 6) + (HashCombined >> 2);
			return Finalize(HashCombined);
		}

		const uint8* XxHash32::CopyIntoBuffer(uint8* Buffer, const uint8* Pointer, uint8 Length, uint8& BufferIndex)
		{
			while (Length-- > 0)
			{
				Buffer[BufferIndex++] = *Pointer++;
			}

			return Pointer;
		}

		void XxHash32::Process(const void* Pointer, HashLength& State0, HashLength& State1, HashLength& State2, HashLength& State3)
		{
			const HashLength* Block = (const HashLength*)Pointer;
			State0 = Process(State0, Block[0]);
			State1 = Process(State1, Block[1]);
			State2 = Process(State2, Block[2]);
			State3 = Process(State3, Block[3]);
		}

		XxHash32::HashLength XxHash32::Remains(HashLength Result, const uint8* Pointer, const uint8* Stop)
		{
			for (; Pointer + 4 <= Stop; Pointer += 4)
			{
				Result = RotateLeft(Result + (*(HashLength*)Pointer) * PrimeC, 17) * PrimeD;
			}

			while (Pointer != Stop)
			{
				Result = RotateLeft(Result + (*Pointer++) * PrimeE, 11) * PrimeA;
			}

			return Result;
		}

		XxHash32::HashLength XxHash32::RotateLeft(HashLength Value, uint8 Bits)
		{
			return (Value << Bits) | (Value >> (32 - Bits));
		}

		XxHash32::HashLength XxHash32::Process(HashLength Value, HashLength Pointer)
		{
			Value += Pointer * PrimeB;
			Value = RotateLeft(Value, 13);
			Value *= PrimeA;
			return Value;
		}

		XxHash32::HashLength XxHash32::Converge(HashLength Seed, HashLength Size, const HashLength* State)
		{
			HashLength Result = 0;
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

		XxHash32::HashLength XxHash32::Avalanche(HashLength Accumulator)
		{
			Accumulator ^= Accumulator >> 15;
			Accumulator *= PrimeB;
			Accumulator ^= Accumulator >> 13;
			Accumulator *= PrimeC;
			Accumulator ^= Accumulator >> 16;
			return Accumulator;
		}

		//-----------------------------------------------------------------------------------------------------------------------
		// XxHash64
		//-----------------------------------------------------------------------------------------------------------------------

		XxHash64::XxHash64(HashLength Seed)
			: HashAlgorithm<HashLength>(Seed), Seed(Seed), Size(0), BufferSize(0)
		{
			State[0] = Seed + PrimeA + PrimeB;
			State[1] = Seed + PrimeB;
			State[2] = Seed;
			State[3] = Seed - PrimeA;
			Buffer[0] = 0;
		}

		XxHash64& XxHash64::Accumulate(const void* Data, uint64 Length)
		{
			NEXUS_ASSERT(Data && Length > 0, Default, "Invalid Data");

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
			HashLength S0 = State[0], S1 = State[1], S2 = State[2], S3 = State[3];
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

		XxHash64::HashLength XxHash64::Hash() const
		{
			// Process remaining data
			const uint8* Pointer = Buffer;
			const uint8* Stop = Pointer + BufferSize;

			HashLength Result = Converge(Seed, Size, State);
			Result = Remains(Result, Pointer, Stop);
			Result = Avalanche(Result);

			return Finalize(Result);
		}

		XxHash64::HashLength XxHash64::Combine(HashLength HashA, HashLength HashB) const
		{
			HashLength HashCombined = HashA;
			HashCombined ^= HashB + 0x9e3779b97f4a7c15 + (HashCombined << 6) + (HashCombined >> 2);
			return Finalize(HashCombined);
		}

		const uint8* XxHash64::CopyIntoBuffer(uint8* Buffer, const uint8* Pointer, uint8 Length, uint8& BufferIndex)
		{
			while (Length-- > 0)
			{
				Buffer[BufferIndex++] = *Pointer++;
			}

			return Pointer;
		}

		void XxHash64::Process(const void* Pointer, HashLength& State0, HashLength& State1, HashLength& State2, HashLength& State3)
		{
			const HashLength* Block = (const HashLength*)Pointer;
			State0 = Process(State0, Block[0]);
			State1 = Process(State1, Block[1]);
			State2 = Process(State2, Block[2]);
			State3 = Process(State3, Block[3]);
		}

		XxHash64::HashLength XxHash64::Remains(HashLength Result, const uint8* Pointer, const uint8* Stop)
		{
			for (; Pointer + 8 <= Stop; Pointer += 8)
			{
				Result = RotateLeft(Result ^ Process(0, *(HashLength*)Pointer), 27) * PrimeA + PrimeD;
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

		XxHash64::HashLength XxHash64::RotateLeft(HashLength Value, uint8 Bits)
		{
			return (Value << Bits) | (Value >> (64 - Bits));
		}

		XxHash64::HashLength XxHash64::Process(HashLength Value, HashLength Pointer)
		{
			Value += Pointer * PrimeB;
			Value = RotateLeft(Value, 31);
			Value *= PrimeA;
			return Value;
		}

		XxHash64::HashLength XxHash64::Converge(HashLength Seed, HashLength Size, const HashLength* State)
		{
			HashLength Result = 0;
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

		XxHash64::HashLength XxHash64::Merge(HashLength Value, HashLength Accumulator)
		{
			return (Value ^ Process(0, Accumulator)) * PrimeA + PrimeD;
		}

		XxHash64::HashLength XxHash64::Avalanche(HashLength Accumulator)
		{
			Accumulator ^= Accumulator >> 33;
			Accumulator *= PrimeB;
			Accumulator ^= Accumulator >> 29;
			Accumulator *= PrimeC;
			Accumulator ^= Accumulator >> 32;
			return Accumulator;
		}

		//-----------------------------------------------------------------------------------------------------------------------
		//	Murmur32
		//-----------------------------------------------------------------------------------------------------------------------

		Murmur32::Murmur32(HashLength Seed)
			: HashAlgorithm<HashLength>(Seed), Seed(Seed), Size(0), Accumulator(0)
		{
			Accumulator = Seed;
		}

		Murmur32& Murmur32::Accumulate(const void* Data, uint64 Length)
		{
			NEXUS_ASSERT(Data && Length > 0, Default, "Invalid Data");

			Size += Length;

			const uint64 NbBlocks = Length / 4;
			const uint8* Pointer = reinterpret_cast<const uint8*>(Data);

			HashLength K = 0;
			const HashLength* Blocks = reinterpret_cast<const HashLength*>(Pointer);
			for (uint64 Index = 0; Index < NbBlocks; ++Index)
			{
				K = Blocks[Index];

				K *= PrimeA;
				K = RotateLeft(K, 15);
				K *= PrimeB;

				Accumulator ^= K;
				Accumulator = RotateLeft(Accumulator, 13);
				Accumulator = Accumulator * 5 + PrimeC;
			}

			K = 0;
			const uint8* Tail = reinterpret_cast<const uint8*>(Pointer + NbBlocks * 4);
			switch (Length & 3)
			{
#pragma warning(push)
#pragma warning(disable: 26819)
			case 3: K ^= Tail[2] << 16;
			case 2: K ^= Tail[1] << 8;
			case 1: K ^= Tail[0];
#pragma warning(pop)

				K *= PrimeA;
				K = RotateLeft(K, 15);
				K *= PrimeB;
				Accumulator ^= K;
			};

			return *this;
		}

		Murmur32::HashLength Murmur32::Hash() const
		{
			HashLength Result = Accumulator;
			Result ^= Size;
			Result = Avalanche(Result);
			return Finalize(Result);
		}

		Murmur32::HashLength Murmur32::Combine(HashLength HashA, HashLength HashB) const
		{
			HashLength HashCombined = HashA;
			HashCombined ^= HashB + 0x9e3779b9 + (HashCombined << 6) + (HashCombined >> 2);
			return Finalize(HashCombined);
		}

		Murmur32::HashLength Murmur32::RotateLeft(HashLength Value, uint8 Bits)
		{
			return (Value << Bits) | (Value >> (32 - Bits));
		}

		Murmur32::HashLength Murmur32::Avalanche(HashLength Accumulator)
		{
			Accumulator ^= Accumulator >> 16;
			Accumulator *= PrimeD;
			Accumulator ^= Accumulator >> 13;
			Accumulator *= PrimeE;
			Accumulator ^= Accumulator >> 16;
			return Accumulator;
		}

		//-----------------------------------------------------------------------------------------------------------------------
		//	Fnv164
		//-----------------------------------------------------------------------------------------------------------------------

		Fnv164::Fnv164(HashLength Seed)
			: HashAlgorithm<HashLength>(Seed), Seed(Seed), Size(0), Accumulator(0)
		{
			Accumulator = Basis + Seed;
		}

		Fnv164& Fnv164::Accumulate(const void* Data, uint64 Length)
		{
			NEXUS_ASSERT(Data && Length > 0, Default, "Invalid Data");

			Size += Length;

			const uint8* Pointer = reinterpret_cast<const uint8*>(Data);
			for (uint64 Index = 0; Index < Length; ++Index)
			{
				Accumulator *= Prime;
				Accumulator ^= static_cast<HashLength>(Pointer[Index]);
			}

			return *this;
		}

		Fnv164::HashLength Fnv164::Hash() const
		{
			return Finalize(Accumulator);
		}

		Fnv164::HashLength Fnv164::Combine(HashLength HashA, HashLength HashB) const
		{
			HashLength CombinedHash = Basis;

			CombinedHash *= Prime;
			CombinedHash ^= HashA;

			CombinedHash *= Prime;
			CombinedHash ^= HashB;

			return Finalize(CombinedHash);
		}

		//-----------------------------------------------------------------------------------------------------------------------
		// Fnv1a64
		//-----------------------------------------------------------------------------------------------------------------------

		Fnv1a64::Fnv1a64(HashLength Seed)
			: HashAlgorithm<HashLength>(Seed), Seed(Seed), Size(0), Accumulator(0)
		{
			Accumulator = Basis + Seed;
		}

		Fnv1a64& Fnv1a64::Accumulate(const void* Data, uint64 Length)
		{
			NEXUS_ASSERT(Data && Length > 0, Default, "Invalid Data");

			Size += Length;

			const uint8* Pointer = reinterpret_cast<const uint8*>(Data);
			for (uint64 Index = 0; Index < Length; ++Index)
			{
				Accumulator ^= static_cast<HashLength>(Pointer[Index]);
				Accumulator *= Prime;
			}

			return *this;
		}

		Fnv1a64::HashLength Fnv1a64::Hash() const
		{
			return Finalize(Accumulator);
		}

		Fnv1a64::HashLength Fnv1a64::Combine(HashLength HashA, HashLength HashB) const
		{
			HashLength CombinedHash = Basis;

			CombinedHash ^= HashA;
			CombinedHash *= Prime;

			CombinedHash ^= HashB;
			CombinedHash *= Prime;

			return Finalize(CombinedHash);
		}
	}
}
