#pragma once 

#include "Core/NexusEngineCore.h"

namespace NxEn
{
	// Logger
	// There is globally available logger. It is also possible to create local logger.
	// Keep the Verbosity & Source enum sync with the const char array in the cpp file

	enum class LoggerVerbosity : uint8
	{
		Fatal	= 0,
		Error	= 1,
		Warning = 2,
		Info	= 3,
	};

	enum class LoggerSource : uint8
	{
		Engine	= 0,
		Editor	= 1,
		App		= 2,
		Project	= 3,
	};

	class Logger
	{
	public:
		NEXUS_ENGINE_API Logger(LoggerVerbosity Verbosity);
		NEXUS_ENGINE_API ~Logger();
		
		NEXUS_ENGINE_API void Log(LoggerSource Source, LoggerVerbosity Verbosity, uint16 Channel, const char* Message, ...) const;

		NEXUS_ENGINE_API void AddChannel(uint16 Channel, bool State = true);
		NEXUS_ENGINE_API void SetChannel(uint16 Channel, bool State);
		NEXUS_ENGINE_API bool HasChannel(uint16 Channel) const;
		NEXUS_ENGINE_API bool CheckChannel(uint16 Channel) const;

		NEXUS_ENGINE_API bool CheckVerbosity(LoggerVerbosity Verbosity) const;
		NEXUS_ENGINE_API void SetVerbosity(LoggerVerbosity Verbosity, bool State, bool All = false);

		NEXUS_ENGINE_API inline static Logger* GetInstance() { return Instance; }

	private:
		// TEMP: Replace int key by string once we have string
		Dictionary<uint16, bool> Channels;
		uint8 VerbosityMask;

		static Logger* Instance;
	};

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_LOG(Src, Vbs, Chn, Msg, ...) NxEn::Logger::GetInstance()->Log(NxEn::LoggerSource::Src, NxEn::LoggerVerbosity::Vbs, Chn, Msg, __VA_ARGS__);
#elif NEXUS_DISTRIB
	#define NEXUS_LOG(Src, Vbs, Chn, Msg, ...)
#endif
}