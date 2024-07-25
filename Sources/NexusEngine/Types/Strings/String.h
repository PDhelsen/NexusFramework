#pragma once

#include "External/Intrinsics.h"
#include "Core/NexusEngine.h"
#include "Types/Integer.h"

namespace NxEn
{
	// TODO: Optimization - Copy / Move - Operator + Self assignement check

	class Allocator;
	struct StringView;
	template <typename T> class List;

	class String
	{
		friend class StringUtility;

	public:
		NEXUS_ENGINE_API String();
		NEXUS_ENGINE_API String(uint64 Bytes);
		NEXUS_ENGINE_API String(const char* Text);
		NEXUS_ENGINE_API String(const char* Text, uint64 Size);
		NEXUS_ENGINE_API String(const StringView& Text);
		NEXUS_ENGINE_API String(const String& Other);
		NEXUS_ENGINE_API String(String&& Other) noexcept;
		NEXUS_ENGINE_API ~String();

		NEXUS_ENGINE_API String& operator=(const String& Other);
		NEXUS_ENGINE_API String& operator+=(const StringView& Other);
		NEXUS_ENGINE_API String& operator-=(const StringView& Other);

		NEXUS_ENGINE_API String& Append(const StringView& Text);
		NEXUS_ENGINE_API String& Replace(const StringView& Old, const StringView& New);
		NEXUS_ENGINE_API String& Assign(const StringView& OldText, const StringView& NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Insert(const StringView& ReferenceText, const StringView& NewText, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Remove(const StringView& Text, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API String& Clear();

		template<typename... Args>
		void Format(const StringView& Format, Args&&... args);
		template<typename... Args>
		void Format(uint64 Size, const StringView& Format, Args&&... args);
		template<typename... Args>
		uint64 Scan(const StringView& Format, Args&&... args);

		NEXUS_ENGINE_API bool Start(const StringView& Substring) const;
		NEXUS_ENGINE_API bool End(const StringView& Substring) const;
		NEXUS_ENGINE_API bool Contains(const StringView& Substring) const;
		NEXUS_ENGINE_API StringView Find(const StringView& Substring, uint64 Offset = 0) const;
		NEXUS_ENGINE_API List<StringView> FindAll(const StringView& Substring) const;
		NEXUS_ENGINE_API StringView Split(const StringView& Substring, uint64 Offset = 0) const;
		NEXUS_ENGINE_API List<StringView> SplitAll(const StringView& Substring) const;

		NEXUS_ENGINE_API void Grow(uint64 Size);
		NEXUS_ENGINE_API void Shrink(uint64 Size = 0);
		NEXUS_ENGINE_API void Validate();

		NEXUS_ENGINE_API StringView ToView() const;
		NEXUS_ENGINE_API StringView ToView(uint64 Offset, uint64 Size) const;

		NEXUS_ENGINE_API const char* C() const { return GetBuffer(); }
		NEXUS_ENGINE_API bool IsEmpty() const { return Count == 0; }
		NEXUS_ENGINE_API uint64 GetCount() const { return Count; }
		NEXUS_ENGINE_API uint64 GetCapacity() const { return Capacity; }

		NEXUS_ENGINE_API static String Empty;
		NEXUS_ENGINE_API static const uint8 SmallStringCapacity = 16;

	private:
		NEXUS_ENGINE_API void Allocate(Allocator* Al, uint64 Bytes, uint64 Size, const char* Text);
		NEXUS_ENGINE_API void Reallocate(uint64 Bytes);
		NEXUS_ENGINE_API void Free();
		NEXUS_ENGINE_API void Resize(uint64 Size);
		NEXUS_ENGINE_API void ValidateCapacityCount(uint64 Bytes, uint64 Size);
		NEXUS_ENGINE_API void ValidateNullTermination();
		NEXUS_ENGINE_API void Append(const char* Text, uint64 Size);
		NEXUS_ENGINE_API void Assign(const char* OldText, uint64 OldSize, const char* NewText, uint64 NewSize, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API void Insert(const char* ReferenceText, uint64 ReferenceSize, const char* NewText, uint64 NewSize, uint64 Offset = 0, uint64 Occurrence = 1, bool All = false);
		NEXUS_ENGINE_API void Remove(const char* Text, uint64 Size, uint64 Offset, uint64 Occurrence, bool All);

		NEXUS_FORCE_INLINE const char* GetBuffer() const { return Sso() ? Data.Small : Data.Large; }
		NEXUS_FORCE_INLINE char* GetData() { return Sso() ? Data.Small : Data.Large; }
		NEXUS_FORCE_INLINE bool Sso() const { return Capacity <= SmallStringCapacity; }

		union Buffer
		{
			char* Large;
			char Small[SmallStringCapacity];
		};

		Allocator* Allctr;
		uint64 Capacity;
		uint64 Count;
		Buffer Data;
	};
}
