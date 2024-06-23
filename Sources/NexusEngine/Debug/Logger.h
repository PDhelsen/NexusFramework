#pragma once 

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Types/Enum.h"

// TEMP: Remove - Container - Once we have custom dictionary
#include "External/StandardLibrary.h"

namespace NxEn
{
	// Logger
	// There is globally available logger. It is also possible to create local logger.
	// Keep the Verbosity & Source enum sync with the const char array in the cpp file

	enum class LoggerVerbosity : uint8
	{
		None	= 0,

		Fatal	= 1 << 0,
		Error	= 1 << 1,
		Warning = 1 << 2,
		Info	= 1 << 3,

		All = Fatal | Error | Warning | Info,

		COUNT
	};
	NEXUS_FLAG(LoggerVerbosity, uint8)

	enum class LoggerSource : uint8
	{
		Engine	= 0,
		Editor	= 1,
		App		= 2,
		Project	= 3,
		COUNT
	};

	class Logger
	{
	public:
		NEXUS_ENGINE_API Logger(LoggerVerbosity Verbosity);
		NEXUS_ENGINE_API ~Logger();

		// TODO: Implementation - Logger - Support for String
		NEXUS_ENGINE_API void Log(LoggerSource Source, LoggerVerbosity Verbosity, uint16 Channel, const char* Message, ...) const;

		NEXUS_ENGINE_API void AddChannel(uint16 Channel, bool State = true);
		NEXUS_ENGINE_API void SetChannel(uint16 Channel, bool State);
		NEXUS_ENGINE_API bool HasChannel(uint16 Channel) const;
		NEXUS_ENGINE_API bool CheckChannel(uint16 Channel) const;

		NEXUS_ENGINE_API bool CheckVerbosity(LoggerVerbosity Verbosity) const;
		NEXUS_ENGINE_API void SetVerbosity(LoggerVerbosity Verbosity, bool State);

		NEXUS_ENGINE_API inline static Logger* GetInstance() { return Instance; }

		NEXUS_ENGINE_API NEXUS_ENUM_TO_STRING_DEFINITION(LoggerVerbosity)
		NEXUS_ENGINE_API NEXUS_ENUM_TO_STRING_DEFINITION(LoggerSource)

	private:
		// TEMP: Replace - String - Once we have custom string
		std::unordered_map<uint16, bool>* Channels;
		LoggerVerbosity VerbosityMask;

		static Logger* Instance;
	};

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_LOG(Src, Vbs, Chn, Msg, ...) NxEn::Logger::GetInstance()->Log(NxEn::LoggerSource::Src, NxEn::LoggerVerbosity::Vbs, Chn, Msg, __VA_ARGS__);
#elif NEXUS_DISTRIB
	#define NEXUS_LOG(Src, Vbs, Chn, Msg, ...)
#endif
}
