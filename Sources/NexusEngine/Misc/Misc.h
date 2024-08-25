#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	// TODO: Implementation - Math - Math API

	NEXUS_ENGINE_API bool IsPowerOfTwo(uint64 Number);
	NEXUS_ENGINE_API int32 LogTwoPowerOfTwo(uint32 Number);

	NEXUS_ENGINE_API int8 Modulo(int8 Number, int8 Mod);
	NEXUS_ENGINE_API int16 Modulo(int16 Number, int16 Mod);
	NEXUS_ENGINE_API int32 Modulo(int32 Number, int32 Mod);
	NEXUS_ENGINE_API int64 Modulo(int64 Number, int64 Mod);
	NEXUS_ENGINE_API uint8 Modulo(uint8 Number, uint8 Mod);
	NEXUS_ENGINE_API uint16 Modulo(uint16 Number, uint16 Mod);
	NEXUS_ENGINE_API uint32 Modulo(uint32 Number, uint32 Mod);
	NEXUS_ENGINE_API uint64 Modulo(uint64 Number, uint64 Mod);
}
