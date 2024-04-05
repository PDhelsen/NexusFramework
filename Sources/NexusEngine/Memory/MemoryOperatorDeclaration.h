#pragma once

// Globaly override the new and delete operator to use Pointer
// Each dll should include in one cpp file the MemoryOperatorImplementation 
// because the new / delete operator cannot be dynamicly link, so the implementation have to present in each dll

#include "External/StandardLibrary.h"

#pragma warning(push)
#pragma warning(disable: 6387)
#pragma warning(disable: 28196)
#pragma warning(disable: 28251)

void* operator new(std::size_t Size);
void* operator new[](std::size_t Size);
void* operator new(std::size_t Size, std::align_val_t Align);
void* operator new[](std::size_t Size, std::align_val_t Align);
void* operator new(std::size_t Size, const std::nothrow_t& Tag) noexcept;
void* operator new[](std::size_t Size, const std::nothrow_t& Tag) noexcept;
void* operator new(std::size_t Size, std::align_val_t Alignement, const std::nothrow_t& Tag) noexcept;
void* operator new[](std::size_t Size, std::align_val_t Alignement, const std::nothrow_t& Tag) noexcept;

void operator delete(void* Pointer);
void operator delete[](void* Pointer);
void operator delete(void* Pointer, std::align_val_t Align);
void operator delete[](void* Pointer, std::align_val_t Align);
void operator delete(void* Pointer, std::size_t sz);
void operator delete[](void* Pointer, std::size_t sz);
void operator delete(void* Pointer, std::size_t sz, std::align_val_t Alignement);
void operator delete[](void* Pointer, std::size_t sz, std::align_val_t Alignement);
void operator delete(void* Pointer, const std::nothrow_t& Tag) noexcept;
void operator delete[](void* Pointer, const std::nothrow_t& Tag) noexcept;
void operator delete(void* Pointer, std::align_val_t Alignement, const std::nothrow_t& Tag) noexcept;
void operator delete[](void* Pointer, std::align_val_t Alignement, const std::nothrow_t& Tag) noexcept;

#pragma warning(pop)
