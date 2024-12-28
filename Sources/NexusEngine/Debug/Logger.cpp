#include "Core/NexusEnginePch.h"
#include "Logger.h"

#include "Application/Time.h"
#include "Platform/Platform.h"
#include "Misc/IO/Path.h"
#include "Misc/IO/File.h"

#define NEXUS_LOG_LINE 256
#define NEXUS_LOG_BUFFER 4096

namespace NxEn
{
	namespace LoggerChannel
	{
		const StringId Default = "Default"_Sid;
		const StringId Assert = "Assert"_Sid;
		const StringId Performance = "Performance"_Sid;
		const StringId Routine = "Routine"_Sid;
		const StringId UnitTest = "UnitTest"_Sid;
	}

	// Keep the const char array sync with the Verbosity & Source enum in the h file
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(LoggerVerbosity, 4, "Fatal", "Error", "Warning", "Info");

	Logger* Logger::GetInstance()
	{
		static Logger* Instance = new Logger(true, LoggerVerbosity::All, LoggerOutput::Console, Path::GetWorkingDirectory() + "Logs.txt");
		return Instance;
	}

	Logger::Logger(bool FlushOnLog, LoggerVerbosity Verbosity, LoggerOutput Output, StringView Path)
		: StringBuilderMessage(NEXUS_LOG_LINE), StringBuilderFormat(NEXUS_LOG_LINE), StringBuffer(NEXUS_LOG_BUFFER), VerbosityMask(Verbosity), Outputs(Output), FlushOnLog(FlushOnLog), Target(nullptr), Handle(nullptr)
	{
		NEXUS_ASSERT(!Enum::CheckFlag(Output, LoggerOutput::File) || Path != StringUtility::Empty, "Path has to be specified in order to write log. LoggerOutput::File is enabled");

		Channels = new Dictionary<StringId, bool, Hashing::Default>();
		AddChannel(LoggerChannel::Default, true);
		AddChannel(LoggerChannel::Assert, true);
		AddChannel(LoggerChannel::Performance, false);
		AddChannel(LoggerChannel::Routine, false);
		AddChannel(LoggerChannel::UnitTest, false);

		Target = Platform::GetInstance();

		if (CheckOutput(LoggerOutput::File))
		{
			Handle = new File(Path);
			Handle->Delete();
			Handle->Create();
			Handle->Open(File::Mode::Append);
		}
	}

	Logger::~Logger()
	{
		Flush();

		if (CheckOutput(LoggerOutput::File))
		{
			Handle->Close();
			delete Handle;
		}

		delete Channels;
	}

	void Logger::AddChannel(StringId Channel, bool State /*true*/)
	{
		NEXUS_ASSERT(!HasChannel(Channel), "Already has channel : %s", Channel.C())
		Channels->Append(Channel, State);
	}

	void Logger::SetChannel(StringId Channel, bool State)
	{
		NEXUS_ASSERT(HasChannel(Channel), "Doesn't have channel : %s", Channel.C())
		Channels->Get(Channel) = State;
	}

	bool Logger::HasChannel(StringId Channel) const
	{
		return Channels->ContainsKey(Channel);
	}

	bool Logger::CheckChannel(StringId Channel) const
	{
		NEXUS_ASSERT(HasChannel(Channel), "Doesn't have channel : %s", Channel.C())
		return Channels->Get(Channel);
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
		return Platform::GetInstance() && CheckVerbosity(Verbosity) && CheckChannel(Channel);
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
			Handle->WriteText(Text);
		}

		Text.Clear();
	}
}
