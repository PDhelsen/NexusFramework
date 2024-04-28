#pragma once

// This file is the precompiled header.
// Include files that will be use in a lot of different cpp file (for header files look at NexusCore.h).
// Also, avoid to include files that will change often


#include "Core/NexusEngine.h"

#include "Types/Integer.h"
#include "Types/Enum.h"
#include "Types/Timestamp.h"
#include "Types/Containers/Array.h"
#include "Types/Containers/List.h"
#include "Types/Containers/Pool.h"
#include "Types/Containers/LinkedList.h"

#include "Memory/Memory.h"
#include "Memory/Handle/Handle.h"
#include "Memory/Handle/HandleManager.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/StackAllocator.h"
#include "Memory/Allocator/HeapAllocator.h"
#include "Memory/Allocator/PoolAllocator.h"

#include "Debug/Logger.h"
#include "Debug/Assert.h"

#include "Misc/References.h"
#include "Misc/Misc.h"
#include "Misc/Sort.h"