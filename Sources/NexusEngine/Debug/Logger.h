#pragma once 

#include "External/Intrinsics.h"
#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Types/Enum.h"
#include "Types/Timestamp.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"

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

		template<typename... Args>
		void Log(LoggerSource Source, LoggerVerbosity Verbosity, uint16 Channel, const StringView& Message, Args&&... args);

		NEXUS_ENGINE_API void AddChannel(uint16 Channel, bool State = true);
		NEXUS_ENGINE_API void SetChannel(uint16 Channel, bool State);
		NEXUS_ENGINE_API bool HasChannel(uint16 Channel) const;
		NEXUS_ENGINE_API bool CheckChannel(uint16 Channel) const;

		NEXUS_ENGINE_API bool CheckVerbosity(LoggerVerbosity Verbosity) const;
		NEXUS_ENGINE_API void SetVerbosity(LoggerVerbosity Verbosity, bool State);

		NEXUS_ENGINE_API inline static Logger* GetInstance() { return Instance; }

		NEXUS_ENUM_TO_STRING_DEFINITION(LoggerVerbosity)
		NEXUS_ENUM_TO_STRING_DEFINITION(LoggerSource)

	private:
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE bool ShouldPrint(LoggerVerbosity Verbosity, uint16 Channel) const;
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE uint8 GetLogLevel(LoggerVerbosity Verbosity) const;
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE void GatherInfo(int8 VerbosityLevel, LoggerSource Source, int8& Hours, int8& Minutes, int8 Seconds, StringView& SourceString, StringView& VerbosityString) const;
		NEXUS_ENGINE_API NEXUS_FORCE_INLINE void Print(const String& Message, uint8 Verbosity) const;

		const String Format = "[%02d:%02d:%02d][%7s][%7s][%i] %s\n";
		static Logger* Instance;

		// TEMP: Replace - String - Once we have custom string
		std::unordered_map<uint16, bool>* Channels;
		LoggerVerbosity VerbosityMask;
		String StringBuilderMessage;
		String StringBuilderFormat;
	};

	template<typename... Args>
	void Logger::Log(LoggerSource Source, LoggerVerbosity Verbosity, uint16 Channel, const StringView& Message, Args&&... args)
	{
		if (!ShouldPrint(Verbosity, Channel))
		{
			return;
		}

		uint8 VerbosityLevel = GetLogLevel(Verbosity);

		StringView SourceString = "", VerbosityString = "";
		int8 Hours = 0, Minutes = 0, Seconds = 0;
		GatherInfo(VerbosityLevel, Source, Hours, Minutes, Seconds, SourceString, VerbosityString);

		StringBuilderMessage.Format(Message, args...);
		StringBuilderFormat.Format(Format, Hours, Minutes, Seconds, SourceString.C(), VerbosityString.C(), Channel, StringBuilderMessage.C());

		Print(StringBuilderFormat, VerbosityLevel);
	}

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_LOG(Src, Vbs, Chn, Msg, ...) NxEn::Logger::GetInstance()->Log(NxEn::LoggerSource::Src, NxEn::LoggerVerbosity::Vbs, Chn, Msg, __VA_ARGS__);
#elif NEXUS_DISTRIB
	#define NEXUS_LOG(Src, Vbs, Chn, Msg, ...)
#endif
}
