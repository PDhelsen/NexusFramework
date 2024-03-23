#include "Core/NexusEnginePch.h"
#include "HandleManager.h"

namespace NxEn
{
	HandleManager* HandleManager::Instance = new HandleManager();

	HandleManager::HandleManager()
		: Pool(PoolAllocator(NEXUS_HANDLE_COUNT, sizeof(void*)))
	{
	}

	HandleManager::~HandleManager()
	{
	}

	void* HandleManager::AllocateHandle(void* Pointer)
	{
		uint64* Handle = (uint64*)Pool.Allocate();
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
		Pool.Free(Handle);
	}
}