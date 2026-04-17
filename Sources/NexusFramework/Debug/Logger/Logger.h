#pragma once

#include "NexusFramework/Debug/Logger/Log.h"
#include "NexusFramework/Types/Containers/Array.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Types/Functions/Event.h"
#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/IO/Stream.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	class Logger : public Log
	{
	private:
		struct LogData
		{
			LogData(LoggerVerbosity Verbosity, StringId Channel);

			String Message;
			StringId Channel;
			LoggerVerbosity Verbosity;
		};

	public:
		inline static const String Format = "[%02d:%02d:%02d][%7s][%s] %s%s";

		NEXUS_FRAMEWORK_API static Logger* GetInstance();

		NEXUS_FRAMEWORK_API Logger(LoggerVerbosity Verbosity, LoggerOutput Output, StringView Path = "", bool AutoFlush = false);
		NEXUS_FRAMEWORK_API Logger(const Logger& Other) = delete;
		NEXUS_FRAMEWORK_API Logger(Logger&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API ~Logger();

		NEXUS_FRAMEWORK_API Logger& operator=(const Logger& Other) = delete;
		NEXUS_FRAMEWORK_API Logger& operator=(Logger&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API void Flush();

		NEXUS_FRAMEWORK_API bool CheckVerbosity(LoggerVerbosity Verbosity) const;
		NEXUS_FRAMEWORK_API void SetVerbosity(LoggerVerbosity Verbosity, bool State);

		NEXUS_FRAMEWORK_API void AddChannel(StringId Channel, bool State = true);
		NEXUS_FRAMEWORK_API void SetChannel(StringId Channel, bool State);
		NEXUS_FRAMEWORK_API void SetAllChannels(bool State);
		NEXUS_FRAMEWORK_API bool HasChannel(StringId Channel) const;
		NEXUS_FRAMEWORK_API bool CheckChannel(StringId Channel) const;
		NEXUS_FRAMEWORK_API Array<StringId> GetChannels() const;

		NEXUS_FRAMEWORK_API bool CheckOutput(LoggerOutput Output) const;
		NEXUS_FRAMEWORK_API void SetOutput(LoggerOutput Output, bool State, StringView Path = "");
		NEXUS_FRAMEWORK_API void RegisterCallback(const Delegate<void(LoggerVerbosity, StringId, StringView)>& Callback);
		NEXUS_FRAMEWORK_API void UnregisterCallback(const Delegate<void(LoggerVerbosity, StringId, StringView)>& Callback);

		NEXUS_FRAMEWORK_API bool GetAutoFlush() const;
		NEXUS_FRAMEWORK_API void SetAutoFlush(bool Auto);

	protected:
		NEXUS_FRAMEWORK_API String& GetBuffer() override;
		NEXUS_FRAMEWORK_API void PrintLog(LoggerVerbosity Verbosity, StringId Channel, StringView Message) override;
		NEXUS_FRAMEWORK_API void FlushLogs() override;

	private:
		List<LogData> Logs;
		Dictionary<StringId, bool, Hashing::Default> Channels;
		LoggerVerbosity VerbosityMask;
		LoggerOutput Outputs;

		Platform* Target;
		TextStream Stream;
		Event<LoggerVerbosity, StringId, StringView> Callback;

		bool AutoFlush;

		Mutex Guard;
	};
}
