#pragma once

// This file regroup the generic header file that needs to included into others header to work properly (for cpp files look at NexusPch.h).
// This mainly concern file that either define macros or generic / standard type (int, string, containers, ... )
// Also, pay attention to avoid circular dependency. The headers included here should not include header that include this file.
// Keep this file to a minimun !!!




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
// #include "Types/Containers/ContainersUtils.h"
#include "Types/Delegate.h"

#include "Debug/Logger.h"
#include "Debug/Assert.h"
