#pragma once 

#include "Core/NexusCore.h"
#include "External/StandardLibrary.h"

namespace NxEn
{
	// Logger
	// There is globally available logger. It is also possible to create local logger.
	// Keep the Verbosity & Source enum sync with the const char array in the cpp file

	class Logger
	{
	public:
		// TODO: Change for flag instead of simple enum
		enum class Verbosity : uint8
		{
			Fatal,
			Error,
			Warning,
			Info
		};

		enum class Source: uint8
		{
			Engine,
			Editor,
			App,
			Project
		};

		NEXUS_ENGINE_API Logger(Verbosity Verbosity);
		NEXUS_ENGINE_API ~Logger();
		
		NEXUS_ENGINE_API void Log(Source Source, Verbosity Verbosity, uint16 Channel, const char* Message, ...) const;

		NEXUS_ENGINE_API void AddChannel(uint16 Channel, bool State = true);
		NEXUS_ENGINE_API void SetChannel(uint16 Channel, bool State);
		NEXUS_ENGINE_API bool HasChannel(uint16 Channel) const;
		NEXUS_ENGINE_API bool CheckChannel(uint16 Channel) const;

		NEXUS_ENGINE_API inline Verbosity GetVerbosity() const { return VerbosityLevel; }
		NEXUS_ENGINE_API inline void SetVerbosity(Verbosity Verbosity) { VerbosityLevel = Verbosity; }

		NEXUS_ENGINE_API inline static Logger* GetInstance() { return Instance; }

	private:
		// TEMP: Replace int key by string once we have string
		Dictionary<uint16, bool> Channels;
		Verbosity VerbosityLevel;

		static Logger* Instance;
	};

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_LOG(Src, Vbs, Chn, Msg, ...) NxEn::Logger::GetInstance()->Log(NxEn::Logger::Source::Src, NxEn::Logger::Verbosity::Vbs, Chn, Msg, __VA_ARGS__);
#elif NEXUS_DISTRIB
	#define NEXUS_LOG(Src, Vbs, Chn, Msg, ...)
#endif
}