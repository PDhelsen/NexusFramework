#pragma once

#include "NexusFramework/Debug/Logger/Log.h"
#include "NexusFramework/Misc/Templates.h"
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
	class NEXUS_FRAMEWORK_API Logger : public Log
	{
	public:
		inline static const String Format = "[%02d:%02d:%02d][%7s][%s] %s%s";

		NEXUS_NOCOPY_NOMOVE(Logger)
		Logger(LoggerVerbosity Verbosity, LoggerOutput Output, StringView Path = "", bool AutoFlush = false);
		~Logger();

		void Flush();

		bool CheckVerbosity(LoggerVerbosity Verbosity) const;
		void SetVerbosity(LoggerVerbosity Verbosity, bool State);

		void AddChannel(StringId Channel, bool State = true);
		void SetChannel(StringId Channel, bool State);
		void SetAllChannels(bool State);
		bool HasChannel(StringId Channel) const;
		bool CheckChannel(StringId Channel) const;
		Array<StringId> GetChannels() const;

		bool CheckOutput(LoggerOutput Output) const;
		void SetOutput(LoggerOutput Output, StringView Path = "");
		void RegisterCallback(const Delegate<void(LoggerVerbosity, StringId, StringView)>& Callback);
		void UnregisterCallback(const Delegate<void(LoggerVerbosity, StringId, StringView)>& Callback);

		bool GetAutoFlush() const;
		void SetAutoFlush(bool Auto);

	protected:
		void PrintLog(LoggerVerbosity Verbosity, StringId Channel, StringView Message) override;
		void FlushLogs();

	private:
		struct Info
		{
			Info(LoggerVerbosity Verbosity, StringId Channel);

			String Message;
			StringId Channel;
			LoggerVerbosity Verbosity;
		};

		List<Info> Infos;
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
