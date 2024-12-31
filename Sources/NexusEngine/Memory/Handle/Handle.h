#pragma once

#include "Types/Numbers/Integer.h"

namespace NxEn
{
	class HandleManager;

	template<typename T>
	struct Handle
	{
		friend HandleManager;

		inline bool IsValid() { return Pointer != nullptr; }
		inline T* GetRedirectedPointer()
		{
			uint64* Address = reinterpret_cast<uint64*>(Pointer);
			return (T*)(*Address);
		}

		inline T* operator->() { return GetRedirectedPointer(); }
		inline bool operator==(const Handle<T>& Other) const { return Pointer == Other.Pointer; }
		inline bool operator!=(const Handle<T>& Other) const { return Pointer != Other.Pointer; }

	private:
		Handle() = default;

		void* Pointer = nullptr;
	};
}
