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
		Handle<T> Create(HandleManager* Bucket, Allocator* Allctr = Allocator::TryGet(), Args&& ...args)
		{
			T* Pointer = Create<T>(Allctr, args...);
			return Bucket->Acquire<T>(Pointer);
		}

		template<typename T>
		void Destroy(Handle<T> Handle, HandleManager* Bucket, Allocator* Allctr = Allocator::TryGet())
		{
			Destroy(Handle.GetRedirectedPointer(), Allctr);
			Bucket->Release<T>(Handle);
		}

		template<typename T, typename ...Args>
		Handle<T> Create(ManagedAllocator* Allctr, Args&& ...args)
		{
			T* Pointer = Create<T>(static_cast<Allocator*>(Allctr), args...);
			return Allctr->Acquire<T>(Pointer);
		}

		template<typename T>
		void Destroy(Handle<T> Handle, ManagedAllocator* Allctr)
		{
			Destroy(Handle.GetRedirectedPointer(), static_cast<Allocator*>(Allctr));
			Allctr->Release<T>(Handle);
		}
	}
}
