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

	void* HandleManager::AllocateHandle(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		uint64& Redirection = Buffer.Acquire();
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

		Buffer.Recycle(*Redirection);
	}

	void* HandleManager::GetHandle(void* Pointer)
	{
		uint64 Address = reinterpret_cast<uint64>(Pointer);

		auto It = ContainersUtils::Find(Buffer, Address);
		return It == Buffer.End() ? nullptr : &It.Get();
	}

	bool HandleManager::IsBelonging(void* Pointer)
	{
		uint64* Address = reinterpret_cast<uint64*>(Pointer);
		return &Buffer.Begin().Get() <= Address && Address < &Buffer.End().Get();
	}

	Dictionary<void*, Handle<void>> HandleManager::GetHandlesPointingToMemoryRange(void* Pointer, uint64 Offset)
	{
		Dictionary<void*, Handle<void>> Handles;

		for (auto It = Buffer.Begin(); It != Buffer.End(); ++It)
		{
			void* Data = reinterpret_cast<void*>(*It);
			if (Memory::IsPointerInRange(Data, Pointer, Offset))
			{
				Handle<void> Handle;
				Handle.Pointer = &It.Get();

				Handles.Append(Data, Handle);
			}
		}

		return Handles;
	}
}
