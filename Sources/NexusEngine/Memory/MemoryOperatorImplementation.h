#pragma once

#include "Core/NexusEngineCore.h"

#pragma warning(push)
#pragma warning(disable: 6387)
#pragma warning(disable: 28196)
#pragma warning(disable: 28251)

void* operator new(size_t Size)
{
	return NxEn::Memory::Malloc(Size);
}

void* operator new[](size_t Size)
{
	return NxEn::Memory::Malloc(Size);
}

void operator delete(void* Memory)
{
	NxEn::Memory::Free(Memory);
}

void operator delete[](void* Memory)
{
	NxEn::Memory::Free(Memory);
}

#pragma warning(pop)