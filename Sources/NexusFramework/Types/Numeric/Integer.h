#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/External/StandardLibrary.h"

namespace NxFr
{
	using GUID = uint64;
	using Byte = uint8;

	namespace Integer
	{
		inline constexpr int8 MinI8 = NEXUS_INTERGER_MIN_I8;
		inline constexpr int16 MinI16 = NEXUS_INTERGER_MIN_I16;
		inline constexpr int32 MinI32 = NEXUS_INTERGER_MIN_I32;
		inline constexpr int64 MinI64 = NEXUS_INTERGER_MIN_I64;
		inline constexpr uint8 MinUI8 = NEXUS_INTERGER_MIN_UI8;
		inline constexpr uint16 MinUI16 = NEXUS_INTERGER_MIN_UI16;
		inline constexpr uint32 MinUI32 = NEXUS_INTERGER_MIN_UI32;
		inline constexpr uint64 MinUI64 = NEXUS_INTERGER_MIN_UI64;
		inline constexpr int8 MaxI8 = NEXUS_INTERGER_MAX_I8;
		inline constexpr int16 MaxI16 = NEXUS_INTERGER_MAX_I16;
		inline constexpr int32 MaxI32 = NEXUS_INTERGER_MAX_I32;
		inline constexpr int64 MaxI64 = NEXUS_INTERGER_MAX_I64;
		inline constexpr uint8 MaxUI8 = NEXUS_INTERGER_MAX_UI8;
		inline constexpr uint16 MaxUI16 = NEXUS_INTERGER_MAX_UI16;
		inline constexpr uint32 MaxUI32 = NEXUS_INTERGER_MAX_UI32;
		inline constexpr uint64 MaxUI64 = NEXUS_INTERGER_MAX_UI64;

		template<typename T>
		bool CheckBit(T Value, T Offset) { return Value & (T(1) << Offset); }
		template<typename T>
		T SetBit1(T Value, T Offset) { return Value |= T(1) << Offset; }
		template<typename T>
		T SetBit0(T Value, T Offset) { return Value &= ~(T(1) << Offset); }
		template<typename T>
		T SetBit(T Value, T Offset, bool State) { return State ? SetBit1(Value, Offset) : SetBit0(Value, Offset); }
		template<typename T>
		bool CheckFlag(T Value, T Flag) { return Value & Flag; }
		template<typename T>
		T SetFlag1(T Value, T Flag) { return Value |= Flag; }
		template<typename T>
		T SetFlag0(T Value, T Flag) { return Value &= ~(Flag); }
		template<typename T>
		T SetFlag(T Value, T Flag, bool State) { return State ? SetFlag1(Value, Flag) : SetFlag0(Value, Flag); }

		NEXUS_FRAMEWORK_API GUID GenerateGuid();
		NEXUS_FRAMEWORK_API GUID GenerateGuid(uint64 Time, uint64 ProcessId, uint64 ThreadId, uint64 Seed = 0);
	}
}
