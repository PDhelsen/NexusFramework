#pragma once

// This file regroup the generic header file that needs to included into others header to work properly (for cpp files look at NexusPch.h).
// This mainly concern file that either define macros or generic / standard type (int, string, containers, ... )
// Also, pay attention to avoid circular dependency. The headers included here should not include header that include this file.
// Keep this file to a minimun !!!

#include "Core/NexusEngine.h"

#include "Types/Integer.h"
#include "Types/Enum.h"
#include "Types/Timestamp.h"

#include "Memory/Memory.h"
#include "Memory/MemoryOperatorDeclaration.h"
#include "Memory/Handle/Handle.h"


#include "Debug/Logger.h"
#include "Debug/Assert.h"