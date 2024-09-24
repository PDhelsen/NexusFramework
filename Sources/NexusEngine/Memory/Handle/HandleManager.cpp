#include "Core/NexusEnginePch.h"
#include "HandleManager.h"

namespace NxEn
{
	HandleManager::HandleManager()
	{
		Buffer = new Array<uint64>(NEXUS_HANDLES_COUNT, Memory::GetGlobal());
		Free = new Stack<uint64*, NEXUS_HANDLES_COUNT>(Memory::GetGlobal());

		for (uint64 I = Buffer->GetCount(); I > 0; I--)
		{
			uint64& Slot = Buffer->Get(I - 1);
			Free->Append(&Slot);
		}
	}

	HandleManager::~HandleManager()
	{
		delete Buffer;
		delete Free;
	}

	void* HandleManager::AllocateHandle(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64* Redirection = Free->Get();
		Free->Remove();

		*Redirection = Address;
		return Redirection;
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

		Free->Append(Redirection);
	}

	void* HandleManager::GetHandle(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		auto It = Buffer->Find(Address);
		return It == Buffer->End() ? nullptr : &It.Get();
	}
}
