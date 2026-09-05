#pragma once

#include "NexusFramework/Memory/Allocator/HeapAllocator.h"
#include "NexusFramework/Memory/Allocator/ContinuousAllocator.h"
#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Memory/Handle/HandleManager.h"
#include "NexusFramework/Memory/Handle/HandleBucketManager.h"

namespace NxFr
{
	class NX_FRAMEWORK_API ManagedAllocator : public ContinuousAllocator
	{
		class Heap : public HeapAllocator
		{
		public:
			NX_NOCOPY_NOMOVE(Heap)
			Heap(uint64 Size);
			virtual ~Heap();

			void Defragment(const HandleMemroyInfos& Infos, float& Time, uint64& Count);
		};

	public:
		NX_NOCOPY_NOMOVE(ManagedAllocator)
		ManagedAllocator(uint64 Size, uint64 Count);
		virtual ~ManagedAllocator();

		template<typename T>
		Handle<T> Acquire(T* Pointer)
		{
			return Manager.Acquire(Pointer);
		}
		template<typename T>
		void Update(Handle<T> Handle, T* Pointer)
		{
			Manager.Update(Handle, Pointer);
		}
		template<typename T>
		void* Release(Handle<T> Handle)
		{
			return Manager.Release(Handle);
		}
		template<typename T>
		Handle<T> Find(T* Pointer)
		{
			return Manager.Find(Pointer);
		}

		void Defragment();
		void Defragment(float Time);
		void Defragment(uint64 Count);

		uint64 GetHandleBucketSize() const { return Manager.GetBucketSize(); }
		void SetHandleBucketSize(uint64 Size) { Manager.SetBucketSize(Size); }

	private:
		void Defragment(float& Time, uint64& Count);

		HandleBucketManager Manager;
		HandleMemroyInfos Infos;
	};
}

