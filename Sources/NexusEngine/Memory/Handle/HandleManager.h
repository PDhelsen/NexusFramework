#pragma once

#include "Core/NexusEngine.h"
#include "Memory/Handle/Handle.h"
#include "Memory/Allocator/PoolAllocator.h"

namespace NxEn
{
	// Manage the global array of data pointer referenced by the Handle

	class HandleManager
	{
	public:
		template<typename T>
		Handle<T> AcquireHandle(T* Pointer);
		template<typename T>
		void UpdateHandle(Handle<T>& Handle, T* Pointer);
		template<typename T>
		void* ReleaseHandle(Handle<T>& Handle);
		template<typename T>
		Handle<T> FindHandle(T* Pointer);

		NEXUS_ENGINE_API static HandleManager* GetInstance() { return Instance; }

	private:
		NEXUS_ENGINE_API HandleManager();
		NEXUS_ENGINE_API ~HandleManager();

		NEXUS_ENGINE_API void* AllocateHandle(void* Pointer);
		NEXUS_ENGINE_API void ModifyHandle(void* Handle, void* Pointer);
		NEXUS_ENGINE_API void FreeHandle(void* Handle);
		NEXUS_ENGINE_API void* GetHandle(void* Pointer);

		// TEMP: Replace - Container - Once we have custom pool
		PoolAllocator Pool;

		static HandleManager* Instance;
	};
	
	template<typename T>
	Handle<T> HandleManager::AcquireHandle(T* Pointer)
	{
		Handle<T> Handle;
		Handle.Pointer = AllocateHandle(Pointer);
		return Handle;
	}

	template<typename T>
	void HandleManager::UpdateHandle(Handle<T>& Handle, T* Pointer)
	{
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
		Handle<T> Handle;
		Handle.Pointer = GetHandle(Pointer);
		return Handle;
	}
}
