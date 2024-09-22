#include "Core/NexusEnginePch.h"
#include "HandleManager.h"

namespace NxEn
{
	HandleManager::HandleManager()
	{
		Buffer = new LinkedList<uint64>(Memory::GetGlobal());
	}

	HandleManager::~HandleManager()
	{
		delete Buffer;
	}

	void* HandleManager::AllocateHandle(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);
		uint64& H = Buffer->AppendFront(Address);
		return &H;
	}

	void HandleManager::ModifyHandle(void* Handle, void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64* H = reinterpret_cast<uint64*>(Handle);
		*H = Address;
	}
	
	void HandleManager::FreeHandle(void* Handle)
	{
		uint64* H = reinterpret_cast<uint64*>(Handle);
		Buffer->Remove(H);
	}

	// TODO: Optimization - Algo - Retreive handle
	void* HandleManager::GetHandle(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		for (auto It = Buffer->Begin(); It != Buffer->End(); ++It)
		{
			uint64& H = It.Get();
			if (Address == H)
			{
				return &H;
			}
		}

		return nullptr;
	}
}
