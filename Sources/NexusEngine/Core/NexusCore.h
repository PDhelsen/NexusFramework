#pragma once

// This file regroup the generic header file that needs to included into others header to work properly.
// This mainly concern file that either define macros or generic / standard type (int, string, containers, ... )
// Also, pay attention to avoid circular dependency. The headers included here should not include header that include this file.
// Keep this file to a minimun !!!

#include "Types/Types.h"

#include "Core/NexusEngine.h"