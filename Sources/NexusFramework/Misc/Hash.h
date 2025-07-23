#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"

namespace NxFr
{

#pragma region Hash Algorithm

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
			NEXUS_FRAMEWORK_API virtual HashLength Hash() const = 0;

			NEXUS_FRAMEWORK_API virtual HashLength Combine(HashLength HashA, HashLength HashB) const = 0;

			NEXUS_FRAMEWORK_API virtual uint64 GetSize() const = 0;
			NEXUS_FRAMEWORK_API virtual HashLength GetSeed() const = 0;

			NEXUS_FRAMEWORK_API static HashLength Finalize(HashLength Value)
			{
				Value -= Value == 0;
				return Value;
			}
		};

		class XxHash32 : public HashAlgorithm<uint32>
		{
		public:
			NEXUS_FRAMEWORK_API XxHash32(HashLength Seed = 0);

			NEXUS_FRAMEWORK_API XxHash32& Accumulate(const void* Data, uint64 Length) override;
			NEXUS_FRAMEWORK_API HashLength Hash() const override;

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
			NEXUS_FRAMEWORK_API HashLength Hash() const;

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
			NEXUS_FRAMEWORK_API HashLength Hash() const;

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
			NEXUS_FRAMEWORK_API HashLength Hash() const;

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
			NEXUS_FRAMEWORK_API HashLength Hash() const;

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

#pragma region Hash Back-end

	namespace Hashing
	{
		template<typename H = Hashing::Default>
		struct HashStrategy
		{
		public:
			HashStrategy(typename H::HashLength Seed = 0)
				: State(Seed)
			{
			}

			HashStrategy<H>& Accumulate(const void* Data, uint64 Length)
			{
				State.Accumulate(Data, Length);
				return *this;
			}

			typename H::HashLength Hash() const
			{
				return State.Hash();
			}

			typename H::HashLength Combine(typename H::HashLength A, typename H::HashLength B) const
			{
				return State.Combine(A, B);
			}

			uint64 GetSize() const { return State.GetSize(); };
			uint64 GetSeed() const { return State.GetSeed(); };

		private:
			H State;
		};

		template<typename T = void*, typename H = Hashing::Default>
		class HashProcess
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const T& Data)
			{
				State.Accumulate(&Data, sizeof(T));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const T& Data)
			{
				HashProcess<T, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};
	}

#pragma endregion

#pragma region Hash Front-end

	template<typename H = Hashing::Default>
	struct Hasher
	{
	public:
		Hasher(typename H::HashLength Seed = 0)
			: State(Seed)
		{
		}

		template<typename T>
		Hasher<H>& Accumulate(const T& Data)
		{
			Hashing::HashProcess<T, H>::Accumulate(State, Data);
			return *this;
		}

		Hasher<H>& Accumulate(const void* Data, uint64 Length)
		{
			State.Accumulate(Data, Length);
			return *this;
		}

		typename H::HashLength Hash() const
		{
			return State.Hash();
		}

		typename H::HashLength Combine(typename H::HashLength A, typename H::HashLength B) const
		{
			return State.Combine(A, B);
		}

		uint64 GetSize() const { return State.GetSize(); };
		uint64 GetSeed() const { return State.GetSeed(); };

	private:
		Hashing::HashStrategy<H> State;
	};

	template<typename H = Hashing::Default>
	class Hash
	{
	public:
		template<typename T>
		static typename H::HashLength HashObject(const T& Data, typename H::HashLength Seed = 0)
		{
			Hashing::HashStrategy<H> State(Seed);
			return Hashing::HashProcess<T, H>::Hash(State, Data);
		}

		static typename H::HashLength HashData(const void* Data, uint64 Length, typename H::HashLength Seed = 0)
		{
			Hashing::HashStrategy<H> State(Seed);
			State.Accumulate(Data, Length);
			return State.Hash();
		}

		static typename H::HashLength Combine(typename H::HashLength HashA, typename H::HashLength HashB, typename H::HashLength Seed = 0)
		{
			Hashing::HashStrategy<H> State(Seed);
			return State.Combine(HashA, HashB);
		}
	};

#pragma endregion

#pragma region Hash Template Specialization

	namespace Hashing
	{
		template<typename H>
		class HashProcess<bool, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, bool Data)
			{
				State.Accumulate(&Data, sizeof(bool));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, bool Data)
			{
				return HashAlgorithm<typename H::HashLength>::Finalize(Data);
			}
		};

		template<typename H>
		class HashProcess<char, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, char Data)
			{
				State.Accumulate(&Data, sizeof(char));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, char Data)
			{
				return HashAlgorithm<typename H::HashLength>::Finalize(Data);
			}
		};

		template<typename H>
		class HashProcess<int8, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, int8 Data)
			{
				State.Accumulate(&Data, sizeof(int8));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, int8 Data)
			{
				return HashAlgorithm<typename H::HashLength>::Finalize(Data);
			}
		};

		template<typename H>
		class HashProcess<int16, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, int16 Data)
			{
				State.Accumulate(&Data, sizeof(int16));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, int16 Data)
			{
				return HashAlgorithm<typename H::HashLength>::Finalize(Data);
			}
		};

		template<typename H>
		class HashProcess<int32, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, int32 Data)
			{
				State.Accumulate(&Data, sizeof(int32));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, int32 Data)
			{
				return HashAlgorithm<typename H::HashLength>::Finalize(Data);
			}
		};

		template<typename H>
		class HashProcess<int64, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, int64 Data)
			{
				State.Accumulate(&Data, sizeof(int64));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, int64 Data)
			{
				return HashAlgorithm<typename H::HashLength>::Finalize(Data);
			}
		};

		template<typename H>
		class HashProcess<uint8, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, uint8 Data)
			{
				State.Accumulate(&Data, sizeof(uint8));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, uint8 Data)
			{
				return HashAlgorithm<typename H::HashLength>::Finalize(Data);
			}
		};

		template<typename H>
		class HashProcess<uint16, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, uint16 Data)
			{
				State.Accumulate(&Data, sizeof(uint16));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, uint16 Data)
			{
				return HashAlgorithm<typename H::HashLength>::Finalize(Data);
			}
		};

		template<typename H>
		class HashProcess<uint32, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, uint32 Data)
			{
				State.Accumulate(&Data, sizeof(uint32));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, uint32 Data)
			{
				return HashAlgorithm<typename H::HashLength>::Finalize(Data);
			}
		};

		template<typename H>
		class HashProcess<uint64, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, uint64 Data)
			{
				State.Accumulate(&Data, sizeof(uint64));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, uint64 Data)
			{
				return HashAlgorithm<typename H::HashLength>::Finalize(Data);
			}
		};

		template<typename H>
		class HashProcess<float, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const float& Data)
			{
				State.Accumulate(&Data, sizeof(float));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const float& Data)
			{
				HashProcess<float, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<double, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const double& Data)
			{
				State.Accumulate(&Data, sizeof(double));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const double& Data)
			{
				HashProcess<double, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<void*, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, void* Data)
			{
				State.Accumulate(&Data, sizeof(void*));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, void* Data)
			{
				typename H::HashLength Reinterpreted = reinterpret_cast<typename H::HashLength>(Data);
				return HashAlgorithm<typename H::HashLength>::Finalize(Reinterpreted);
			}
		};
	}

#pragma endregion

}
