#pragma once

// This file is the base the include for the others dll (Editor, App, Project), it should never be include into a .h/.cpp in the engine project.
// Add default / generic include that will be use for sure by others project.



#include "Core/NexusEngine.h"

#include "Types/Integer.h"
#include "Types/Enum.h"
#include "Types/Timestamp.h"
#include "Types/Containers/Array.h"
#include "Types/Containers/List.h"
#include "Types/Containers/Pool.h"
#include "Types/Containers/LinkedList.h"
#include "Types/Containers/Stack.h"
#include "Types/Containers/Queue.h"
#include "Types/Containers/Dequeue.h"

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