#pragma once

// This file is the base the include for the others dll (Editor, App, Project), it should never be include into a .h/.cpp in the engine project.
// Add default / generic include that will be use for sure by others project.






#include "Core/NexusEngine.h"

#include "Types/Integer.h"
#include "Types/Enum.h"
#include "Types/Timestamp.h"
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
#include "Types/Containers/LinkedList.h"
#include "Types/Containers/Stack.h"
#include "Types/Containers/Queue.h"
#include "Types/Containers/Set.h"
#include "Types/Containers/Dictionary.h"
#include "Types/Containers/Tree.h"
#include "Types/Containers/Graph.h"
#include "Types/Containers/Collection.h"
#include "Types/Containers/Pool.h"
#include "Types/Containers/Tuple.h"

#include "Debug/Logger.h"
#include "Debug/Assert.h"
