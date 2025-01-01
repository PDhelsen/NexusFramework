#pragma once 

#include "Debug/Logger/Log.h"
#include "Types/Containers/Dictionary.h"
#include "Platform/Platform.h"
#include "IO/File.h"

namespace NxEn
{
	class Logger : public Log
	{
	public:
		NEXUS_ENGINE_API Logger(bool FlushOnLog, LoggerVerbosity Verbosity, LoggerOutput Output, StringView Path = "");
		NEXUS_ENGINE_API Logger(const Logger& Other) = delete;
		NEXUS_ENGINE_API Logger(Logger&& Other) noexcept = delete;
		NEXUS_ENGINE_API ~Logger();

		NEXUS_ENGINE_API Logger& operator=(const Logger& Other) = delete;
		NEXUS_ENGINE_API Logger& operator=(Logger&& Other) noexcept = delete;

		NEXUS_ENGINE_API void AddChannel(StringId Channel, bool State = true);
		NEXUS_ENGINE_API void SetChannel(StringId Channel, bool State);
		NEXUS_ENGINE_API bool HasChannel(StringId Channel) const;
		NEXUS_ENGINE_API bool CheckChannel(StringId Channel) const;

		NEXUS_ENGINE_API bool CheckVerbosity(LoggerVerbosity Verbosity) const;
		NEXUS_ENGINE_API void SetVerbosity(LoggerVerbosity Verbosity, bool State);

		NEXUS_ENGINE_API bool CheckOutput(LoggerOutput Output) const;

		NEXUS_ENGINE_API void Flush();

		NEXUS_ENGINE_API static Logger* GetInstance();

	protected:
		NEXUS_ENGINE_API bool ShouldLogMessage(LoggerVerbosity Verbosity, StringId Channel) const override;
		NEXUS_ENGINE_API String& GetMessageBuffer() override;
		NEXUS_ENGINE_API void LogMessageInternal(LoggerVerbosity Verbosity, StringId Channel) override;

	private:
		inline uint8 GetLogLevel(LoggerVerbosity Verbosity) const;
		inline void GatherInfo(int8 VerbosityLevel, StringView& VerbosityString, int8& Hours, int8& Minutes, int8& Seconds) const;
		inline void CopyIntoBuffer(String& Text);
		inline void Print(String& Text);

		inline static const String Format = "[%02d:%02d:%02d][%7s][%s] %s\n";

		Dictionary<StringId, bool, Hashing::Default> Channels;
		String StringBuilderMessage;
		String StringBuilderFormat;
		String StringBuffer;

		LoggerVerbosity VerbosityMask;
		LoggerOutput Outputs;
		bool FlushOnLog;

		Platform* Target;
		File Handle;
	};
}
