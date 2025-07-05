#pragma once

#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	namespace Node
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
		struct NodeHashmap
		{
			T Value;
			uint64 Hash;

			bool IsFree() { return Hash == 0; }
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
		// Utils
		// -------------------------------

		template<typename T, typename N>
		static N* GetNode(T* Value)
		{
			return reinterpret_cast<N*>(Value);
		}

		template<typename T, typename N>
		static const N* GetNode(const T* Value)
		{
			return reinterpret_cast<const N*>(Value);
		}

		template<typename N>
		static void FixupNode(N* Head, N** Tail)
		{
			N* Current = Head;
			Current->Prev = nullptr;
			while (Current != nullptr)
			{
				N* Prev = Current;
				Current = Current->Next;

				if (Current)
				{
					Current->Prev = Prev;
				}
				else
				{
					Prev->Next = nullptr;
					*Tail = Prev;
				}
			}
		}
	}

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
		KeyValuePair(const K& Key, Args&&... args)
			: Key(Key), Value(args...)
		{

		}

		template<typename... Args>
		KeyValuePair(K&& Key, Args&&... args)
			: Key(Move(Key)), Value(args...)
		{

		}

		KeyValuePair(const KeyValuePair<K, T>& Other)
			: Key(Other.Key), Value(Other.Value)
		{
		}

		KeyValuePair(KeyValuePair<K, T>&& Other) noexcept
			: Key(Move(Other.Key)), Value(Move(Other.Value))
		{
		}

		KeyValuePair<K, T>& operator=(const KeyValuePair<K, T>& Other)
		{
			if (*this == Other)
			{
				return *this;
			}

			Key = Other.Key;
			Value = Other.Value;

			return *this;
		}

		KeyValuePair<K, T>& operator=(KeyValuePair<K, T>&& Other) noexcept
		{
			if (*this == Other)
			{
				return *this;
			}

			Key = Move(Other.Key);
			Value = Move(Other.Value);

			return *this;
		}

		bool operator==(const KeyValuePair<K, T>& Other) const
		{
			return Key == Other.Key && Value == Other.Value;
		}

		bool operator!=(const KeyValuePair<K, T>& Other) const
		{
			return !(*this == Other);
		}

		K Key;
		T Value;
	};
}
