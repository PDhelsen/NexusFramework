#pragma once

// This file is the base the include for the others dll (Editor, App, Project), it should never be include into a .h/.cpp in the engine project.
// Add default / generic include that will be use for sure by others project.





#include "Types/Integer.h"
#include "Types/Flags.h"
#include "Types/Timestamp.h"

#include "Memory/Memory.h"
#include "Memory/MemoryOperatorDeclaration.h"
#include "Memory/Handle/Handle.h"

#include "Core/NexusEngine.h"


#include "Misc/Misc.h"

#include "Application/Time.h"

#include "Debug/Logger.h"
#include "Debug/Assert.h"