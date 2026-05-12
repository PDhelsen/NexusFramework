#pragma once

#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Memory/Handle/HandleManager.h"

namespace NxFr
{
	namespace Memory
	{
		template<typename T, typename ...Args>
		Handle<T> Create(HandleManager* Manager, Allocator* Allocator = AllocatorContext::Get(), Args&& ...args)
		{
			T* Pointer = Create<T>(Allocator, args...);
			return Manager->AcquireHandle<T>(Pointer);
		}

		template<typename T>
		void Destroy(HandleManager* Manager, Handle<T> Handle, Allocator* Allocator = AllocatorContext::Get())
		{
			Destroy(Handle.GetRedirectedPointer(), Allocator);
			Manager->ReleaseHandle<T>(Handle);
		}
	}
}
