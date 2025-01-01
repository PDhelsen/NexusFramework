#pragma once 

#include "External/Intrinsics.h"
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

	class Log
	{
	public:
		NEXUS_ENGINE_API static Log* GetInstance();

		template<typename... Args>
		void LogMessage(LoggerVerbosity Verbosity, StringId Channel, StringView Message, Args&&... args);

	protected:
		NEXUS_ENGINE_API Log() = default;
		NEXUS_ENGINE_API ~Log() = default;

		NEXUS_ENGINE_API virtual bool ShouldPrintMessage(LoggerVerbosity Verbosity, StringId Channel) const = 0;
		NEXUS_ENGINE_API virtual void PrintMessage(LoggerVerbosity Verbosity, StringId Channel) = 0;

		NEXUS_ENGINE_API virtual String& GetMessageBuffer() = 0;
	};

	template<typename... Args>
	void Log::LogMessage(LoggerVerbosity Verbosity, StringId Channel, StringView Message, Args&&... args)
	{
		if (!ShouldPrintMessage(Verbosity, Channel))
		{
			return;
		}

		GetMessageBuffer().Format(Message, args...);
		PrintMessage(Verbosity, Channel);
	}
}

#if NEXUS_DEBUG || NEXUS_RELEASE
#define NEXUS_LOG_INSTANCE(Instance, Vbs, Chn, Msg, ...) if (Instance) { Instance->LogMessage(::NxEn::LoggerVerbosity::Vbs, ::NxEn::LoggerChannel::Chn, Msg, __VA_ARGS__); }

#define NEXUS_LOG(Vbs, Chn, Msg, ...) NEXUS_LOG_INSTANCE(::NxEn::Log::GetInstance(), Vbs, Chn, Msg, __VA_ARGS__)
#elif NEXUS_DISTRIB
#define NEXUS_LOG_INSTANCE(Instance, Vbs, Chn, Msg, ...)

#define NEXUS_LOG(Vbs, Chn, Msg, ...)
#endif

#if NEXUS_DEBUG || NEXUS_RELEASE
#define NEXUS_ASSERT_INSTANCE(Instance, Condition, Channel, Msg, ...) if (!(Condition)) { \
	NEXUS_LOG_INSTANCE(Instance, Fatal, Channel, Msg, __VA_ARGS__); \
	NEXUS_LOG_INSTANCE(Instance, Fatal, Channel, "%s %s %d", NEXUS_FUNCTION_SIGNATURE, NEXUS_FILE_NAME, NEXUS_LINE_NUMBER); \
	NEXUS_DEBUGBREAK; }

#define NEXUS_ASSERT(Condition, Channel, Msg, ...) if (!(Condition)) { \
	NEXUS_LOG(Fatal, Channel, Msg, __VA_ARGS__); \
	NEXUS_LOG(Fatal, Channel, "%s %s %d", NEXUS_FUNCTION_SIGNATURE, NEXUS_FILE_NAME, NEXUS_LINE_NUMBER); \
	NEXUS_DEBUGBREAK; }
#elif NEXUS_DISTRIB
#define NEXUS_ASSERT_INSTANCE(Instance, Condition, Msg, ...)

#define NEXUS_ASSERT(Condition, Msg, ...)
#endif

#define NEXUS_ASSERT_STATIC(condition, Msg) static_assert(condition, Msg)
