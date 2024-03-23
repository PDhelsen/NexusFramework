#pragma once

#include "Core/NexusEngineCore.h"
#include "Memory/Handle/Handle.h"
#include "Memory/Allocator/PoolAllocator.h"

namespace NxEn
{
	class HandleManager
	{
	public:
		template<typename T>
		Handle<T> AcquireHandle(T* Pointer);
		template<typename T>
		void UpdateHandle(Handle<T>& Handle, T* Pointer);
		template<typename T>
		void ReleaseHandle(Handle<T>& Handle);

		NEXUS_ENGINE_API static HandleManager* GetInstance() { return Instance; }

	private:
		NEXUS_ENGINE_API HandleManager();
		NEXUS_ENGINE_API ~HandleManager();

		NEXUS_ENGINE_API void* AllocateHandle(void* Pointer);
		NEXUS_ENGINE_API void ModifyHandle(void* Handle, void* Pointer);
		NEXUS_ENGINE_API void FreeHandle(void* Handle);

		PoolAllocator Pool;

		static HandleManager* Instance;
	};
	
	template<typename T>
	inline Handle<T> HandleManager::AcquireHandle(T* Pointer)
	{
		Handle<T> Handle;
		Handle.Pointer = AllocateHandle(Pointer);
		return Handle;
	}

	template<typename T>
	inline void HandleManager::UpdateHandle(Handle<T>& Handle, T* Pointer)
	{
		ModifyHandle(Handle.Pointer, Pointer);
	}

	template<typename T>
	inline void HandleManager::ReleaseHandle(Handle<T>& Handle)
	{
		FreeHandle(Handle.Pointer);
		Handle.Pointer = nullptr;
	}
}
