#pragma once

#include "Types/Integer.h"
#include "Memory/Memory.h"
#include "Misc/References.h"

namespace NxEn
{
	template<typename T>
	struct NodeGraphConnection;

	// -------------------------------
	// Nodes
	// -------------------------------

	template<typename T>
	struct NodeSimple
	{
		T Value;
		NodeSimple<T>* Next;
	};

	template<typename T>
	struct NodeDouble
	{
		T Value;
		NodeDouble<T>* Next;
		NodeDouble<T>* Prev;
	};

	template<typename T>
	struct NodeTree
	{
		T Value;
		uint64 Count;
		NodeTree<T>* Parent;
		NodeTree<T>* Sibling;
		NodeTree<T>* Child;
	};

	template<typename T>
	struct NodeGraph
	{
		T Value;
		uint64 Count;
		NodeGraph<T>* Next;
		NodeGraphConnection<T>* Connection;
	};

	// -------------------------------
	// Connections
	// -------------------------------

	enum class NodeGraphConnectionType : uint8
	{
		From, To
	};

	template<typename T>
	struct NodeGraphConnection
	{
		NodeGraph<T>* Target;
		NodeGraphConnection<T>* Next;
		NodeGraphConnectionType Type;
	};

	// -------------------------------
	// Key - Value
	// -------------------------------

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
