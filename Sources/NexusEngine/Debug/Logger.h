#pragma once 

#include "Core/NexusCore.h"

namespace NxEn
{
	class Logger
	{
	public:
		// The log format and the enum to strings are defined by const char* in the cpp file.
		// The enum declaration and the const char array in the cpp should match.

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
		
		NEXUS_ENGINE_API void Log(Source Source, Verbosity Verbosity, uint16 Channel, const char* Message, ...);

		NEXUS_ENGINE_API void AddChannel(uint16 Channel, bool State = false);
		NEXUS_ENGINE_API void SetChannel(uint16 Channel, bool State);

		NEXUS_ENGINE_API inline Verbosity GetVerbosity() const { return VerbosityLevel; }
		NEXUS_ENGINE_API inline void SetVerbosity(Verbosity Verbosity) { VerbosityLevel = Verbosity; }

		NEXUS_ENGINE_API inline static Logger* GetInstance() { return Instance; }

	private:
		inline bool HasChannel(uint16 Channel) const;
		inline bool CheckChannel(uint16 Channel) const;
		inline bool CheckVerbosityLevel(Verbosity Verbosity) const;
		inline const char* VerbosityToString(Verbosity Verbosity) const;
		inline const char* SourceToString(Source Source) const;
		inline const char* Format(const char* Message, const char* Date, const char* Source, const char* Verbosity, uint16 Channel) const;

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