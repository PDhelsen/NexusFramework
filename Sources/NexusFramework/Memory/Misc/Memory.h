#pragma once

#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Memory/Handle/HandleBucket.h"

namespace NxFr
{
	namespace Memory
	{
		template<typename T, typename ...Args>
		Handle<T> Create(HandleBucket* Bucket, Allocator* Allocator = Allocator::TryGet(), Args&& ...args)
		{
			T* Pointer = Create<T>(Allocator, args...);
			return Bucket->Acquire<T>(Pointer);
		}

		template<typename T>
		void Destroy(HandleBucket* Bucket, Handle<T> Handle, Allocator* Allocator = Allocator::TryGet())
		{
			Destroy(Handle.GetRedirectedPointer(), Allocator);
			Bucket->Release<T>(Handle);
		}
	}
}
