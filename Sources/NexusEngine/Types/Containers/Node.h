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

		KeyValuePair(K&& Key, T&& Value)
			: Key(Move(Key)), Value(Move(Value))
		{

		}

		template<typename... Args>
		KeyValuePair(K&& Key, Args&&... args)
			: Key(Move(Key)), Value(args...)
		{

		}

		void SetValue(const T& Other) { Value = Other; }
		void SetValue(T&& Other) { Value = Move(Other); }
		template<typename... Args>
		void SetValue(Args&&... args)
		{
			Memory::Destruct(&Value);
			Memory::Construct<T>(&Value, args...);
		}

		const K& GetKey() const { return Key; }
		T& GetValue() { return Value; }

	private:
		K Key;
		T Value;
	};
}
