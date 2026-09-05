#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Containers/Pool.h"
#include "NexusFramework/Types/Containers/Tuple.h"
#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	using HandleManagerInfo = Tuple<Handle<void>, HandleManager*>;
	using HandleMemroyInfos = Dictionary<void*, HandleManagerInfo>;

	class NX_FRAMEWORK_API HandleManager
	{
	public:
		NX_NOCOPY_NOMOVE(HandleManager)
		HandleManager(uint64 Size);
		~HandleManager();

		template<typename T>
		Handle<T> Acquire(T* Pointer)
		{
			Handle<T> Handle;
			Handle.Pointer = Allocate(Pointer);
			return Handle;
		}
		template<typename T>
		void Update(Handle<T>& Handle, T* Pointer)
		{
			Modify(Handle.Pointer, Pointer);
		}
		template<typename T>
		void* Release(Handle<T>& Handle)
		{
			void* RawPointer = Handle.GetRedirectedPointer();
			Free(Handle.Pointer);
			Handle.Pointer = nullptr;
			return RawPointer;
		}
		template<typename T>
		Handle<T> Find(T* Pointer)
		{
			Handle<T> Handle;
			Handle.Pointer = GetHandle(Pointer);
			return Handle;
		}
		template<typename T>
		bool Belong(Handle<T> Handle)
		{
			return Belong(Handle.Pointer);
		}

		HandleMemroyInfos GetInfos();
		void GetInfos(HandleMemroyInfos& Infos);

		bool IsEmpty() const { return Buffer.GetCount() == 0; }
		uint64 GetCount() const { return Buffer.GetCount(); }
		uint64 GetCapacity() const { return Buffer.GetCapacity(); }

	private:
		void* Allocate(void* Pointer);
		void Modify(void* Handle, void* Pointer);
		void Free(void* Handle);
		void* GetHandle(void* Pointer);
		bool Belong(void* Pointer);

		Pool<uint64, Pooling::PreAllocated<uint64>> Buffer;
	};
}
