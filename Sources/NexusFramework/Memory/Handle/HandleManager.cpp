#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Handle/HandleManager.h"

#include "NexusFramework/Core/NexusFrameworkGlobals.h"

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

		auto It = Buffer.Find(Address);
		return It == Buffer.End() ? nullptr : &It.Get();
	}

	Dictionary<void*, Handle<uint8>> HandleManager::GetHandlesPointingToMemoryRange(void* Pointer, uint64 Offset)
	{
		Dictionary<void*, Handle<uint8>> Handles;

		for (auto It = Buffer.Begin(); It != Buffer.End(); ++It)
		{
			void* Data = reinterpret_cast<void*>(*It);
			if (Memory::IsPointerInRange(Data, Pointer, Offset))
			{
				Handle<uint8> Handle;
				Handle.Pointer = &It.Get();

				Handles.Append(Data, Handle);
			}
		}

		return Handles;
	}
}
