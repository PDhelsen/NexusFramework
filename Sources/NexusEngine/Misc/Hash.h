#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Debug/Assert.h"

namespace NxEn
{
	class Hash64;

	template<typename T = void*, class H = Hash64>
	struct Hasher
	{
		static H::HashLength Hash(const T& Data, H::HashLength Seed = 0)
		{
			return H::Hash(&Data, sizeof(T), Seed);
		}

		static H::HashLength Hash(const void* Data, uint64 Length, H::HashLength Seed = 0)
		{
			return H::Hash(Data, Length, Seed);
		}

		static H::HashLength Combine(H::HashLength HashA, H::HashLength HashB)
		{
			constexpr H::HashLength Magic = sizeof(H::HashLength) > 4 ? 0x9e3779b97f4a7c15 : 0x9e3779b9;

			typename H::HashLength HashCombined = HashA;
			HashCombined ^= HashB + Magic + (HashCombined << 6) + (HashCombined >> 2);
			return HashCombined;
		}
	};

	class Hash32
	{
	public:
		using HashLength = uint32;

		NEXUS_ENGINE_API Hash32(uint32 Seed = 0);

		NEXUS_ENGINE_API static uint32 Hash(const void* Data, uint64 Length, uint32 Seed = 0);

		NEXUS_ENGINE_API Hash32& Accumulate(const void* Data, uint64 Length);
		NEXUS_ENGINE_API uint32 Hash() const;

		NEXUS_ENGINE_API uint64 GetSize() const { return Size; }
		NEXUS_ENGINE_API uint32 GetSeed() const { return Seed; }

	private:
		static inline const uint8* CopyIntoBuffer(uint8* Buffer, const uint8* Data, uint8 Length, uint8& BufferIndex);
		static inline void Process(const void* Data, uint32& State0, uint32& State1, uint32& State2, uint32& State3);
		static inline uint32 Remains(uint32 Result, const uint8* Data, const uint8* Stop);

		static inline uint32 RotateLeft(uint32 Value, uint8 Bits);
		static inline uint32 Process(uint32 Value, uint32 Data);
		static inline uint32 Converge(uint32 Seed, uint32 Size, const uint32* State);
		static inline uint32 Avalanche(uint32 Accumulator);

		static const uint32 PrimeA = 0x9E3779B1U;  // 0b10011110001101110111100110110001
		static const uint32 PrimeB = 0x85EBCA77U;  // 0b10000101111010111100101001110111
		static const uint32 PrimeC = 0xC2B2AE3DU;  // 0b11000010101100101010111000111101
		static const uint32 PrimeD = 0x27D4EB2FU;  // 0b00100111110101001110101100101111
		static const uint32 PrimeE = 0x165667B1U;  // 0b00010110010101100110011110110001
		static const uint8 MaxBufferSize = 16;

		uint64 Size;
		uint32 Seed;
		uint32 State[4];
		uint8 Buffer[MaxBufferSize];
		uint8 BufferSize;
	};

	class Hash64
	{
	public:
		using HashLength = uint64;
		
		NEXUS_ENGINE_API Hash64(uint64 Seed = 0);

		NEXUS_ENGINE_API static uint64 Hash(const void* Data, uint64 Length, uint64 Seed = 0);

		NEXUS_ENGINE_API Hash64& Accumulate(const void* Data, uint64 Length);
		NEXUS_ENGINE_API uint64 Hash() const;

		NEXUS_ENGINE_API uint64 GetSize() const { return Size; }
		NEXUS_ENGINE_API uint64 GetSeed() const { return Seed; }

	private:
		static inline const uint8* CopyIntoBuffer(uint8* Buffer, const uint8* Data, uint8 Length, uint8& BufferIndex);
		static inline void Process(const void* Data, uint64& State0, uint64& State1, uint64& State2, uint64& State3);
		static inline uint64 Remains(uint64 Result, const uint8* Data, const uint8* Stop);

		static inline uint64 RotateLeft(uint64 Value, uint8 Bits);
		static inline uint64 Process(uint64 Value, uint64 Data);
		static inline uint64 Converge(uint64 Seed, uint64 Size, const uint64* State);
		static inline uint64 Merge(uint64 Value, uint64 Accumulator);
		static inline uint64 Avalanche(uint64 Accumulator);

		static const uint64 PrimeA = 0x9E3779B185EBCA87ULL;  // 0b1001111000110111011110011011000110000101111010111100101010000111
		static const uint64 PrimeB = 0xC2B2AE3D27D4EB4FULL;  // 0b1100001010110010101011100011110100100111110101001110101101001111
		static const uint64 PrimeC = 0x165667B19E3779F9ULL;  // 0b0001011001010110011001111011000110011110001101110111100111111001
		static const uint64 PrimeD = 0x85EBCA77C2B2AE63ULL;  // 0b1000010111101011110010100111011111000010101100101010111001100011
		static const uint64 PrimeE = 0x27D4EB2F165667C5ULL;  // 0b0010011111010100111010110010111100010110010101100110011111000101
		static const uint8 MaxBufferSize = 32;

		uint64 Size;
		uint64 Seed;
		uint64 State[4];
		uint8 Buffer[MaxBufferSize];
		uint8 BufferSize;
	};
}

