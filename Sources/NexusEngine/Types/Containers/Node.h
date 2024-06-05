#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	struct LinkedNodeSimple
	{
		T Value;
		LinkedNodeSimple* Next;
	};

	template<typename T>
	struct LinkedNodeDouble
	{
		T Value;
		LinkedNodeDouble* Next;
		LinkedNodeDouble* Prev;
	};

	template<typename T>
	struct LinkedNodeTree
	{
		T Value;
		uint64 Count;
		LinkedNodeTree* Parent;
		LinkedNodeTree* Sibling;
		LinkedNodeTree* Child;
	};

	template<typename T>
	struct LinkedConnectionGraph;
	
	enum class LinkedConnectionType : uint8
	{
		From, To
	};

	template<typename T>
	struct LinkedNodeGraph
	{
		T Value;
		uint64 Count;
		LinkedNodeGraph<T>* Next;
		LinkedConnectionGraph<T>* Connection;
	};

	template<typename T>
	struct LinkedConnectionGraph
	{
		LinkedConnectionType Type;
		LinkedNodeGraph<T>* Target;
		LinkedConnectionGraph<T>* Next;
	};

	template<typename K, typename T>
	class KeyValuePair
	{
	public:
		KeyValuePair(const K& Key, const T& Value)
			: Key(Key), Value(Value)
		{

		}

		KeyValuePair(const K& Key, T&& Value)
			: Key(Key), Value(Move(Value))
		{

		}

		void Initialize(const K& InitialKey, const T& InitialValue)
		{
			Key = InitialKey;
			Value = InitialValue;
		}

		void Initialize(const K& InitialKey, T&& InitialValue)
		{
			Key = InitialKey;
			Value = Move(InitialValue);
		}

		template<typename... Args>
		void Initialize(const K& InitialKey, Args&&... args)
		{
			Key = InitialKey;
			Memory::Construct<T>(&Value, args...);
		}

		void SetValue(const T& Other) { Value = Other; }
		void SetValue(T&& Other) { Value = Move(Other); }
		template<typename... Args>
		void SetValue(Args&&... args) { Memory::Construct<T>(&Value, args...); }

		const K& GetKey() const { return Key; }
		T& GetValue() { return Value; }

	private:
		K Key;
		T Value;
	};
}