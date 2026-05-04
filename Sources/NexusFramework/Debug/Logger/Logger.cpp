#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Debug/Logger/Logger.h"

#include "NexusFramework/Time/Time.h"
#include "NexusFramework/Time/Timestamp.h"

#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxFr
{
	namespace LoggerChannel
	{
		const StringId Default = "Default"_Sid;
		const StringId Verbose = "Verbose"_Sid;
	}

	static Platform::TerminalColor VerbosityToTerminalColor(LoggerVerbosity Verbosity)
	{
		switch (Verbosity)
		{
		case NxFr::LoggerVerbosity::Fatal:
			return Platform::TerminalColor::Magenta;
		case NxFr::LoggerVerbosity::Error:
			return Platform::TerminalColor::Red;
		case NxFr::LoggerVerbosity::Warning:
			return Platform::TerminalColor::Yellow;
		case NxFr::LoggerVerbosity::Info:
			return Platform::TerminalColor::White;

		case NxFr::LoggerVerbosity::None:
		case NxFr::LoggerVerbosity::All:
		case NxFr::LoggerVerbosity::COUNT:
		default:
			return Platform::TerminalColor::None;
		}
	}

	Logger::LogData::LogData(LoggerVerbosity Verbosity, StringId Channel)
		: Message(64), Channel(Channel), Verbosity(Verbosity)
	{
	}

	Log* Log::GetInstance()
	{
		return Globals::Logs;
	}

	Logger* Logger::GetInstance()
	{
		return Globals::Logs;
	}

	Logger::Logger(LoggerVerbosity Verbosity, LoggerOutput Output, StringView Path, bool AutoFlush)
		: Logs(), Channels(), VerbosityMask(Verbosity), Outputs(Output),
		Target(Platform::GetInstance()), Stream(""), Callback(),
		AutoFlush(AutoFlush), Guard()
	{
		SetOutput(LoggerOutput::File, !Path.IsEmpty(), Path);
	}

	Logger::~Logger()
	{
		Flush();
		SetOutput(LoggerOutput::File, false);
	}

	void Logger::Flush()
	{
		Lock LockGuard(Guard);
		FlushLogs();
	}

	bool Logger::CheckVerbosity(LoggerVerbosity Verbosity) const
	{
		return Enum::CheckFlag(VerbosityMask, Verbosity);
	}

	void Logger::SetVerbosity(LoggerVerbosity Verbosity, bool State)
	{
		VerbosityMask = Enum::SetFlag(VerbosityMask, Verbosity, State);
	}

	void Logger::AddChannel(StringId Channel, bool State)
	{
		if (HasChannel(Channel))
		{
			NEXUS_LOG(Error, Default, "Channel (%s) is already registered", Channel.C());
			return;
		}

		Channels.Append(Channel, State);
	}

	void Logger::SetChannel(StringId Channel, bool State)
	{
		if (!HasChannel(Channel))
		{
			NEXUS_LOG(Error, Default, "Channel (%s) is not registered", Channel.C());
			return;
		}

		Channels[Channel] = State;
	}

	void Logger::SetAllChannels(bool State)
	{
		for (auto& [Id, Value] : Channels)
		{
			Channels[Id] = State;
		}
	}

	bool Logger::HasChannel(StringId Channel) const
	{
		return Channels.TryGet(Channel) != nullptr;
	}

	bool Logger::CheckChannel(StringId Channel) const
	{
		if (!HasChannel(Channel))
		{
			NEXUS_LOG(Error, Default, "Channel (%s) is not registered", Channel.C());
			return false;
		}

		return Channels[Channel];
	}

	Array<StringId> Logger::GetChannels() const
	{
		Array<StringId> Labels(Channels.GetCount());

		uint64 Index = 0;
		for (auto& [Id, State] : Channels)
		{
			Labels[Index++] = Id;
		}

		return Labels;
	}

	bool Logger::CheckOutput(LoggerOutput Output) const
	{
		return Enum::CheckFlag(Outputs, Output);
	}

	void Logger::SetOutput(LoggerOutput Output, bool State, StringView Path)
	{
		if (CheckOutput(LoggerOutput::File))
		{
			Stream.Close();
		}

		Outputs = Enum::SetFlag(Outputs, Output, State);

		if (CheckOutput(LoggerOutput::File))
		{
			Stream = TextStream(Path);
			Stream.Open(File::Mode::Write);
		}
	}

	void Logger::RegisterCallback(const Delegate<void(LoggerVerbosity, StringId, StringView)>& Callback)
	{
		this->Callback += Callback;
	}

	void Logger::UnregisterCallback(const Delegate<void(LoggerVerbosity, StringId, StringView)>& Callback)
	{
		this->Callback -= Callback;
	}

	bool Logger::GetAutoFlush() const
	{
		return AutoFlush;
	}

	void Logger::SetAutoFlush(bool State)
	{
		AutoFlush = State;
	}

	String& Logger::GetBuffer()
	{
		static thread_local String Buffer(64, nullptr);
		return Buffer;
	}

	void Logger::PrintLog(LoggerVerbosity Verbosity, StringId Channel, StringView Message)
	{
		if (!CheckVerbosity(Verbosity) || !CheckChannel(Channel) || Message.IsEmpty())
		{
			return;
		}

		String VerbosityLabel = StringUtility::ToString(Verbosity);
		bool IsFatal = Enum::CheckFlag(Verbosity, LoggerVerbosity::Fatal);
		Timestamp Stamp = Time::Now();
		int8 Hours = Stamp.Hours;
		int8 Minutes = Stamp.Minutes;
		int8 Seconds = Stamp.Seconds;

		LogData Data(Verbosity, Channel);
		Data.Message.Format(Format, Hours, Minutes, Seconds, VerbosityLabel.C(), Channel.C(), GetBuffer().C(), StringUtility::NewLine.C());

		Lock LockGuard(Guard);
		Logs.Append(Move(Data));
		if (AutoFlush || IsFatal)
		{
			FlushLogs();
		}
	}

	void Logger::FlushLogs()
	{
		for (auto& Data : Logs)
		{
			if (CheckOutput(LoggerOutput::Console))
			{
				Target->WriteToTerminal(Data.Message, VerbosityToTerminalColor(Data.Verbosity));
			}
			if (CheckOutput(LoggerOutput::IDE))
			{
				Target->WriteToDebugger(Data.Message);
			}
			if (CheckOutput(LoggerOutput::File))
			{
				Stream.WriteBlock(Data.Message);
			}
			if (CheckOutput(LoggerOutput::Callback))
			{
				Callback.Invoke(Data.Verbosity, Data.Channel, Data.Message);
			}
		}

		if (CheckOutput(LoggerOutput::File))
		{
			Stream.Flush();
		}

		Logs.Clear();
	}
}
