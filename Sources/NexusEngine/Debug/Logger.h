#pragma once 

#include "Core/NexusEngine.h"
#include "Types/Integer.h"
#include "Types/Enum.h"
#include "Types/Timestamp.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"

namespace NxEn
{
	class Fnv1a64;
	template <typename K, typename T, class H> class Dictionary;

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
		NEXUS_ENGINE_API Logger(const Logger& Other) = delete;
		NEXUS_ENGINE_API Logger(Logger&& Other) noexcept = delete;
		NEXUS_ENGINE_API ~Logger();

		NEXUS_ENGINE_API Logger& operator=(const Logger& Other) = delete;
		NEXUS_ENGINE_API Logger& operator=(Logger&& Other) noexcept = delete;

		template<typename... Args>
		void Log(LoggerSource Source, LoggerVerbosity Verbosity, const String& Channel, StringView Message, Args&&... args);

		NEXUS_ENGINE_API void AddChannel(const String& Channel, bool State = true);
		NEXUS_ENGINE_API void SetChannel(const String& Channel, bool State);
		NEXUS_ENGINE_API bool HasChannel(const String& Channel) const;
		NEXUS_ENGINE_API bool CheckChannel(const String& Channel) const;

		NEXUS_ENGINE_API bool CheckVerbosity(LoggerVerbosity Verbosity) const;
		NEXUS_ENGINE_API void SetVerbosity(LoggerVerbosity Verbosity, bool State);

		NEXUS_ENGINE_API inline static Logger* GetInstance() { return Instance; }

		NEXUS_ENUM_TO_STRING_DEFINITION(LoggerVerbosity)
		NEXUS_ENUM_TO_STRING_DEFINITION(LoggerSource)

	private:
		NEXUS_ENGINE_API inline bool ShouldPrint(LoggerVerbosity Verbosity, const String& Channel) const;
		NEXUS_ENGINE_API inline uint8 GetLogLevel(LoggerVerbosity Verbosity) const;
		NEXUS_ENGINE_API inline void GatherInfo(int8 VerbosityLevel, LoggerSource Source, int8& Hours, int8& Minutes, int8 Seconds, StringView& SourceString, StringView& VerbosityString) const;
		NEXUS_ENGINE_API inline void Print(StringView Message, uint8 Verbosity) const;

		inline static const String Format = "[%02d:%02d:%02d][%7s][%7s][%s] %s\n";
		static Logger* Instance;

		Dictionary<String, bool, Fnv1a64>* Channels;
		LoggerVerbosity VerbosityMask;
		String StringBuilderMessage;
		String StringBuilderFormat;
	};

	template<typename... Args>
	void Logger::Log(LoggerSource Source, LoggerVerbosity Verbosity, const String& Channel, StringView Message, Args&&... args)
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
		StringBuilderFormat.Format(Format, Hours, Minutes, Seconds, SourceString.C(), VerbosityString.C(), Channel.C(), StringBuilderMessage.C());

		Print(StringBuilderFormat, VerbosityLevel);
	}

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_LOG(Src, Vbs, Chn, Msg, ...) NxEn::Logger::GetInstance()->Log(NxEn::LoggerSource::Src, NxEn::LoggerVerbosity::Vbs, Chn, Msg, __VA_ARGS__);
#elif NEXUS_DISTRIB
	#define NEXUS_LOG(Src, Vbs, Chn, Msg, ...)
#endif
}
