#pragma once

#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"

#include "NexusFramework/Misc/Hash/Hash.h"

namespace NxFr
{
	namespace HashUtility
	{
		template<typename H>
		struct Hasher<const char*, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const char* Data)
			{
				State.Accumulate(Data, StringCApi::Length(Data));
			}
		};

		template<size_t N, typename H>
		struct Hasher<char[N], H>
		{
		public:
			static void Accumulate(Hash<H>& State, const char(&Data)[N])
			{
				State.Accumulate(Data, StringCApi::Length(Data));
			}
		};

		template<typename H>
		struct Hasher<String, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const String& Data)
			{
				State.Accumulate(Data.C(), Data.GetCount());
			}
		};

		template<typename H>
		struct Hasher<StringView, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const StringView& Data)
			{
				State.Accumulate(Data.C(), Data.GetCount());
			}
		};

		template<typename H>
		struct Hasher<StringId, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const StringId& Data)
			{
				State.Accumulate(Data.GetId());
			}
		};
	}
}
