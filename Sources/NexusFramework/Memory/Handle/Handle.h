#pragma once

#include "NexusFramework/Types/Numeric/Integer.h"

namespace NxFr
{
	template<typename T>
	struct Handle
	{
		template<typename>
		friend struct Handle;
		friend class HandleManager;

		Handle() : Pointer(nullptr) {}
		template<typename U>
		Handle(const Handle<U>& Other) : Pointer(Other.Pointer) {}

		inline explicit operator bool() const { return IsValid(); }

		inline explicit operator T*() { return GetRedirectedPointer(); }
		inline explicit operator const T*() const { return GetRedirectedPointer(); }
		inline T* operator->() { return GetRedirectedPointer(); }
		inline const T* operator->() const { return GetRedirectedPointer(); }

		template<typename U> inline bool operator==(U* Other) const { return GetRedirectedPointer() == Other; }
		template<typename U> inline bool operator!=(U* Other) const { return GetRedirectedPointer() != Other; }
		template<typename U> inline bool operator==(const Handle<U>& Other) const { return GetRedirectedPointer() == Other.GetRedirectedPointer(); }
		template<typename U> inline bool operator!=(const Handle<U>& Other) const { return GetRedirectedPointer() != Other.GetRedirectedPointer(); }
		template<typename U> inline bool operator<(const Handle<U>& Other) const { return GetRedirectedPointer() < Other.GetRedirectedPointer(); }
		template<typename U> inline bool operator<=(const Handle<U>& Other) const { return GetRedirectedPointer() <= Other.GetRedirectedPointer(); }
		template<typename U> inline bool operator>(const Handle<U>& Other) const { return GetRedirectedPointer() > Other.GetRedirectedPointer(); }
		template<typename U> inline bool operator>=(const Handle<U>& Other) const { return GetRedirectedPointer() >= Other.GetRedirectedPointer(); }

		inline bool IsValid() const { return Pointer != nullptr && GetRedirectedPointer() != nullptr; }
		inline T* GetRedirectedPointer()
		{
			uint64* Address = reinterpret_cast<uint64*>(Pointer);
			return Address ? (T*)(*Address) : nullptr;
		}
		inline const T* GetRedirectedPointer() const
		{
			uint64* Address = reinterpret_cast<uint64*>(Pointer);
			return Address ? (T*)(*Address) : nullptr;
		}

	private:
		void* Pointer;
	};
}
