#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Containers/Pool.h"
#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	class HandleManager
	{
	public:
		NEXUS_FRAMEWORK_API HandleManager(uint64 Size);
		NEXUS_FRAMEWORK_API ~HandleManager();

		template<typename T>
		Handle<T> AcquireHandle(T* Pointer);
		template<typename T>
		void UpdateHandle(Handle<T>& Handle, T* Pointer);
		template<typename T>
		void* ReleaseHandle(Handle<T>& Handle);
		template<typename T>
		Handle<T> FindHandle(T* Pointer);

		NEXUS_FRAMEWORK_API Dictionary<void*, Handle<uint8>> GetHandlesPointingToMemoryRange(void* Pointer, uint64 Offset);

		bool IsEmpty() const { return Buffer.GetCount() == 0; }
		uint64 GetCount() const { return Buffer.GetCount(); }
		uint64 GetCapacity() const { return Buffer.GetCapacity(); }

	private:
		NEXUS_FRAMEWORK_API void* AllocateHandle(void* Pointer);
		NEXUS_FRAMEWORK_API void ModifyHandle(void* Handle, void* Pointer);
		NEXUS_FRAMEWORK_API void FreeHandle(void* Handle);
		NEXUS_FRAMEWORK_API void* GetHandle(void* Pointer);

		Pool<uint64, Pooling::PreAllocated<uint64>> Buffer;
	};

	template<typename T>
	Handle<T> HandleManager::AcquireHandle(T* Pointer)
	{
		NEXUS_ASSERT(Pointer, Default, "Null Pointer");

		Handle<T> Handle;
		Handle.Pointer = AllocateHandle(Pointer);
		return Handle;
	}

	template<typename T>
	void HandleManager::UpdateHandle(Handle<T>& Handle, T* Pointer)
	{
		NEXUS_ASSERT(Pointer, Default, "Null Pointer");

		ModifyHandle(Handle.Pointer, Pointer);
	}

	template<typename T>
	void* HandleManager::ReleaseHandle(Handle<T>& Handle)
	{
		void* RawPointer = Handle.GetRedirectedPointer();
		FreeHandle(Handle.Pointer);
		Handle.Pointer = nullptr;
		return RawPointer;
	}

	template<typename T>
	Handle<T> HandleManager::FindHandle(T* Pointer)
	{
		NEXUS_ASSERT(Pointer, Default, "Null Pointer");

		Handle<T> Handle;
		Handle.Pointer = GetHandle(Pointer);
		return Handle;
	}
}
