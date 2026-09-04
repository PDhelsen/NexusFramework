#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Handle/HandleManager.h"

namespace NxFr
{
	HandleManager::HandleManager(uint64 BucketSize)
		: Buckets(), BucketSize(BucketSize)
	{
	}

	HandleManager::~HandleManager()
	{
		ClearBuckets();
	}

	HandlePointerInfos HandleManager::GetInfos()
	{
		HandlePointerInfos Infos;
		GetInfos(Infos);
		return Infos;
	}

	void HandleManager::GetInfos(HandlePointerInfos& Infos)
	{
		for (auto* Bucket : Buckets)
		{
			Bucket->GetInfos(Infos);
		}
	}

	Handle<void*> HandleManager::GetHandle(void* Pointer) const
	{
		for (HandleBucket* Bucket : Buckets)
		{
			Handle<void*> Handle = Bucket->Find(Pointer);
			if (Handle.IsValid())
			{
				return Handle;
			}
		}

		return Handle<void*>();
	}

	HandleBucket* HandleManager::FindOrCreateBucket()
	{
		HandleBucket* Result = FindBucket();
		if (Result == nullptr)
		{
			Result = CreateBucket();
		}

		return Result;
	}

	HandleBucket* HandleManager::FindBucket()
	{
		for (HandleBucket* Bucket : Buckets)
		{
			if (Bucket->GetCount() < Bucket->GetCapacity())
			{
				return Bucket;
			}
		}

		return nullptr;
	}

	HandleBucket* HandleManager::CreateBucket()
	{
		Allocator::Scope Context(nullptr);

		HandleBucket* Bucket = new HandleBucket(BucketSize);
		Buckets.Append(Bucket);
		return Bucket;
	}

	HandleBucket* HandleManager::GetBucket(Handle<void*> Handle) const
	{
		for (HandleBucket* Bucket : Buckets)
		{
			if (Bucket->Belong(Handle))
			{
				return Bucket;
			}
		}

		return nullptr;
	}

	void HandleManager::ClearBuckets()
	{
		Allocator::Scope Context(nullptr);

		for (HandleBucket* Bucket : Buckets)
		{
			delete Bucket;
		}

		Buckets.Clear();
	}
}
