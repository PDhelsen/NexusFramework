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
		if (FlushOnLog)
		{
			return;
		}

		Write(BufferLogs);
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

	bool Logger::ShouldPrintMessage(LoggerVerbosity Verbosity, StringId Channel) const
	{
		return Target && CheckVerbosity(Verbosity) && CheckChannel(Channel);
	}

	void Logger::PrintMessage(LoggerVerbosity Verbosity, StringId Channel)
	{
		uint8 VerbosityLevel = GetLogLevel(Verbosity);

		StringView VerbosityString = "";
		int8 Hours = 0, Minutes = 0, Seconds = 0;
		GatherInfo(VerbosityLevel, VerbosityString, Hours, Minutes, Seconds);

		BufferFormat.Format(Format, Hours, Minutes, Seconds, VerbosityString.C(), Channel.C(), BufferMessage.C());

		if (FlushOnLog)
		{
			Write(BufferFormat);
		}
		else
		{
			CopyIntoBuffer(BufferFormat);
		}

		if (Enum::CheckFlag(Verbosity, LoggerVerbosity::Fatal))
		{
			Flush();
		}
	}

	uint8 Logger::GetLogLevel(LoggerVerbosity Verbosity) const
	{
		return Math::LogTwoPowerOfTwo((uint8)Verbosity);
	}

	void Logger::GatherInfo(int8 VerbosityLevel, StringView& VerbosityString, int8& Hours, int8& Minutes, int8& Seconds) const
	{
		VerbosityString = Enum::LoggerVerbosityToString(VerbosityLevel);

		Timestamp Stamp = Time::Now();
		Hours = Stamp.Hours;
		Minutes = Stamp.Minutes;
		Seconds = Stamp.Seconds;
	}

	void Logger::CopyIntoBuffer(String& Text)
	{
		if (BufferLogs.GetCapacity() - BufferLogs.GetCount() < Text.GetCount())
		{
			Write(Text);
		}

		BufferLogs += Text;
	}

	void Logger::Write(String& Text)
	{
		if (CheckOutput(LoggerOutput::Console))
		{
			Target->WriteToTerminal(Text);
		}
		if (CheckOutput(LoggerOutput::IDE))
		{
			Target->WriteToDebugger(Text);
		}
		if (CheckOutput(LoggerOutput::File))
		{
			Handle.WriteText(Text);
		}

		Text.Clear();
	}
}
