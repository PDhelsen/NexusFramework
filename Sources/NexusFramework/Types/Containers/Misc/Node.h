#pragma once

#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Misc/Node/Node.h"

namespace NxFr
{
	namespace Node
	{
		template<typename T>
		struct NodeGraphConnection;

#pragma region Node

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

		template<typename T>
		struct NodeHashmap
		{
			enum class TombstoneMode : uint8
			{
				NotTombstone, IsTombstone, WasTombstone
			};

			T Value;
			uint64 Hash;
			bool Free;
			TombstoneMode Tombstone;

			void Clear()
			{
				Hash = 0;
				Free = true;
				Tombstone = TombstoneMode::NotTombstone;
			}

			void Update()
			{
				Free = true;
				Tombstone = TombstoneMode::IsTombstone;
			}

			void Update(uint64 Key)
			{
				Hash = Key;
				Free = false;
				if (Tombstone == TombstoneMode::IsTombstone)
				{
					Tombstone = TombstoneMode::WasTombstone;
				}
			}
		};

#pragma endregion

#pragma region Connection

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

#pragma endregion

	}
}
