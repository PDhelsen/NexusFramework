#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Strings/StringView.h"

namespace NxFr
{
	namespace Iterator
	{
		struct StringCharacter
		{
		public:
			NEXUS_FRAMEWORK_API StringCharacter(StringView Text, uint64 Offset = 0);
			NEXUS_FRAMEWORK_API ~StringCharacter();

			NEXUS_FRAMEWORK_API StringCharacter& operator++();
			NEXUS_FRAMEWORK_API StringCharacter operator++(int32);
			NEXUS_FRAMEWORK_API StringCharacter& operator--();
			NEXUS_FRAMEWORK_API StringCharacter operator--(int32);

			NEXUS_FRAMEWORK_API StringView operator->() const;
			NEXUS_FRAMEWORK_API StringView operator*() const;

			NEXUS_FRAMEWORK_API bool operator==(const StringCharacter& Other);
			NEXUS_FRAMEWORK_API bool operator!=(const StringCharacter& Other);
			NEXUS_FRAMEWORK_API bool Equals(const StringCharacter& Other) const;

			NEXUS_FRAMEWORK_API StringView GetRemainder() const;
			NEXUS_FRAMEWORK_API StringView Get() const;
			NEXUS_FRAMEWORK_API uint64 Id() const;
			NEXUS_FRAMEWORK_API void Iterate();
			NEXUS_FRAMEWORK_API void Reverse();

			NEXUS_FRAMEWORK_API StringCharacter& Next();
			NEXUS_FRAMEWORK_API StringCharacter& Previous();

		private:
			StringView Data;
			uint64 Cursor;
		};

		struct StringToken
		{
		public:
			NEXUS_FRAMEWORK_API StringToken(StringView Token, StringView Text, uint64 Offset = 0);
			NEXUS_FRAMEWORK_API ~StringToken();

			NEXUS_FRAMEWORK_API StringToken& operator++();
			NEXUS_FRAMEWORK_API StringToken operator++(int32);
			NEXUS_FRAMEWORK_API StringToken& operator--();
			NEXUS_FRAMEWORK_API StringToken operator--(int32);

			NEXUS_FRAMEWORK_API StringView operator->() const;
			NEXUS_FRAMEWORK_API StringView operator*() const;

			NEXUS_FRAMEWORK_API bool operator==(const StringToken& Other);
			NEXUS_FRAMEWORK_API bool operator!=(const StringToken& Other);
			NEXUS_FRAMEWORK_API bool Equals(const StringToken& Other) const;

			NEXUS_FRAMEWORK_API StringView GetToken() const;
			NEXUS_FRAMEWORK_API StringView GetRemainder() const;
			NEXUS_FRAMEWORK_API StringView Get() const;
			NEXUS_FRAMEWORK_API uint64 Id() const;
			NEXUS_FRAMEWORK_API void Iterate();
			NEXUS_FRAMEWORK_API void Reverse();

			NEXUS_FRAMEWORK_API StringToken& Next();
			NEXUS_FRAMEWORK_API StringToken& Previous();

		private:
			StringView Token;
			StringView Data;
			StringView Cursor;
		};
	}
}
