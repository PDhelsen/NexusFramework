#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Handle/HandleBucketManager.h"

namespace NxFr
{
	HandleBucketManager::HandleBucketManager(uint64 BucketSize)
		: Buckets(), BucketSize(BucketSize)
	{
	}

	HandleBucketManager::~HandleBucketManager()
	{
		ClearBuckets();
	}

	HandleMemroyInfos HandleBucketManager::GetInfos()
	{
		HandleMemroyInfos Infos;
		GetInfos(Infos);
		return Infos;
	}

	void HandleBucketManager::GetInfos(HandleMemroyInfos& Infos)
	{
		for (auto* Bucket : Buckets)
		{
			Bucket->GetInfos(Infos);
		}
	}

	Handle<void*> HandleBucketManager::GetHandle(void* Pointer) const
	{
		for (HandleManager* Bucket : Buckets)
		{
			Handle<void*> Handle = Bucket->Find(Pointer);
			if (Handle.IsValid())
			{
				return Handle;
			}
		}

		return Handle<void*>();
	}

	HandleManager* HandleBucketManager::FindOrCreateBucket()
	{
		HandleManager* Result = FindBucket();
		if (Result == nullptr)
		{
			Result = CreateBucket();
		}

		return Result;
	}

	HandleManager* HandleBucketManager::FindBucket()
	{
		for (HandleManager* Bucket : Buckets)
		{
			if (Bucket->GetCount() < Bucket->GetCapacity())
			{
				return Bucket;
			}
		}

		return nullptr;
	}

	HandleManager* HandleBucketManager::CreateBucket()
	{
		Allocator::Scope Context(nullptr);

		HandleManager* Bucket = new HandleManager(BucketSize);
		Buckets.Append(Bucket);
		return Bucket;
	}

	HandleManager* HandleBucketManager::GetBucket(Handle<void*> Handle) const
	{
		for (HandleManager* Bucket : Buckets)
		{
			if (Bucket->Belong(Handle))
			{
				return Bucket;
			}
		}

		return nullptr;
	}

	void HandleBucketManager::ClearBuckets()
	{
		Allocator::Scope Context(nullptr);

		for (HandleManager* Bucket : Buckets)
		{
			delete Bucket;
		}

		Buckets.Clear();
	}
}
