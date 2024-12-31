#pragma once

#include "External/StandardLibrary.h"
#include "Memory/Memory.h"
#include "Debug/Logger/Assert.h"

#pragma warning(push)
#pragma warning(disable: 6387)
#pragma warning(disable: 28196)
#pragma warning(disable: 28251)

void* operator new(std::size_t Size)
{
	return NxEn::Memory::Allocate(Size);
}

void* operator new[](std::size_t Size)
{
	return NxEn::Memory::Allocate(Size);
}

void* operator new(std::size_t Size, std::align_val_t Align)
{
	NEXUS_ASSERT(false, Default, "Not supported new operator");
	return nullptr;
}

void* operator new[](std::size_t Size, std::align_val_t Align)
{
	NEXUS_ASSERT(false, Default, "Not supported new operator");
	return nullptr;
}

void* operator new(std::size_t Size, const std::nothrow_t& Tag) noexcept
{
	NEXUS_ASSERT(false, Default, "Not supported new operator");
	return nullptr;
}

void* operator new[](std::size_t Size, const std::nothrow_t& Tag) noexcept
{
	NEXUS_ASSERT(false, Default, "Not supported new operator");
	return nullptr;
}

void* operator new(std::size_t Size, std::align_val_t Alignement, const std::nothrow_t& Tag) noexcept
{
	NEXUS_ASSERT(false, Default, "Not supported new operator");
	return nullptr;
}

void* operator new[](std::size_t Size, std::align_val_t Alignement, const std::nothrow_t& Tag) noexcept
{
	NEXUS_ASSERT(false, Default, "Not supported new operator");
	return nullptr;
}

void operator delete(void* Pointer)
{
	NxEn::Memory::Free(Pointer);
}

void operator delete[](void* Pointer)
{
	NxEn::Memory::Free(Pointer);
}

void operator delete(void* Pointer, std::align_val_t Align)
{
	NEXUS_ASSERT(false, Default, "Not supported delete operator");
}

void operator delete[](void* Pointer, std::align_val_t Align)
{
	NEXUS_ASSERT(false, Default, "Not supported delete operator");
}

void operator delete(void* Pointer, std::size_t Size)
{
	NxEn::Memory::Free(Pointer);
}

void operator delete[](void* Pointer, std::size_t Size)
{
	NxEn::Memory::Free(Pointer);
}

void operator delete(void* Pointer, std::size_t Size, std::align_val_t Alignement)
{
	NEXUS_ASSERT(false, Default, "Not supported delete operator");
}

void operator delete[](void* Pointer, std::size_t Size, std::align_val_t Alignement)
{
	NEXUS_ASSERT(false, Default, "Not supported delete operator");
}

void operator delete(void* Pointer, const std::nothrow_t& Tag) noexcept
{
	NEXUS_ASSERT(false, Default, "Not supported delete operator");
}

void operator delete[](void* Pointer, const std::nothrow_t& Tag) noexcept
{
	NEXUS_ASSERT(false, Default, "Not supported delete operator");
}

void operator delete(void* Pointer, std::align_val_t Alignement, const std::nothrow_t& Tag) noexcept
{
	NEXUS_ASSERT(false, Default, "Not supported delete operator");
}

void operator delete[](void* Pointer, std::align_val_t Alignement, const std::nothrow_t& Tag) noexcept
{
	NEXUS_ASSERT(false, Default, "Not supported delete operator");
}

#pragma warning(pop)
