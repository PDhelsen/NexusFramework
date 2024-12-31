#include "Core/NexusEnginePch.h"
#include "Logger.h"

#include "Platform/Platform.h"
#include "Time/Time.h"
#include "Time/Timestamp.h"
#include "IO/Path.h"
#include "IO/File.h"

#include "Core/NexusEngineGlobals.h"

namespace NxEn
{
	namespace LoggerChannel
	{
		const StringId Default = "Default"_Sid;
		const StringId Verbose = "Verbose"_Sid;
	}

	// Keep the const char array sync with the Verbosity & Source enum in the h file
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(LoggerVerbosity, 4, "Fatal", "Error", "Warning", "Info");

	ILogger* ILogger::GetInstance()
	{
		return Globals::Logs;
	}

	Logger::Logger(bool FlushOnLog, LoggerVerbosity Verbosity, LoggerOutput Output, StringView Path)
		: Channels(), StringBuilderMessage(256), StringBuilderFormat(256), StringBuffer(4096), VerbosityMask(Verbosity), Outputs(Output), FlushOnLog(FlushOnLog), Target(Platform::GetInstance()), Handle("")
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

	void Logger::Flush()
	{
		if (FlushOnLog)
		{
			return;
		}

		Print(StringBuffer);
	}

	bool Logger::ShouldLog(LoggerVerbosity Verbosity, StringId Channel) const
	{
		return Target && CheckVerbosity(Verbosity) && CheckChannel(Channel);
	}

	String& Logger::GetMessage()
	{
		return StringBuilderMessage;
	}

	void Logger::LogMessage(LoggerVerbosity Verbosity, StringId Channel)
	{
		uint8 VerbosityLevel = GetLogLevel(Verbosity);

		StringView VerbosityString = "";
		int8 Hours = 0, Minutes = 0, Seconds = 0;
		GatherInfo(VerbosityLevel, VerbosityString, Hours, Minutes, Seconds);

		StringBuilderFormat.Format(Format, Hours, Minutes, Seconds, VerbosityString.C(), Channel.C(), StringBuilderMessage.C());

		if (FlushOnLog)
		{
			Print(StringBuilderFormat);
		}
		else
		{
			CopyIntoBuffer(StringBuilderFormat);
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
		if (StringBuffer.GetCapacity() - StringBuffer.GetCount() < Text.GetCount())
		{
			Print(Text);
		}

		StringBuffer += Text;
	}

	void Logger::Print(String& Text)
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
