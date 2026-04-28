#pragma once

#include "NexusFramework/External/Intrinsics.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Numeric/Enum.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Types/Strings/StringFunctions.h"
#include "NexusFramework/Types/Strings/StringTemplate.h"
#include "NexusFramework/Math/Math.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		NEXUS_FRAMEWORK_API extern const StringId Default;
		NEXUS_FRAMEWORK_API extern const StringId Verbose;
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

	enum class LoggerOutput : uint8
	{
		None = 0,

		Console = 1 << 0,
		IDE = 1 << 1,
		File = 1 << 2,
		Callback = 1 << 3,

		All = Console | IDE | File | Callback,

		COUNT
	};

	class Log
	{
	public:
		NEXUS_FRAMEWORK_API static Log* GetInstance();

		template<typename... Args>
		void LogMessage(LoggerVerbosity Verbosity, StringId Channel, StringView Message, Args&&... args);

	protected:
		NEXUS_FRAMEWORK_API Log() = default;
		NEXUS_FRAMEWORK_API ~Log() = default;

		NEXUS_FRAMEWORK_API virtual String& GetBuffer() = 0;
		NEXUS_FRAMEWORK_API virtual void PrintLog(LoggerVerbosity Verbosity, StringId Channel, StringView Message) = 0;
		NEXUS_FRAMEWORK_API virtual void FlushLogs() = 0;

	};

	template<typename... Args>
	void Log::LogMessage(LoggerVerbosity Verbosity, StringId Channel, StringView Message, Args&&... args)
	{
		GetBuffer().Format(Message, args...);
		PrintLog(Verbosity, Channel, Message);
	}
}

NEXUS_FLAG(NxFr::LoggerVerbosity, uint8)
NEXUS_FLAG_STRING(NxFr::LoggerVerbosity, 4, "Fatal", "Error", "Warning", "Info")
NEXUS_FLAG(NxFr::LoggerOutput, uint8)
NEXUS_FLAG_STRING(NxFr::LoggerOutput, 4, "Fatal", "Error", "Warning", "Info")

#if NEXUS_DEBUG || NEXUS_RELEASE
#define NEXUS_LOG_INSTANCE(Instance, Vbs, Chn, Msg, ...) if (Instance) { Instance->LogMessage(::NxFr::LoggerVerbosity::Vbs, ::NxFr::LoggerChannel::Chn, Msg, __VA_ARGS__); }

#define NEXUS_LOG(Vbs, Chn, Msg, ...) NEXUS_LOG_INSTANCE(::NxFr::Log::GetInstance(), Vbs, Chn, Msg, __VA_ARGS__)
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
