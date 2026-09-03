#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Memory/Handle/HandleBucket.h"
#include "NexusFramework/Types/Containers/Set.h"

namespace NxFr
{
	class HandleManager
	{
	public:
		NX_NOCOPY_NOMOVE(HandleManager);
		HandleManager(uint64 BucketSize);
		~HandleManager();

		template<typename T>
		Handle<T> Acquire(T* Pointer)
		{
			return FindOrCreateBucket()->Acquire(Pointer);
		}
		template<typename T>
		void Update(Handle<T> Handle, T* Pointer)
		{
			GetBucket(Handle)->Update(Handle, Pointer);
		}
		template<typename T>
		void* Release(Handle<T> Handle)
		{
			return GetBucket(Handle)->Release(Handle);
		}
		template<typename T>
		Handle<T> Find(T* Pointer)
		{
			return GetHandle(Pointer);
		}

		uint64 GetBucketSize() const { return BucketSize; }
		void SetBucketSize(uint64 Size) { this->BucketSize = Size; }

	private:
		Handle<void*> GetHandle(void* Pointer) const;
		HandleBucket* FindOrCreateBucket();
		HandleBucket* FindBucket();
		HandleBucket* CreateBucket();
		HandleBucket* GetBucket(Handle<void*> Handle) const;
		void ClearBuckets();

		Set<HandleBucket*> Buckets;
		uint64 BucketSize;
	};
}

