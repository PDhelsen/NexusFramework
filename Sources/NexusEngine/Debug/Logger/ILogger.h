#pragma once 

#include "Core/NexusEngineCore.h"
#include "Types/Numbers/Integer.h"
#include "Types/Numbers/Enum.h"
#include "Types/Strings/String.h"
#include "Types/Strings/StringView.h"
#include "Types/Strings/StringId.h"

namespace NxEn
{
	namespace LoggerChannel
	{
		NEXUS_ENGINE_API extern const StringId Default;
		NEXUS_ENGINE_API extern const StringId Verbose;
	}

	enum class LoggerVerbosity : uint8
	{
		None = 0,

		Fatal = 1 << 0,
		Error = 1 << 1,
		Warning = 1 << 2,
		Info = 1 << 3,

		All = Fatal | Error | Warning | Info,

		COUNT
	};
	NEXUS_ENUM_TO_FLAG(LoggerVerbosity)
	NEXUS_ENUM_TO_STRING_DEFINITION(LoggerVerbosity)

	enum class LoggerOutput : uint8
	{
		None = 0,

		Console = 1 << 0,
		IDE = 1 << 1,
		File = 1 << 2,

		All = Console | IDE | File,

		COUNT
	};
	NEXUS_ENUM_TO_FLAG(LoggerOutput)

	class ILogger
	{
	public:
		template<typename... Args>
		void Log(LoggerVerbosity Verbosity, StringId Channel, StringView Message, Args&&... args);

		NEXUS_ENGINE_API static ILogger* GetInstance();

	protected:
		NEXUS_ENGINE_API ILogger() = default;
		NEXUS_ENGINE_API ~ILogger() = default;

		NEXUS_ENGINE_API virtual bool ShouldLog(LoggerVerbosity Verbosity, StringId Channel) const = 0;
		NEXUS_ENGINE_API virtual String& GetMessage() = 0;
		NEXUS_ENGINE_API virtual void LogMessage(LoggerVerbosity Verbosity, StringId Channel) = 0;
	};

	template<typename... Args>
	void ILogger::Log(LoggerVerbosity Verbosity, StringId Channel, StringView Message, Args&&... args)
	{
		if (!ShouldLog(Verbosity, Channel))
		{
			return;
		}

		GetMessage().Format(Message, args...);
		LogMessage(Verbosity, Channel);
	}
}

#if NEXUS_DEBUG || NEXUS_RELEASE
#define NEXUS_LOG_INSTANCE(Instance, Vbs, Chn, Msg, ...) if (Instance) { Instance->Log(::NxEn::LoggerVerbosity::Vbs, ::NxEn::LoggerChannel::Chn, Msg, __VA_ARGS__); }

#define NEXUS_LOG(Vbs, Chn, Msg, ...) NEXUS_LOG_INSTANCE(::NxEn::ILogger::GetInstance(), Vbs, Chn, Msg, __VA_ARGS__)
#elif NEXUS_DISTRIB
#define NEXUS_LOG_INSTANCE(Instance, Vbs, Chn, Msg, ...)

#define NEXUS_LOG(Vbs, Chn, Msg, ...)
#endif
