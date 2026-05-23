#pragma once

#include "NexusFramework/External/Intrinsics.h"
#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Numeric/Enum.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Types/Strings/Utility.h"

namespace NxFr
{
	class Log;
	namespace Globals
	{
		NX_FRAMEWORK_API extern Log* GetLogger();
	}

	namespace LoggerChannel
	{
		inline const StringId Default = "Default"_Sid;
		inline const StringId Verbose = "Verbose"_Sid;
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

	class NX_FRAMEWORK_API Log
	{
	public:
		template<typename... Args>
		void LogMessage(LoggerVerbosity Verbosity, StringId Channel, StringView Message, Args&&... args)
		{
			StringUtility::Format(GetBuffer(), Message, args...);
			PrintLog(Verbosity, Channel, Message);
		}

	protected:
		static String& GetBuffer();

		virtual void PrintLog(LoggerVerbosity Verbosity, StringId Channel, StringView Message) = 0;
	};
}

NX_FLAG(NxFr::LoggerVerbosity, uint8)
NX_FLAG_STRING(NxFr::LoggerVerbosity, 4, "Fatal", "Error", "Warning", "Info")
NX_FLAG(NxFr::LoggerOutput, uint8)
NX_FLAG_STRING(NxFr::LoggerOutput, 4, "Fatal", "Error", "Warning", "Info")

#if NX_DEBUG || NX_RELEASE
#define NX_LOG_INSTANCE(Instance, Vbs, Chn, Msg, ...) if (Instance) { Instance->LogMessage(::NxFr::LoggerVerbosity::Vbs, ::NxFr::LoggerChannel::Chn, Msg, __VA_ARGS__); }

#define NX_LOG(Vbs, Chn, Msg, ...) NX_LOG_INSTANCE(::NxFr::Globals::GetLogger(), Vbs, Chn, Msg, __VA_ARGS__)
#elif NX_DISTRIB
#define NX_LOG_INSTANCE(Instance, Vbs, Chn, Msg, ...)

#define NX_LOG(Vbs, Chn, Msg, ...)
#endif

#if NX_DEBUG || NX_RELEASE
#define NX_ASSERT_INSTANCE(Instance, Condition, Channel, Msg, ...) if (!(Condition)) { \
	NX_LOG_INSTANCE(Instance, Fatal, Channel, Msg, __VA_ARGS__); \
	NX_LOG_INSTANCE(Instance, Fatal, Channel, "%s %s %d", NX_FUNCTION_SIGNATURE, NX_FILE_NAME, NX_LINE_NUMBER); \
	NX_DEBUGBREAK; }

#define NX_ASSERT(Condition, Channel, Msg, ...) if (!(Condition)) { \
	NX_LOG(Fatal, Channel, Msg, __VA_ARGS__); \
	NX_LOG(Fatal, Channel, "%s %s %d", NX_FUNCTION_SIGNATURE, NX_FILE_NAME, NX_LINE_NUMBER); \
	NX_DEBUGBREAK; }
#elif NX_DISTRIB
#define NX_ASSERT_INSTANCE(Instance, Condition, Msg, ...)

#define NX_ASSERT(Condition, Msg, ...)
#endif

#define NX_ASSERT_STATIC(condition, Msg) static_assert(condition, Msg)
