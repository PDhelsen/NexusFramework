#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Memory/Handle/HandleManager.h"
#include "NexusFramework/Types/Containers/Set.h"

namespace NxFr
{
	class NX_FRAMEWORK_API HandleBucketManager
	{
	public:
		NX_NOCOPY_NOMOVE(HandleBucketManager);
		HandleBucketManager(uint64 BucketSize);
		~HandleBucketManager();

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

		HandleMemroyInfos GetInfos();
		void GetInfos(HandleMemroyInfos& Infos);

		uint64 GetBucketSize() const { return BucketSize; }
		void SetBucketSize(uint64 Size) { this->BucketSize = Size; }

	private:
		Handle<void*> GetHandle(void* Pointer) const;
		HandleManager* FindOrCreateBucket();
		HandleManager* FindBucket();
		HandleManager* CreateBucket();
		HandleManager* GetBucket(Handle<void*> Handle) const;
		void ClearBuckets();

		Set<HandleManager*> Buckets;
		uint64 BucketSize;
	};
}

