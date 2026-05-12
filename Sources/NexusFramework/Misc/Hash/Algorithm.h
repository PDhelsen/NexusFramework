#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"

namespace NxFr
{
	namespace Hashing
	{
		template<typename L>
		class NEXUS_FRAMEWORK_API HashAlgorithm
		{
		public:
			using HashLength = L;

			HashAlgorithm(HashLength Seed) {};

			virtual HashAlgorithm& Accumulate(const void* Data, uint64 Length) = 0;
			virtual HashLength Finalize() const = 0;
			virtual HashLength Combine(HashLength HashA, HashLength HashB) const = 0;

			virtual uint64 GetSize() const = 0;
			virtual HashLength GetSeed() const = 0;
		};

		class NEXUS_FRAMEWORK_API XxHash32 : public HashAlgorithm<uint32>
		{
		public:
			XxHash32(HashLength Seed = 0);

			XxHash32& Accumulate(const void* Data, uint64 Length) override;
			HashLength Finalize() const override;
			HashLength Combine(HashLength HashA, HashLength HashB) const override;

			uint64 GetSize() const override { return Size; }
			HashLength GetSeed() const override { return Seed; }

		private:
			inline static const uint8* CopyIntoBuffer(uint8* Buffer, const uint8* Data, uint8 Length, uint8& BufferIndex);
			inline static void Process(const void* Data, HashLength& State0, HashLength& State1, HashLength& State2, HashLength& State3);
			inline static HashLength Remains(HashLength Result, const uint8* Data, const uint8* Stop);

			inline static HashLength RotateLeft(HashLength Value, uint8 Bits);
			inline static HashLength Process(HashLength Value, HashLength Data);
			inline static HashLength Converge(HashLength Seed, HashLength Size, const HashLength* State);
			inline static HashLength Avalanche(HashLength Accumulator);

			static const HashLength PrimeA = 0x9E3779B1U;
			static const HashLength PrimeB = 0x85EBCA77U;
			static const HashLength PrimeC = 0xC2B2AE3DU;
			static const HashLength PrimeD = 0x27D4EB2FU;
			static const HashLength PrimeE = 0x165667B1U;
			static const uint8 MaxBufferSize = 16;

			uint64 Size;
			HashLength Seed;
			HashLength State[4];
			uint8 Buffer[MaxBufferSize];
			uint8 BufferSize;
		};

		class NEXUS_FRAMEWORK_API XxHash64 : public HashAlgorithm<uint64>
		{
		public:
			XxHash64(HashLength Seed = 0);

			XxHash64& Accumulate(const void* Data, uint64 Length);
			HashLength Finalize() const;
			HashLength Combine(HashLength HashA, HashLength HashB) const override;

			uint64 GetSize() const { return Size; }
			HashLength GetSeed() const { return Seed; }

		private:
			inline static const uint8* CopyIntoBuffer(uint8* Buffer, const uint8* Data, uint8 Length, uint8& BufferIndex);
			inline static void Process(const void* Data, HashLength& State0, HashLength& State1, HashLength& State2, HashLength& State3);
			inline static HashLength Remains(HashLength Result, const uint8* Data, const uint8* Stop);

			inline static HashLength RotateLeft(HashLength Value, uint8 Bits);
			inline static HashLength Process(HashLength Value, HashLength Data);
			inline static HashLength Converge(HashLength Seed, HashLength Size, const HashLength* State);
			inline static HashLength Merge(HashLength Value, HashLength Accumulator);
			inline static HashLength Avalanche(HashLength Accumulator);

			static const HashLength PrimeA = 0x9E3779B185EBCA87ULL;
			static const HashLength PrimeB = 0xC2B2AE3D27D4EB4FULL;
			static const HashLength PrimeC = 0x165667B19E3779F9ULL;
			static const HashLength PrimeD = 0x85EBCA77C2B2AE63ULL;
			static const HashLength PrimeE = 0x27D4EB2F165667C5ULL;
			static const uint8 MaxBufferSize = 32;

			uint64 Size;
			HashLength Seed;
			HashLength State[4];
			uint8 Buffer[MaxBufferSize];
			uint8 BufferSize;
		};

		class NEXUS_FRAMEWORK_API Murmur32 : public HashAlgorithm<uint32>
		{
		public:
			Murmur32(HashLength Seed = 0);

			Murmur32& Accumulate(const void* Data, uint64 Length);
			HashLength Finalize() const;
			HashLength Combine(HashLength HashA, HashLength HashB) const override;

			uint64 GetSize() const { return Size; }
			HashLength GetSeed() const { return Seed; }

		private:
			inline static HashLength RotateLeft(HashLength Value, uint8 Bits);
			inline static HashLength Avalanche(HashLength Accumulator);

			static const HashLength PrimeA = 0xcc9e2d51;
			static const HashLength PrimeB = 0x1b873593;
			static const HashLength PrimeC = 0xe6546b64;
			static const HashLength PrimeD = 0x85ebca6b;
			static const HashLength PrimeE = 0xc2b2ae35;

			uint64 Size;
			HashLength Seed;
			HashLength Accumulator;
		};

		class NEXUS_FRAMEWORK_API Fnv164 : public HashAlgorithm<uint64>
		{
		public:
			Fnv164(HashLength Seed = 0);

			Fnv164& Accumulate(const void* Data, uint64 Length);
			HashLength Finalize() const;
			HashLength Combine(HashLength HashA, HashLength HashB) const override;

			uint64 GetSize() const { return Size; }
			HashLength GetSeed() const { return Seed; }

		private:
			static const HashLength Basis = 0xcbf29ce484222325;
			static const HashLength Prime = 0x100000001b3;

			uint64 Size;
			HashLength Seed;
			HashLength Accumulator;
		};

		class NEXUS_FRAMEWORK_API Fnv1a64 : public HashAlgorithm<uint64>
		{
		public:
			Fnv1a64(HashLength Seed = 0);

			Fnv1a64& Accumulate(const void* Data, uint64 Length);
			HashLength Finalize() const;
			HashLength Combine(HashLength HashA, HashLength HashB) const override;

			uint64 GetSize() const { return Size; }
			HashLength GetSeed() const { return Seed; }

		private:
			static const HashLength Basis = 0xcbf29ce484222325;
			static const HashLength Prime = 0x100000001b3;

			uint64 Size;
			HashLength Seed;
			HashLength Accumulator;
		};
	}
}
