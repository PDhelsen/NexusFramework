#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"

namespace NxFr
{
	struct StringView;
	namespace Iterator
	{
		struct StringCharacter;
		struct StringToken;
	}

	class NX_FRAMEWORK_API String
	{
	public:
		using Similar = StringView;

		String(Allocator* Allctr = Allocator::TryGet());
		String(uint64 Bytes, Allocator* Allctr = Allocator::TryGet());
		String(const char* Text, Allocator* Allctr = Allocator::TryGet());
		String(const char* Text, uint64 Size, Allocator* Allctr = Allocator::TryGet());
		String(StringView Text, Allocator* Allctr = Allocator::TryGet());
		String(const String& Text);
		String(String&& Other) noexcept;
		~String();

		String& operator=(const String& Other);
		String& operator=(String&& Other) noexcept;
		String& operator+=(StringView Other);
		String& operator-=(StringView Other);

		String& Assign(StringView OldText, StringView NewText);
		String& Assign(StringView OldText, StringView NewText, uint64 Offset, uint64 Occurrence);
		String& Append(StringView Text);
		String& Insert(StringView ReferenceText, StringView NewText);
		String& Insert(StringView ReferenceText, StringView NewText, uint64 Offset, uint64 Occurrence);
		String& Remove(StringView Text);
		String& Remove(StringView Text, uint64 Offset, uint64 Occurrence);
		String& Terminate(uint64 Size);
		String& Clear();

		void Reserve(uint64 Size);
		void Validate();

		const Iterator::StringCharacter Begin() const;
		const Iterator::StringToken Begin(StringView Token) const;
		const Iterator::StringCharacter End() const;
		const Iterator::StringToken End(StringView Token) const;

		StringView Substring(uint64 Offset, uint64 Size) const;
		char* Characters();

		const char* C() const { return GetBuffer(); }
		bool IsEmpty() const { return Count == 0; }
		uint64 GetCount() const { return Count; }
		uint64 GetCapacity() const { return Capacity; }

	private:
		inline static const uint8 SmallStringCapacity = 16;

		union Buffer
		{
			char* Large;
			char Small[SmallStringCapacity];
		};

		void Allocate(uint64 Bytes, uint64 Size, const char* Text);
		void Reallocate(uint64 Bytes);
		void Free();
		void Resize(uint64 Size);
		void ValidateCapacityCount(uint64 Bytes, uint64 Size);
		void ValidateNullTermination();
		void Append(const char* Text, uint64 Size);
		void Assign(const char* OldText, uint64 OldSize, const char* NewText, uint64 NewSize, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		void Insert(const char* ReferenceText, uint64 ReferenceSize, const char* NewText, uint64 NewSize, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		void Remove(const char* Text, uint64 Size, uint64 Offset, uint64 Occurrence, bool All);

		inline const char* GetBuffer() const { return Sso() ? Data.Small : Data.Large; }
		inline char* GetData() { return Sso() ? Data.Small : Data.Large; }
		inline bool Sso() const { return Capacity <= SmallStringCapacity; }

		Allocator* Alloc;
		uint64 Capacity;
		uint64 Count;
		Buffer Data;
	};
}
