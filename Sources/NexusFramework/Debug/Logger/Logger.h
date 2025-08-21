#pragma once

#include "NexusFramework/Debug/Logger/Log.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/IO/File.h"

namespace NxFr
{
	class Logger : public Log
	{
	public:
		inline static const String Format = "[%02d:%02d:%02d][%7s][%s] %s\n";

		NEXUS_FRAMEWORK_API static Logger* GetInstance();

		NEXUS_FRAMEWORK_API Logger(bool FlushOnLog, LoggerVerbosity Verbosity, LoggerOutput Output, StringView Path = "");
		NEXUS_FRAMEWORK_API Logger(const Logger& Other) = delete;
		NEXUS_FRAMEWORK_API Logger(Logger&& Other) noexcept = delete;
		NEXUS_FRAMEWORK_API ~Logger();

		NEXUS_FRAMEWORK_API Logger& operator=(const Logger& Other) = delete;
		NEXUS_FRAMEWORK_API Logger& operator=(Logger&& Other) noexcept = delete;

		NEXUS_FRAMEWORK_API void Flush();

		NEXUS_FRAMEWORK_API void AddChannel(StringId Channel, bool State = true);
		NEXUS_FRAMEWORK_API void SetChannel(StringId Channel, bool State);
		NEXUS_FRAMEWORK_API void SetAllChannels(bool State);
		NEXUS_FRAMEWORK_API bool HasChannel(StringId Channel) const;
		NEXUS_FRAMEWORK_API bool CheckChannel(StringId Channel) const;
		NEXUS_FRAMEWORK_API Array<StringId> GetChannels() const;

		NEXUS_FRAMEWORK_API bool CheckVerbosity(LoggerVerbosity Verbosity) const;
		NEXUS_FRAMEWORK_API void SetVerbosity(LoggerVerbosity Verbosity, bool State);

		NEXUS_FRAMEWORK_API bool CheckOutput(LoggerOutput Output) const;

	protected:
		NEXUS_FRAMEWORK_API bool ShouldPrintMessage(LoggerVerbosity Verbosity, StringId Channel) const override;
		NEXUS_FRAMEWORK_API void PrintMessage(LoggerVerbosity Verbosity, StringId Channel) override;

		NEXUS_FRAMEWORK_API String& GetMessageBuffer() override { return BufferMessage; };

	private:
		inline uint8 GetLogLevel(LoggerVerbosity Verbosity) const;
		inline void GatherInfo(int8 VerbosityLevel, StringView& VerbosityString, int8& Hours, int8& Minutes, int8& Seconds) const;
		inline void CopyIntoBuffer(String& Text);
		inline void Write(String& Text);

		Dictionary<StringId, bool, Hashing::Default> Channels;
		LoggerVerbosity VerbosityMask;
		LoggerOutput Outputs;
		bool FlushOnLog;

		String BufferMessage;
		String BufferFormat;
		String BufferLogs;

		Platform* Target;
		File Handle;
	};
}
