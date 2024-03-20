#pragma once

// This file regroup the generic header file that needs to included into others header to work properly (for cpp files look at NexusPch.h).
// This mainly concern file that either define macros or generic / standard type (int, string, containers, ... )
// Also, pay attention to avoid circular dependency. The headers included here should not include header that include this file.
// Keep this file to a minimun !!!

#include "External/StandardLibrary.h"

#include "Types/Integer.h"
#include "Types/Flags.h"
#include "Types/Timestamp.h"

#include "Memory/Memory.h"
#include "Memory/MemoryOperatorDeclaration.h"
#include "Memory/Allocator/StackAllocator.h"
#include "Memory/Allocator/HeapAllocator.h"
#include "Memory/Allocator/PoolAllocator.h"

#include "Core/NexusEngine.h"