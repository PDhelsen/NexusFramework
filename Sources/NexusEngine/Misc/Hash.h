#pragma once

#include "External/Intrinsics.h"
#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Debug/Assert.h"

namespace NxEn
{
	class XxHash64;

	template<typename T = const void*, class H = XxHash64>
	struct Hash
	{
		static H::HashLength HashObject(const T& Data, H::HashLength Seed = 0)
		{
			return H::Hash(&Data, sizeof(T), Seed);
		}

		static H::HashLength HashData(const void* Data, uint64 Length, H::HashLength Seed = 0)
		{
			return H::Hash(Data, Length, Seed);
		}

		static H::HashLength Combine(H::HashLength HashA, H::HashLength HashB)
		{
			return H::Combine(HashA, HashB);
		}
	};

	template<typename T>
	class HashFunction
	{
	public:
		using HashLength = T;

		NEXUS_ENGINE_API HashFunction(T Seed) {};

		//NEXUS_ENGINE_API static HashLength Hash(const void* Data, uint64 Length, HashLength Seed = 0) = 0;
		//NEXUS_ENGINE_API static HashLength Combine(HashLength HashA, HashLength HashB) = 0;

		NEXUS_ENGINE_API virtual HashFunction& Accumulate(const void* Data, uint64 Length) = 0;
		NEXUS_ENGINE_API virtual HashLength Hash() const = 0;

		NEXUS_ENGINE_API virtual uint64 GetSize() const = 0;
		NEXUS_ENGINE_API virtual HashLength GetSeed() const = 0;
	};

	class XxHash32 : public HashFunction<uint32>
	{
	public:
		NEXUS_ENGINE_API XxHash32(HashLength Seed = 0);

		NEXUS_ENGINE_API static HashLength Hash(const void* Data, uint64 Length, HashLength Seed = 0);
		NEXUS_ENGINE_API static HashLength Combine(HashLength HashA, HashLength HashB);

		NEXUS_ENGINE_API XxHash32& Accumulate(const void* Data, uint64 Length) override;
		NEXUS_ENGINE_API HashLength Hash() const override;

		NEXUS_ENGINE_API uint64 GetSize() const override { return Size; }
		NEXUS_ENGINE_API HashLength GetSeed() const override { return Seed; }

	private:
		NEXUS_FORCE_INLINE static const uint8* CopyIntoBuffer(uint8* Buffer, const uint8* Data, uint8 Length, uint8& BufferIndex);
		NEXUS_FORCE_INLINE static void Process(const void* Data, HashLength& State0, HashLength& State1, HashLength& State2, HashLength& State3);
		NEXUS_FORCE_INLINE static HashLength Remains(HashLength Result, const uint8* Data, const uint8* Stop);

		NEXUS_FORCE_INLINE static HashLength RotateLeft(HashLength Value, uint8 Bits);
		NEXUS_FORCE_INLINE static HashLength Process(HashLength Value, HashLength Data);
		NEXUS_FORCE_INLINE static HashLength Converge(HashLength Seed, HashLength Size, const HashLength* State);
		NEXUS_FORCE_INLINE static HashLength Avalanche(HashLength Accumulator);

		static const HashLength PrimeA = 0x9E3779B1U;  // 0b10011110001101110111100110110001
		static const HashLength PrimeB = 0x85EBCA77U;  // 0b10000101111010111100101001110111
		static const HashLength PrimeC = 0xC2B2AE3DU;  // 0b11000010101100101010111000111101
		static const HashLength PrimeD = 0x27D4EB2FU;  // 0b00100111110101001110101100101111
		static const HashLength PrimeE = 0x165667B1U;  // 0b00010110010101100110011110110001
		static const uint8 MaxBufferSize = 16;

		uint64 Size;
		HashLength Seed;
		HashLength State[4];
		uint8 Buffer[MaxBufferSize];
		uint8 BufferSize;
	};

	class XxHash64 : public HashFunction<uint64>
	{
	public:
		NEXUS_ENGINE_API XxHash64(HashLength Seed = 0);

		NEXUS_ENGINE_API static HashLength Hash(const void* Data, uint64 Length, HashLength Seed = 0);
		NEXUS_ENGINE_API static HashLength Combine(HashLength HashA, HashLength HashB);

		NEXUS_ENGINE_API XxHash64& Accumulate(const void* Data, uint64 Length);
		NEXUS_ENGINE_API HashLength Hash() const;

		NEXUS_ENGINE_API uint64 GetSize() const { return Size; }
		NEXUS_ENGINE_API HashLength GetSeed() const { return Seed; }

	private:
		NEXUS_FORCE_INLINE static const uint8* CopyIntoBuffer(uint8* Buffer, const uint8* Data, uint8 Length, uint8& BufferIndex);
		NEXUS_FORCE_INLINE static void Process(const void* Data, HashLength& State0, HashLength& State1, HashLength& State2, HashLength& State3);
		NEXUS_FORCE_INLINE static HashLength Remains(HashLength Result, const uint8* Data, const uint8* Stop);

		NEXUS_FORCE_INLINE static HashLength RotateLeft(HashLength Value, uint8 Bits);
		NEXUS_FORCE_INLINE static HashLength Process(HashLength Value, HashLength Data);
		NEXUS_FORCE_INLINE static HashLength Converge(HashLength Seed, HashLength Size, const HashLength* State);
		NEXUS_FORCE_INLINE static HashLength Merge(HashLength Value, HashLength Accumulator);
		NEXUS_FORCE_INLINE static HashLength Avalanche(HashLength Accumulator);

		static const HashLength PrimeA = 0x9E3779B185EBCA87ULL;  // 0b1001111000110111011110011011000110000101111010111100101010000111
		static const HashLength PrimeB = 0xC2B2AE3D27D4EB4FULL;  // 0b1100001010110010101011100011110100100111110101001110101101001111
		static const HashLength PrimeC = 0x165667B19E3779F9ULL;  // 0b0001011001010110011001111011000110011110001101110111100111111001
		static const HashLength PrimeD = 0x85EBCA77C2B2AE63ULL;  // 0b1000010111101011110010100111011111000010101100101010111001100011
		static const HashLength PrimeE = 0x27D4EB2F165667C5ULL;  // 0b0010011111010100111010110010111100010110010101100110011111000101
		static const uint8 MaxBufferSize = 32;

		uint64 Size;
		HashLength Seed;
		HashLength State[4];
		uint8 Buffer[MaxBufferSize];
		uint8 BufferSize;
	};
}

