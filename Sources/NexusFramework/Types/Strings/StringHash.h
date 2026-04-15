#pragma once

#include "NexusFramework/Misc/Hash.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"

namespace NxFr
{
	namespace Hashing
	{
		template<typename H>
		class HashProcess<const char*, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const char* Data)
			{
				State.Accumulate(Data, StringCApi::Length(Data));
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const char* Data)
			{
				HashProcess<const char*, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<size_t N, typename H>
		class HashProcess<char[N], H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const char(&Data)[N])
			{
				HashProcess<const char*, H>::Accumulate(State, Data);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const char(&Data)[N])
			{
				HashProcess<const char*, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<String, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const String& Data)
			{
				State.Accumulate(Data.C(), Data.GetCount());
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const String& Data)
			{
				HashProcess<String, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<StringView, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const StringView& Data)
			{
				State.Accumulate(Data.C(), Data.GetCount());
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const StringView& Data)
			{
				HashProcess<StringView, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<StringId, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const StringId& Data)
			{
				HashProcess<uint64, H>::Accumulate(State, Data.GetId());
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const StringId& Data)
			{
				return Data.GetId();
			}
		};
	}
}
