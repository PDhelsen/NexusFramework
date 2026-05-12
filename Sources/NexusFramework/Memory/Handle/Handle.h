#pragma once

#include "NexusFramework/Types/Numeric/Integer.h"

namespace NxFr
{
	class HandleManager;

	template<typename T>
	struct Handle
	{
		template<typename>
		friend struct Handle;
		friend HandleManager;

		Handle() : Pointer(nullptr) {}
		template<typename U>
		Handle(const Handle<U>& Other) : Pointer(Other.Pointer) {}

		inline explicit operator bool() const { return IsValid(); }

		inline explicit operator T*() { return GetRedirectedPointer(); }
		inline explicit operator const T*() const { return GetRedirectedPointer(); }
		inline T* operator->() { return GetRedirectedPointer(); }
		inline const T* operator->() const { return GetRedirectedPointer(); }

		inline bool operator==(T* Other) const { return GetRedirectedPointer() == Other; }
		inline bool operator!=(T* Other) const { return GetRedirectedPointer() != Other; }
		inline bool operator==(const Handle<T>& Other) const { return GetRedirectedPointer() == Other.GetRedirectedPointer(); }
		inline bool operator!=(const Handle<T>& Other) const { return GetRedirectedPointer() != Other.GetRedirectedPointer(); }
		inline bool operator<(const Handle<T>& Other) const { return GetRedirectedPointer() < Other.GetRedirectedPointer(); }
		inline bool operator<=(const Handle<T>& Other) const { return GetRedirectedPointer() <= Other.GetRedirectedPointer(); }
		inline bool operator>(const Handle<T>& Other) const { return GetRedirectedPointer() > Other.GetRedirectedPointer(); }
		inline bool operator>=(const Handle<T>& Other) const { return GetRedirectedPointer() >= Other.GetRedirectedPointer(); }

		inline bool IsValid() const { return Pointer != nullptr && GetRedirectedPointer() != nullptr; }
		inline T* GetRedirectedPointer()
		{
			uint64* Address = reinterpret_cast<uint64*>(Pointer);
			return (T*)(*Address);
		}
		inline const T* GetRedirectedPointer() const
		{
			uint64* Address = reinterpret_cast<uint64*>(Pointer);
			return (T*)(*Address);
		}

	private:
		void* Pointer;
	};
}
