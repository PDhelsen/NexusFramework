#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Containers/Pool.h"
#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	class NX_FRAMEWORK_API HandleManager
	{
	public:
		NX_NOCOPY_NOMOVE(HandleManager)
		HandleManager(uint64 Size);
		~HandleManager();

		template<typename T>
		Handle<T> AcquireHandle(T* Pointer)
		{
			Handle<T> Handle;
			Handle.Pointer = AllocateHandle(Pointer);
			return Handle;
		}
		template<typename T>
		void UpdateHandle(Handle<T>& Handle, T* Pointer)
		{
			ModifyHandle(Handle.Pointer, Pointer);
		}
		template<typename T>
		void* ReleaseHandle(Handle<T>& Handle)
		{
			void* RawPointer = Handle.GetRedirectedPointer();
			FreeHandle(Handle.Pointer);
			Handle.Pointer = nullptr;
			return RawPointer;
		}
		template<typename T>
		Handle<T> FindHandle(T* Pointer)
		{
			Handle<T> Handle;
			Handle.Pointer = GetHandle(Pointer);
			return Handle;
		}
		template<typename T>
		bool BelongToManager(Handle<T> Handle)
		{
			return IsBelonging(Handle.Pointer);
		}

		Dictionary<void*, Handle<void>> GetHandlesPointingToMemoryRange(void* Pointer, uint64 Offset);

		bool IsEmpty() const { return Buffer.GetCount() == 0; }
		uint64 GetCount() const { return Buffer.GetCount(); }
		uint64 GetCapacity() const { return Buffer.GetCapacity(); }

	private:
		void* AllocateHandle(void* Pointer);
		void ModifyHandle(void* Handle, void* Pointer);
		void FreeHandle(void* Handle);
		void* GetHandle(void* Pointer);
		bool IsBelonging(void* Pointer);

		Pool<uint64, Pooling::PreAllocated<uint64>> Buffer;
	};
}
