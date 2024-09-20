#pragma once

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	class Time
	{
	public:
		NEXUS_ENGINE_API int64 GetTimeSinceEpoch() const;
		NEXUS_ENGINE_API Timestamp Now() const;

		NEXUS_ENGINE_API static Time* GetInstance() { static Time* Instance = new Time(); return Instance; }

	private:
		Time();
		~Time();
	};
}

