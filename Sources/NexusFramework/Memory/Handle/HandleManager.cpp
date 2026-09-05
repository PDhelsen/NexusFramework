#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Handle/HandleManager.h"

namespace NxFr
{
	HandleManager::HandleManager(uint64 Size)
		: Buffer(Size)
	{
	}

	HandleManager::~HandleManager()
	{

	}

	void* HandleManager::Allocate(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64& Redirection = Buffer.Acquire();
		Redirection = Address;

		return &Redirection;
	}

	void HandleManager::Modify(void* Handle, void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64* Redirection = reinterpret_cast<uint64*>(Handle);
		*Redirection = Address;
	}

	void HandleManager::Free(void* Handle)
	{
		uint64* Redirection = reinterpret_cast<uint64*>(Handle);
		*Redirection = 0;

		Buffer.Recycle(*Redirection);
	}

	void* HandleManager::GetHandle(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		auto It = ContainerUtility::Find(Buffer, Address);
		return It == Buffer.End() ? nullptr : &It.Get();
	}

	bool HandleManager::Belong(void* Pointer)
	{
		uint64* Address = reinterpret_cast<uint64*>(Pointer);
		return &Buffer.Begin().Get() <= Address && Address < &Buffer.End().Get();
	}

	HandleMemroyInfos HandleManager::GetInfos()
	{
		HandleMemroyInfos Infos;
		GetInfos(Infos);
		return Infos;
	}

	void HandleManager::GetInfos(HandleMemroyInfos& Infos)
	{
		for (auto It = Buffer.Begin(); It != Buffer.End(); ++It)
		{
			void* Data = reinterpret_cast<void*>(*It);

			Handle<void> Handle;
			Handle.Pointer = &It.Get();

			Infos.Append(Data, HandleManagerInfo(Handle, this));
		}
	}
}
