#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Memory/Allocator/AllocatorContext.h"
#include "NexusFramework/Memory/Memory.h"

namespace NxFr
{
	struct StringView;
	template <typename T> class List;

	class String
	{
	public:
		using Similar = StringView;

	private:
		inline static const uint8 SmallStringCapacity = 16;

		union Buffer
		{
			char* Large;
			char Small[SmallStringCapacity];
		};

	public:
		NEXUS_FRAMEWORK_API String(Allocator* Allctr = AllocatorContext::Get());
		NEXUS_FRAMEWORK_API String(uint64 Bytes, Allocator* Allctr = AllocatorContext::Get());
		NEXUS_FRAMEWORK_API String(const char* Text, Allocator* Allctr = AllocatorContext::Get());
		NEXUS_FRAMEWORK_API String(const char* Text, uint64 Size, Allocator* Allctr = AllocatorContext::Get());
		NEXUS_FRAMEWORK_API String(StringView Text, Allocator* Allctr = AllocatorContext::Get());
		NEXUS_FRAMEWORK_API String(const String& Text);
		NEXUS_FRAMEWORK_API String(String&& Other) noexcept;
		NEXUS_FRAMEWORK_API ~String();

		NEXUS_FRAMEWORK_API String& operator=(const String& Other);
		NEXUS_FRAMEWORK_API String& operator=(String&& Other) noexcept;
		NEXUS_FRAMEWORK_API String& operator+=(StringView Other);
		NEXUS_FRAMEWORK_API String& operator-=(StringView Other);

		template<typename... Args>
		String& Format(StringView Format, Args&&... args);
		template<typename... Args>
		String& Format(uint64 Size, StringView Format, Args&&... args);

		NEXUS_FRAMEWORK_API String& Assign(StringView OldText, StringView NewText);
		NEXUS_FRAMEWORK_API String& Assign(StringView OldText, StringView NewText, uint64 Offset, uint64 Occurrence);
		NEXUS_FRAMEWORK_API String& Append(StringView Text);
		NEXUS_FRAMEWORK_API String& Insert(StringView ReferenceText, StringView NewText);
		NEXUS_FRAMEWORK_API String& Insert(StringView ReferenceText, StringView NewText, uint64 Offset, uint64 Occurrence);
		NEXUS_FRAMEWORK_API String& Remove(StringView Text);
		NEXUS_FRAMEWORK_API String& Remove(StringView Text, uint64 Offset, uint64 Occurrence);
		NEXUS_FRAMEWORK_API String& Terminate(uint64 Size);
		NEXUS_FRAMEWORK_API String& Clear();

		NEXUS_FRAMEWORK_API void Grow(uint64 Size);
		NEXUS_FRAMEWORK_API void Shrink(uint64 Size = 0);
		NEXUS_FRAMEWORK_API void Validate();

		NEXUS_FRAMEWORK_API StringView Substring(uint64 Offset, uint64 Size) const;
		NEXUS_FRAMEWORK_API char* Characters();

		NEXUS_FRAMEWORK_API const char* C() const { return GetBuffer(); }
		NEXUS_FRAMEWORK_API bool IsEmpty() const { return Count == 0; }
		NEXUS_FRAMEWORK_API uint64 GetCount() const { return Count; }
		NEXUS_FRAMEWORK_API uint64 GetCapacity() const { return Capacity; }

	private:
		NEXUS_FRAMEWORK_API void Allocate(uint64 Bytes, uint64 Size, const char* Text);
		NEXUS_FRAMEWORK_API void Reallocate(uint64 Bytes);
		NEXUS_FRAMEWORK_API void Free();
		NEXUS_FRAMEWORK_API void Resize(uint64 Size);
		NEXUS_FRAMEWORK_API void ValidateCapacityCount(uint64 Bytes, uint64 Size);
		NEXUS_FRAMEWORK_API void ValidateNullTermination();
		NEXUS_FRAMEWORK_API void Append(const char* Text, uint64 Size);
		NEXUS_FRAMEWORK_API void Assign(const char* OldText, uint64 OldSize, const char* NewText, uint64 NewSize, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_FRAMEWORK_API void Insert(const char* ReferenceText, uint64 ReferenceSize, const char* NewText, uint64 NewSize, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_FRAMEWORK_API void Remove(const char* Text, uint64 Size, uint64 Offset, uint64 Occurrence, bool All);

		inline const char* GetBuffer() const { return Sso() ? Data.Small : Data.Large; }
		inline char* GetData() { return Sso() ? Data.Small : Data.Large; }
		inline bool Sso() const { return Capacity <= SmallStringCapacity; }

		Allocator* Alloc;
		uint64 Capacity;
		uint64 Count;
		Buffer Data;
	};
}
