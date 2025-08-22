#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Debug/Logger/Logger.h"

#include "NexusFramework/Platform/Platform.h"
#include "NexusFramework/Time/Time.h"
#include "NexusFramework/Time/Timestamp.h"
#include "NexusFramework/IO/Path.h"
#include "NexusFramework/IO/File.h"

#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxFr
{
	// Keep the const char array sync with the Verbosity enum in the Log.h file
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(LoggerVerbosity, 4, "Fatal", "Error", "Warning", "Info");

	namespace LoggerChannel
	{
		const StringId Default = "Default"_Sid;
		const StringId Verbose = "Verbose"_Sid;
	}

	Log* Log::GetInstance()
	{
		return Globals::Logs;
	}

	Logger* Logger::GetInstance()
	{
		return Globals::Logs;
	}

	Logger::Logger(bool FlushOnLog, LoggerVerbosity Verbosity, LoggerOutput Output, StringView Path)
		: Channels(), VerbosityMask(Verbosity), Outputs(Output), FlushOnLog(FlushOnLog), BufferMessage(256), BufferFormat(256), BufferLogs(4096), Target(Platform::GetInstance()), Handle("")
	{
		NEXUS_ASSERT(!Enum::CheckFlag(Output, LoggerOutput::File) || Path != StringUtility::Empty, Default, "Path has to be specified in order to write log. LoggerOutput::File is enabled");

		if (CheckOutput(LoggerOutput::File))
		{
			Handle = File(Path);
			Handle.Delete();
			Handle.Create();
			Handle.Open(File::Mode::Append);
		}
	}

	Logger::~Logger()
	{
		Flush();

		if (CheckOutput(LoggerOutput::File))
		{
			Handle.Close();
		}
	}

	void Logger::Flush()
	{
		Print(LoggerVerbosity::None, 0, BufferLogs, true);
		BufferLogs.Clear();
	}

	void Logger::AddChannel(StringId Channel, bool State /*true*/)
	{
		if (HasChannel(Channel))
		{
			NEXUS_LOG(Warning, Default, "Already has channel : %s", Channel.C());
			return;
		}

		Channels.Append(Channel, State);
	}

	void Logger::SetChannel(StringId Channel, bool State)
	{
		if (!HasChannel(Channel))
		{
			NEXUS_LOG(Error, Default, "Doesn't have channel : %s", Channel.C());
			return;
		}

		Channels.Get(Channel) = State;
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
		return Channels.ContainsKey(Channel);
	}

	bool Logger::CheckChannel(StringId Channel) const
	{
		if (!HasChannel(Channel))
		{
			NEXUS_LOG(Error, Default, "Doesn't have channel : %s", Channel.C());
			return false;
		}

		return Channels.Get(Channel);
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

	bool Logger::CheckVerbosity(LoggerVerbosity Verbosity) const
	{
		return Enum::CheckFlag(VerbosityMask, Verbosity);
	}

	void Logger::SetVerbosity(LoggerVerbosity Verbosity, bool State)
	{
		VerbosityMask = Enum::SetFlag(VerbosityMask, Verbosity, State);
	}

	bool Logger::CheckOutput(LoggerOutput Output) const
	{
		return Enum::CheckFlag(Outputs, Output);
	}

	void Logger::RegisterCallback(const Delegate<void(LoggerVerbosity, StringId, StringView)>& Callback)
	{
		this->Callback += Callback;
	}

	void Logger::UnregisterCallback(const Delegate<void(LoggerVerbosity, StringId, StringView)>& Callback)
	{
		this->Callback -= Callback;
	}

	String* Logger::ShouldPrintMessage(LoggerVerbosity Verbosity, StringId Channel, StringView Message)
	{
		BufferMessage.Clear();
		bool ShouldPrint = !Message.IsEmpty() && CheckVerbosity(Verbosity) && CheckChannel(Channel);
		return ShouldPrint ? &BufferMessage : nullptr;
	}

	String* Logger::FormatMessage(LoggerVerbosity Verbosity, StringId Channel, StringView Message)
	{
		uint8 VerbosityLevel = Enum::ToIndex(Verbosity);
		StringView VerbosityLabel = Enum::LoggerVerbosityToString(VerbosityLevel);

		StringView ChannelLabel = Channel.C();

		Timestamp Stamp = Time::Now();
		int8 Hours = Stamp.Hours;
		int8 Minutes = Stamp.Minutes;
		int8 Seconds = Stamp.Seconds;

		BufferFormat.Clear();
		BufferFormat.Format(Format, Hours, Minutes, Seconds, VerbosityLabel.C(), ChannelLabel.C(), Message.C());
		return &BufferFormat;
	}

	void Logger::PrintMessage(LoggerVerbosity Verbosity, StringId Channel, StringView Message)
	{
		Print(Verbosity, Channel, Message, false);

		if (Enum::CheckFlag(Verbosity, LoggerVerbosity::Fatal))
		{
			Flush();
		}
	}

	void Logger::Print(LoggerVerbosity Verbosity, StringId Channel, StringView Message, bool Flushing)
	{
		if (Message.IsEmpty())
		{
			return;
		}

		bool Write = FlushOnLog || Flushing;

		if (!Write)
		{
			if (BufferLogs.GetCapacity() - BufferLogs.GetCount() < Message.GetCount())
			{
				Flush();
			}
			
			BufferLogs += Message;
		}

		if (CheckOutput(LoggerOutput::Console) && Write)
		{
			Target->WriteToTerminal(Message);
		}
		if (CheckOutput(LoggerOutput::IDE) && Write)
		{
			Target->WriteToDebugger(Message);
		}
		if (CheckOutput(LoggerOutput::File) && Write)
		{
			Handle.WriteText(Message);
		}
		if (CheckOutput(LoggerOutput::File) && !Flushing)
		{
			Callback.Invoke(Verbosity, Channel, Message);
		}
	}
}
