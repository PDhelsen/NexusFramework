#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{

#pragma region Hash Algorithm

	template<typename H>
	struct Hash;

	namespace Hashing
	{
		using Default = class Fnv1a64;

		template<typename L>
		class HashAlgorithm
		{
		public:
			using HashLength = L;

			NEXUS_FRAMEWORK_API HashAlgorithm(HashLength Seed) {};

			NEXUS_FRAMEWORK_API virtual HashAlgorithm& Accumulate(const void* Data, uint64 Length) = 0;
			NEXUS_FRAMEWORK_API virtual HashLength Finalize() const = 0;
			NEXUS_FRAMEWORK_API virtual HashLength Combine(HashLength HashA, HashLength HashB) const = 0;

			NEXUS_FRAMEWORK_API virtual uint64 GetSize() const = 0;
			NEXUS_FRAMEWORK_API virtual HashLength GetSeed() const = 0;
		};

		class XxHash32 : public HashAlgorithm<uint32>
		{
		public:
			NEXUS_FRAMEWORK_API XxHash32(HashLength Seed = 0);

			NEXUS_FRAMEWORK_API XxHash32& Accumulate(const void* Data, uint64 Length) override;
			NEXUS_FRAMEWORK_API HashLength Finalize() const override;
			NEXUS_FRAMEWORK_API HashLength Combine(HashLength HashA, HashLength HashB) const override;

			NEXUS_FRAMEWORK_API uint64 GetSize() const override { return Size; }
			NEXUS_FRAMEWORK_API HashLength GetSeed() const override { return Seed; }

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

		class XxHash64 : public HashAlgorithm<uint64>
		{
		public:
			NEXUS_FRAMEWORK_API XxHash64(HashLength Seed = 0);

			NEXUS_FRAMEWORK_API XxHash64& Accumulate(const void* Data, uint64 Length);
			NEXUS_FRAMEWORK_API HashLength Finalize() const;
			NEXUS_FRAMEWORK_API HashLength Combine(HashLength HashA, HashLength HashB) const override;

			NEXUS_FRAMEWORK_API uint64 GetSize() const { return Size; }
			NEXUS_FRAMEWORK_API HashLength GetSeed() const { return Seed; }

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

		class Murmur32 : public HashAlgorithm<uint32>
		{
		public:
			NEXUS_FRAMEWORK_API Murmur32(HashLength Seed = 0);

			NEXUS_FRAMEWORK_API Murmur32& Accumulate(const void* Data, uint64 Length);
			NEXUS_FRAMEWORK_API HashLength Finalize() const;
			NEXUS_FRAMEWORK_API HashLength Combine(HashLength HashA, HashLength HashB) const override;

			NEXUS_FRAMEWORK_API uint64 GetSize() const { return Size; }
			NEXUS_FRAMEWORK_API HashLength GetSeed() const { return Seed; }

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

		class Fnv164 : public HashAlgorithm<uint64>
		{
		public:
			NEXUS_FRAMEWORK_API Fnv164(HashLength Seed = 0);

			NEXUS_FRAMEWORK_API Fnv164& Accumulate(const void* Data, uint64 Length);
			NEXUS_FRAMEWORK_API HashLength Finalize() const;
			NEXUS_FRAMEWORK_API HashLength Combine(HashLength HashA, HashLength HashB) const override;

			NEXUS_FRAMEWORK_API uint64 GetSize() const { return Size; }
			NEXUS_FRAMEWORK_API HashLength GetSeed() const { return Seed; }

		private:
			static const HashLength Basis = 0xcbf29ce484222325;
			static const HashLength Prime = 0x100000001b3;

			uint64 Size;
			HashLength Seed;
			HashLength Accumulator;
		};

		class Fnv1a64 : public HashAlgorithm<uint64>
		{
		public:
			NEXUS_FRAMEWORK_API Fnv1a64(HashLength Seed = 0);

			NEXUS_FRAMEWORK_API Fnv1a64& Accumulate(const void* Data, uint64 Length);
			NEXUS_FRAMEWORK_API HashLength Finalize() const;
			NEXUS_FRAMEWORK_API HashLength Combine(HashLength HashA, HashLength HashB) const override;

			NEXUS_FRAMEWORK_API uint64 GetSize() const { return Size; }
			NEXUS_FRAMEWORK_API HashLength GetSeed() const { return Seed; }

		private:
			static const HashLength Basis = 0xcbf29ce484222325;
			static const HashLength Prime = 0x100000001b3;

			uint64 Size;
			HashLength Seed;
			HashLength Accumulator;
		};
	}

#pragma endregion

#pragma region Hash Template Specialization

	namespace Hashing
	{
		template<typename T, typename H = Hashing::Default>
		struct Hasher
		{
		public:
			static void Accumulate(Hash<H>& State, const T& Data)
			{
				State.Accumulate(&Data, sizeof(T));
			}
		};

		template<typename T, typename H>
		class Hasher<T*, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const T* Data)
			{
				State.Accumulate(reinterpret_cast<uint64>(Data));
			}
		};

		template<typename H>
		class Hasher<nullptr_t, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const nullptr_t* Data)
			{
				State.Accumulate((uint64)0);
			}
		};
	}

#pragma endregion

	template<typename H = Hashing::Default>
	struct Hash
	{
	public:
		template<typename T>
		static typename H::HashLength HashObject(const T& Data, typename H::HashLength Seed = 0)
		{
			Hash<H> Instance(Seed);
			Instance.Accumulate(Data);
			return Instance.Finalize();
		}
		
		static typename H::HashLength HashData(const void* Data, uint64 Length, typename H::HashLength Seed = 0)
		{
			Hash<H> Instance(Seed);
			Instance.Accumulate(Data, Length);
			return Instance.Finalize();
		}
		
		static typename H::HashLength CombineHashes(typename H::HashLength HashA, typename H::HashLength HashB, typename H::HashLength Seed = 0)
		{
			Hash<H> Instance(Seed);
			return Instance.Combine(HashA, HashB);
		}

		Hash(typename H::HashLength Seed = 0)
			: State(Seed)
		{
		}

		template<typename T>
		Hash<H>& Accumulate(const T& Data)
		{
			Hashing::Hasher<T, H>::Accumulate(*this, Data);
			return *this;
		}

		Hash<H>& Accumulate(const void* Data, uint64 Length)
		{
			State.Accumulate(Data, Length);
			return *this;
		}

		typename H::HashLength Finalize() const
		{
			return State.Finalize();
		}

		typename H::HashLength Combine(typename H::HashLength A, typename H::HashLength B) const
		{
			return State.Combine(A, B);
		}

		typename H::HashLength GetSize() const { return State.GetSize(); };
		typename H::HashLength GetSeed() const { return State.GetSeed(); };

	private:
		H State;
	};

}
