#pragma once

// This file is the precompiled header.
// Include files that will be use in a lot of different cpp file (for header files look at NexusCore.h).
// Also, avoid to include files that will change often


#include "External/Intrinsics.h"
#include "External/StandardLibrary.h"

#include "Core/NexusEngine.h"

#include "Types/Integer.h"
#include "Types/Enum.h"
#include "Types/Timestamp.h"
#include "Types/Timespan.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringId.h"
#include "Types/Strings/StringView.h"
#include "Types/Strings/StringCApi.h"
#include "Types/Strings/StringFunctions.h"
#include "Types/Strings/StringTemplate.h"
#include "Types/Containers/Node.h"
#include "Types/Containers/Iterator.h"
#include "Types/Containers/Array.h"
#include "Types/Containers/List.h"
#include "Types/Containers/Dequeue.h"
#include "Types/Containers/Stack.h"
#include "Types/Containers/Queue.h"
#include "Types/Containers/LinkedList.h"
#include "Types/Containers/Set.h"
#include "Types/Containers/Dictionary.h"
#include "Types/Containers/Tree.h"
#include "Types/Containers/Graph.h"
#include "Types/Containers/Collection.h"
#include "Types/Containers/Buffer.h"
#include "Types/Containers/Pool.h"
#include "Types/Containers/Tuple.h"
#include "Types/Containers/ContainersUtils.h"
#include "Types/Delegate.h"

#include "Debug/Logger.h"
#include "Debug/Assert.h"

#include "Misc/References.h"
#include "Misc/Hash.h"
#include "Misc/Sort.h"
#include "Misc/Math.h"
#include "Misc/IO/Path.h"
#include "Misc/IO/File.h"
#include "Misc/IO/Directory.h"

#include "Memory/Memory.h"
#include "Memory/Handle/Handle.h"
#include "Memory/Handle/HandleManager.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/GlobalAllocator.h"
#include "Memory/Allocator/NexusAllocator.h"
#include "Memory/Allocator/StackAllocator.h"
#include "Memory/Allocator/HeapAllocator.h"
#include "Memory/Allocator/PoolAllocator.h"
