#pragma once

#include "NexusFramework/Misc/Hash/Algorithm.h"

namespace NxFr
{
	template<typename H>
	struct Hash;

	namespace Hashing
	{
		using Default = class Fnv1a64;

		template<typename T, typename H = Hashing::Default>
		struct Hasher
		{
		public:
			static void Accumulate(Hash<H>& State, const T& Data)
			{
				State.Accumulate(&Data, sizeof(T));
			}
		};
	}

	template<typename H = Hashing::Default>
	struct Hash
	{
	public:
		using HashLength = typename H::HashLength;

		template<typename T>
		static HashLength HashObject(const T& Data, HashLength Seed = 0)
		{
			Hash<H> Instance(Seed);
			Instance.Accumulate(Data);
			return Instance.Finalize();
		}

		static HashLength HashData(const void* Data, uint64 Length, HashLength Seed = 0)
		{
			Hash<H> Instance(Seed);
			Instance.Accumulate(Data, Length);
			return Instance.Finalize();
		}

		static HashLength CombineHashes(HashLength HashA, HashLength HashB, HashLength Seed = 0)
		{
			Hash<H> Instance(Seed);
			return Instance.Combine(HashA, HashB);
		}

		Hash(HashLength Seed = 0)
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

		HashLength Finalize() const
		{
			return State.Finalize();
		}

		HashLength Combine(HashLength A, HashLength B) const
		{
			return State.Combine(A, B);
		}

		HashLength GetSize() const { return State.GetSize(); };
		HashLength GetSeed() const { return State.GetSeed(); };

	private:
		H State;
	};

	namespace Hashing
	{
		template<typename T, typename H>
		struct Hasher<T*, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const T* Data)
			{
				State.Accumulate(reinterpret_cast<uint64>(Data));
			}
		};

		template<typename H>
		struct Hasher<nullptr_t, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const nullptr_t* Data)
			{
				State.Accumulate((uint64)0);
			}
		};
	}
}
