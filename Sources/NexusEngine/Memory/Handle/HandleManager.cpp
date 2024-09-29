#include "Core/NexusEnginePch.h"
#include "HandleManager.h"

namespace NxEn
{
	HandleManager::HandleManager()
	{
		Buffer = new Pool<uint64, Pooling::PreAllocated<uint64>>(NEXUS_HANDLES_COUNT, Memory::GetGlobal());
	}

	HandleManager::~HandleManager()
	{
		delete Buffer;
	}

	void* HandleManager::AllocateHandle(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64& Redirection = Buffer->Acquire();
		Redirection = Address;

		return &Redirection;
	}

	void HandleManager::ModifyHandle(void* Handle, void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64* Redirection = reinterpret_cast<uint64*>(Handle);
		*Redirection = Address;
	}
	
	void HandleManager::FreeHandle(void* Handle)
	{
		uint64* Redirection = reinterpret_cast<uint64*>(Handle);
		*Redirection = 0;

		Buffer->Recycle(*Redirection);
	}

	void* HandleManager::GetHandle(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		auto It = Buffer->Find(Address);
		return It == Buffer->End() ? nullptr : &It.Get();
	}
}
