#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	// Time
	// Signleton that manage the global time of the Application (Now, Timestemp, ...)

	class Time
	{
	public:
		NEXUS_ENGINE_API uint64 GetTimeSinceEpoch() const;
		NEXUS_ENGINE_API Timestamp Now() const;

		NEXUS_ENGINE_API inline static Time* GetInstance() { return Instance; }

	private:
		Time();
		~Time();

		static Time* Instance;
	};
}

