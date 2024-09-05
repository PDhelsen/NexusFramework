#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"

namespace NxEn
{
	class Fnv1a64;

	//-----------------------------------------------------------------------------------------------------------------------
	// Hash Proxy
	//-----------------------------------------------------------------------------------------------------------------------

	// TODO: Architecture - Hash - Strategy Pattern
	// TODO: Architecture - Hash - Hash object instance not only static

	template<typename T = const void*, class H = Fnv1a64>
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

	template<class H>
	struct Hash<bool, H>
	{
		static H::HashLength HashObject(const bool& Data, H::HashLength Seed = 0)
		{
			return Data;
		}
	};

	template<class H>
	struct Hash<char, H>
	{
		static H::HashLength HashObject(const char& Data, H::HashLength Seed = 0)
		{
			return Data;
		}
	};

	template<class H>
	struct Hash<int8, H>
	{
		static H::HashLength HashObject(const int8& Data, H::HashLength Seed = 0)
		{
			return Data;
		}
	};

	template<class H>
	struct Hash<int16, H>
	{
		static H::HashLength HashObject(const int16& Data, H::HashLength Seed = 0)
		{
			return Data;
		}
	};

	template<class H>
	struct Hash<int32, H>
	{
		static H::HashLength HashObject(const int32& Data, H::HashLength Seed = 0)
		{
			return Data;
		}
	};

	template<class H>
	struct Hash<int64, H>
	{
		static H::HashLength HashObject(const int64& Data, H::HashLength Seed = 0)
		{
			return Data;
		}
	};

	template<class H>
	struct Hash<uint8, H>
	{
		static H::HashLength HashObject(const uint8& Data, H::HashLength Seed = 0)
		{
			return Data;
		}
	};

	template<class H>
	struct Hash<uint16, H>
	{
		static H::HashLength HashObject(const uint16& Data, H::HashLength Seed = 0)
		{
			return Data;
		}
	};

	template<class H>
	struct Hash<uint32, H>
	{
		static H::HashLength HashObject(const uint32& Data, H::HashLength Seed = 0)
		{
			return Data;
		}
	};

	template<class H>
	struct Hash<uint64, H>
	{
		static H::HashLength HashObject(const uint64& Data, H::HashLength Seed = 0)
		{
			return Data;
		}
	};

	//-----------------------------------------------------------------------------------------------------------------------
	// Hash Function
	//-----------------------------------------------------------------------------------------------------------------------

	template<typename T>
	class HashFunction
	{
	public:
		using HashLength = T;

		NEXUS_ENGINE_API HashFunction(HashLength Seed) {};

		//NEXUS_ENGINE_API static HashLength Hash(const void* Data, uint64 Length, HashLength Seed = 0) = 0;
		//NEXUS_ENGINE_API static HashLength Combine(HashLength HashA, HashLength HashB) = 0;

		NEXUS_ENGINE_API virtual HashFunction& Accumulate(const void* Data, uint64 Length) = 0;
		NEXUS_ENGINE_API virtual HashLength Hash() const = 0;

		NEXUS_ENGINE_API virtual uint64 GetSize() const = 0;
		NEXUS_ENGINE_API virtual HashLength GetSeed() const = 0;

	protected:
		inline static HashLength Finalize(HashLength Value);
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

	class Murmur32 : public HashFunction<uint32>
	{
	public:
		NEXUS_ENGINE_API Murmur32(HashLength Seed = 0);

		NEXUS_ENGINE_API static HashLength Hash(const void* Data, uint64 Length, HashLength Seed = 0);
		NEXUS_ENGINE_API static HashLength Combine(HashLength HashA, HashLength HashB);

		NEXUS_ENGINE_API Murmur32& Accumulate(const void* Data, uint64 Length);
		NEXUS_ENGINE_API HashLength Hash() const;

		NEXUS_ENGINE_API uint64 GetSize() const { return Size; }
		NEXUS_ENGINE_API HashLength GetSeed() const { return Seed; }

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

	class Fnv164 : public HashFunction<uint64>
	{
	public:
		NEXUS_ENGINE_API Fnv164(HashLength Seed = 0);

		NEXUS_ENGINE_API static HashLength Hash(const void* Data, uint64 Length, HashLength Seed = 0);
		NEXUS_ENGINE_API static HashLength Combine(HashLength HashA, HashLength HashB);

		NEXUS_ENGINE_API Fnv164& Accumulate(const void* Data, uint64 Length);
		NEXUS_ENGINE_API HashLength Hash() const;

		NEXUS_ENGINE_API uint64 GetSize() const { return Size; }
		NEXUS_ENGINE_API HashLength GetSeed() const { return Seed; }

	private:
		static const HashLength Basis = 0xcbf29ce484222325;
		static const HashLength Prime = 0x100000001b3;

		uint64 Size;
		HashLength Seed;
		HashLength Accumulator;
	};

	class Fnv1a64 : public HashFunction<uint64>
	{
	public:
		NEXUS_ENGINE_API Fnv1a64(HashLength Seed = 0);

		NEXUS_ENGINE_API static HashLength Hash(const void* Data, uint64 Length, HashLength Seed = 0);
		NEXUS_ENGINE_API static HashLength Combine(HashLength HashA, HashLength HashB);

		NEXUS_ENGINE_API Fnv1a64& Accumulate(const void* Data, uint64 Length);
		NEXUS_ENGINE_API HashLength Hash() const;

		NEXUS_ENGINE_API uint64 GetSize() const { return Size; }
		NEXUS_ENGINE_API HashLength GetSeed() const { return Seed; }

	private:
		static const HashLength Basis = 0xcbf29ce484222325;
		static const HashLength Prime = 0x100000001b3;

		uint64 Size;
		HashLength Seed;
		HashLength Accumulator;
	};

	template<typename T>
	inline HashFunction<T>::HashLength HashFunction<T>::Finalize(HashFunction<T>::HashLength Value)
	{
		Value |= Value == 0;
		return Value;
	}
}

