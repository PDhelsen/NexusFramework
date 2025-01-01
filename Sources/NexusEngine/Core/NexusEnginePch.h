#pragma once

// This file is the precompiled header.
// Include files that will be use in a lot of different cpp file (for header files look at NexusCore.h).
// Also, avoid to include files that will change often


#include "Core/NexusEngineCore.h"

#include "Types/Numbers/Integer.h"
#include "Types/Numbers/Decimal.h"
#include "Types/Numbers/Enum.h"
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
#include "Types/Functions/Delegate.h"
#include "Types/Functions/Event.h"

#include "Debug/Logger/Logger.h"
