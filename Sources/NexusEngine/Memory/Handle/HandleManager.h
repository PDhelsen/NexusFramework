#pragma once

#include "Core/NexusEngineCore.h"
#include "Types/Numbers/Integer.h"
#include "Types/Containers/Dictionary.h"
#include "Types/Containers/Pool.h"
#include "Memory/Handle/Handle.h"
#include "Debug/Logger/Log.h"

namespace NxEn
{
	class HandleManager
	{
	public:
		NEXUS_ENGINE_API static HandleManager* GetInstance();

		NEXUS_ENGINE_API HandleManager(uint64 Size);
		NEXUS_ENGINE_API ~HandleManager();

		template<typename T>
		Handle<T> AcquireHandle(T* Pointer);
		template<typename T>
		void UpdateHandle(Handle<T>& Handle, T* Pointer);
		template<typename T>
		void* ReleaseHandle(Handle<T>& Handle);
		template<typename T>
		Handle<T> FindHandle(T* Pointer);

		NEXUS_ENGINE_API Dictionary<void*, Handle<uint8>> GetHandlesPointingToMemoryRange(void* Pointer, uint64 Offset);

	private:
		NEXUS_ENGINE_API void* AllocateHandle(void* Pointer);
		NEXUS_ENGINE_API void ModifyHandle(void* Handle, void* Pointer);
		NEXUS_ENGINE_API void FreeHandle(void* Handle);
		NEXUS_ENGINE_API void* GetHandle(void* Pointer);

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
