#include "Core/NexusEnginePch.h"
#include "HandleManager.h"

namespace NxEn
{
	HandleManager::HandleManager()
	{
		AllocatorActive Active(Memory::GetGlobal());
		Pool = new PoolAllocator(NEXUS_HANDLE_COUNT, sizeof(void*));
	}

	HandleManager::~HandleManager()
	{
		AllocatorActive Active(Memory::GetGlobal());
		delete Pool;
	}

	void* HandleManager::AllocateHandle(void* Pointer)
	{
		uint64* Handle = (uint64*)Pool->Allocate();
		*Handle = reinterpret_cast<uint64>(Pointer);
		return Handle;
	}

	void HandleManager::ModifyHandle(void* Handle, void* Pointer)
	{
		uint64* Address = reinterpret_cast<uint64*>(Handle);
		*Address = reinterpret_cast<uint64>(Pointer);
	}
	
	void HandleManager::FreeHandle(void* Handle)
	{
		Pool->Free(Handle);
	}

	// TODO: Optimization - Algo - Retreive handle
	void* HandleManager::GetHandle(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64* Memory = (uint64*)Pool->GetMemoryBlock();
		for (uint64 I = 0; I < NEXUS_HANDLE_COUNT; ++I)
		{
			uint64 Handle = Memory[I];
			if (Address == Handle)
			{
				return &Memory[I];
			}
		}

		return nullptr;
	}
}
