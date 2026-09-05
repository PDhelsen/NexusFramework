#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	template<typename T>
	struct Context;
	class Allocator;

	namespace Memory
	{
		NX_FRAMEWORK_API void* Allocate(uint64, Allocator*, uint64);
		NX_FRAMEWORK_API void* Reallocate(void*, uint64, Allocator*, uint64);
		NX_FRAMEWORK_API void Free(void*, Allocator*);
	}

	class NX_FRAMEWORK_API Allocator
	{
		friend void* Memory::Allocate(uint64, Allocator*, uint64);
		friend void* Memory::Reallocate(void*, uint64, Allocator*, uint64);
		friend void Memory::Free(void*, Allocator*);

	public:
		struct NX_FRAMEWORK_API Scope
		{
			Scope(Allocator* Instance);
			~Scope();
		};

		static Context<Allocator>& GetContexts();
		static Allocator* TryGet();

		NX_NOCOPY_NOMOVE(Allocator)
		Allocator() = default;
		virtual ~Allocator() = default;

		virtual void Clear() = 0;
		virtual bool CanAllocate(uint64 Size, uint64 Alignement) const = 0;
		virtual bool BelongToAllocator(void* Pointer) const = 0;

		virtual uint64 UsedAmount() const = 0;
		virtual uint64 FreeAmount() const = 0;
		virtual uint64 TotalAmount() const = 0;
		virtual float Usage() const = 0;
		virtual bool IsFull() const = 0;
		virtual bool IsEmpty() const = 0;

	protected:
		virtual void* Allocate(uint64 Size, uint64 Alignement) = 0;
		virtual void* Reallocate(void* Pointer, uint64 Size, uint64 Alignement) = 0;
		virtual void Free(void* Pointer) = 0;
	};
}

