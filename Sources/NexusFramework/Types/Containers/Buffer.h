#pragma once

#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Memory/Allocator/AllocatorContext.h"
#include "NexusFramework/Debug/Logger/Log.h"
#include "NexusFramework/Types/Containers/Iterator.h"

namespace NxFr
{
	struct BufferView;

	class Buffer
	{
		friend class ContainersUtils;

	public:
		using I = Iterator::IteratorPointer;

		NEXUS_FRAMEWORK_API Buffer(uint64 Size = 0, Allocator* Allctr = AllocatorContext::Get());
		NEXUS_FRAMEWORK_API Buffer(const Buffer& Other);
		NEXUS_FRAMEWORK_API Buffer(Buffer&& Other) noexcept;
		NEXUS_FRAMEWORK_API ~Buffer();

		NEXUS_FRAMEWORK_API Buffer& operator=(const Buffer& Other);
		NEXUS_FRAMEWORK_API Buffer& operator=(Buffer&& Other) noexcept;

		NEXUS_FRAMEWORK_API bool operator==(const Buffer& Other) const;
		NEXUS_FRAMEWORK_API bool operator!=(const Buffer& Other) const;
		NEXUS_FRAMEWORK_API bool operator>(const Buffer& Other) const;
		NEXUS_FRAMEWORK_API bool operator>=(const Buffer& Other) const;
		NEXUS_FRAMEWORK_API bool operator<(const Buffer& Other) const;
		NEXUS_FRAMEWORK_API bool operator<=(const Buffer& Other) const;

		NEXUS_FRAMEWORK_API void Clear();
		NEXUS_FRAMEWORK_API BufferView Get(uint64 Size, uint64 Offset);
		NEXUS_FRAMEWORK_API void Set(const void* Source, uint64 Size = 0, uint64 OffsetBuffer = 0, uint64 OffsetSource = 0);
		NEXUS_FRAMEWORK_API void Grow(uint64 Size);
		NEXUS_FRAMEWORK_API void Resize(uint64 Size);

		NEXUS_FRAMEWORK_API I GetIterator(uint64 Offset);
		NEXUS_FRAMEWORK_API const I GetIterator(uint64 Offset) const;
		NEXUS_FRAMEWORK_API I begin();
		NEXUS_FRAMEWORK_API I Begin();
		NEXUS_FRAMEWORK_API const I begin() const;
		NEXUS_FRAMEWORK_API const I Begin() const;
		NEXUS_FRAMEWORK_API I BeginReverse();
		NEXUS_FRAMEWORK_API const I BeginReverse() const;
		NEXUS_FRAMEWORK_API I end();
		NEXUS_FRAMEWORK_API I End();
		NEXUS_FRAMEWORK_API const I end() const;
		NEXUS_FRAMEWORK_API const I End() const;
		NEXUS_FRAMEWORK_API I EndReverse();
		NEXUS_FRAMEWORK_API const I EndReverse() const;

		template<typename T>
		T* GetPtr(uint64 Offset = 0) const { return static_cast<T*>(GetPtr(Offset)); }
		NEXUS_FRAMEWORK_API void* GetPtr(uint64 Offset = 0) const { return (Byte*)Data + Offset; }
		NEXUS_FRAMEWORK_API uint64 GetCount() const { return Count; }

	private:
		void Allocate(uint64 Size);
		void Reallocate(uint64 Size);
		void Free();
		void Clear(uint64 Size, uint64 Offset);
		void Copy(const void* Source, uint64 Size, uint64 OffsetBuffer, uint64 OffsetSource);
		void ValidateCount(uint64 Size);
		I GetIt(uint64 Offset);
		const I GetIt(uint64 Offset) const;

		Allocator* Alloc;
		uint64 Count;
		void* Data;
	};

	struct BufferView
	{
		friend class ContainersUtils;

	public:
		using I = Iterator::IteratorPointer;

		NEXUS_FRAMEWORK_API BufferView();
		NEXUS_FRAMEWORK_API BufferView(void* Ptr, uint64 Size);
		NEXUS_FRAMEWORK_API BufferView(const Buffer& Other);
		NEXUS_FRAMEWORK_API BufferView(const BufferView& Other);

		NEXUS_FRAMEWORK_API I GetIterator(uint64 Offset);
		NEXUS_FRAMEWORK_API const I GetIterator(uint64 Offset) const;
		NEXUS_FRAMEWORK_API I begin();
		NEXUS_FRAMEWORK_API I Begin();
		NEXUS_FRAMEWORK_API const I begin() const;
		NEXUS_FRAMEWORK_API const I Begin() const;
		NEXUS_FRAMEWORK_API I BeginReverse();
		NEXUS_FRAMEWORK_API const I BeginReverse() const;
		NEXUS_FRAMEWORK_API I end();
		NEXUS_FRAMEWORK_API I End();
		NEXUS_FRAMEWORK_API const I end() const;
		NEXUS_FRAMEWORK_API const I End() const;
		NEXUS_FRAMEWORK_API I EndReverse();
		NEXUS_FRAMEWORK_API const I EndReverse() const;

		template<typename T>
		T* GetPtr(uint64 Offset = 0) const { return static_cast<T*>(GetPtr(Offset)); }
		NEXUS_FRAMEWORK_API void* GetPtr(uint64 Offset = 0) const { return (Byte*)Data + Offset; }
		NEXUS_FRAMEWORK_API uint64 GetCount() const { return Count; }

	private:
		I GetIt(uint64 Offset);
		const I GetIt(uint64 Offset) const;

		void* Data;
		uint64 Count;
	};
}
