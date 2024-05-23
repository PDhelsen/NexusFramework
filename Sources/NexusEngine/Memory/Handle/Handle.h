#pragma once

namespace NxEn
{
	// Struct that store a pointer to global array of data pointer and forward a the data pointer
	// Allow to move data in memory without losing reference to it.
	// Needed for heap defragmentation

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
		inline bool operator==(const Handle<T>& Other) { return Pointer == Other.Pointer; }
		inline bool operator!=(const Handle<T>& Other) { return Pointer != Other.Pointer; }

	private:
		Handle() = default;

		void* Pointer = nullptr;
	};
}