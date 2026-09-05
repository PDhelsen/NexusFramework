#pragma once

#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Memory/Handle/HandleManager.h"
#include "NexusFramework/Memory/Allocator/ManagedAllocator.h"

namespace NxFr
{
	namespace Memory
	{
		template<typename T, typename ...Args>
		Handle<T> Create(HandleManager* Bucket, Allocator* Allocator = Allocator::TryGet(), Args&& ...args)
		{
			T* Pointer = Create<T>(Allocator, args...);
			return Bucket->Acquire<T>(Pointer);
		}

		template<typename T>
		void Destroy(Handle<T> Handle, HandleManager* Bucket, Allocator* Allocator = Allocator::TryGet())
		{
			Destroy(Handle.GetRedirectedPointer(), Allocator);
			Bucket->Release<T>(Handle);
		}

		template<typename T, typename ...Args>
		Handle<T> Create(ManagedAllocator* Allocator, Args&& ...args)
		{
			T* Pointer = Create<T>(Allocator, args...);
			return Allocator->Acquire<T>(Pointer);
		}

		template<typename T>
		void Destroy(Handle<T> Handle, ManagedAllocator* Allocator)
		{
			Destroy(Handle.GetRedirectedPointer(), Allocator);
			Allocator->Release<T>(Handle);
		}
	}
}
