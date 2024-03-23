#pragma once

// This file is the precompiled header.
// Include files that will be use in a lot of different cpp file (for header files look at NexusCore.h).
// Also, avoid to include files that will change often

#include "External/StandardLibrary.h"

#include "Types/Integer.h"
#include "Types/Flags.h"
#include "Types/Timestamp.h"

#include "Memory/Memory.h"
#include "Memory/MemoryOperatorDeclaration.h"
#include "Memory/Allocator/StackAllocator.h"
#include "Memory/Allocator/HeapAllocator.h"
#include "Memory/Allocator/PoolAllocator.h"
#include "Memory/Handle/Handle.h"

#include "Core/NexusEngine.h"

#include "Application/Time.h"

#include "Debug/Logger.h"
#include "Debug/Assert.h"