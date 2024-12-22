#include "Core/NexusEnginePch.h"
#include "Logger.h"

#include "Application/Time.h"
#include "Platform/Platform.h"
#include "Misc/IO/Path.h"
#include "Misc/IO/File.h"

#define NEXUS_LOG_LINE 256

namespace NxEn
{
	StringId LoggerChannel::Default;
	StringId LoggerChannel::Assert;
	StringId LoggerChannel::Performance;
	StringId LoggerChannel::Routine;
	StringId LoggerChannel::UnitTest;

	// Keep the const char array sync with the Verbosity & Source enum in the h file
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION(LoggerSource, "Engine", "Editor", "App", "Project");
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(LoggerVerbosity, 4, "Fatal", "Error", "Warning", "Info");
	static Platform::TerminalColor Colors[4] = { Platform::TerminalColor::Magenta, Platform::TerminalColor::Red, Platform::TerminalColor::Yellow, Platform::TerminalColor::White };

	Logger* Logger::GetInstance()
	{
		static Logger* Instance = new Logger(LoggerVerbosity::All, LoggerOutput::Console, Path::GetWorkingDirectory() + "Logs.txt");
		return Instance;
	}

	Logger::Logger(LoggerVerbosity Verbosity, LoggerOutput Output, StringView Path)
		: StringBuilderMessage(NEXUS_LOG_LINE), StringBuilderFormat(NEXUS_LOG_LINE), VerbosityMask(Verbosity), Outputs(Output), Target(nullptr), Handle(nullptr)
	{
		LoggerChannel::Default = "Default"_Sid;
		LoggerChannel::Assert = "Assert"_Sid;
		LoggerChannel::Performance = "Performance"_Sid;
		LoggerChannel::Routine = "Routine"_Sid;
		LoggerChannel::UnitTest = "UnitTest"_Sid;

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

	bool Logger::ShouldPrint(LoggerVerbosity Verbosity, StringId Channel) const
	{
		return Platform::GetInstance() && CheckVerbosity(Verbosity) && CheckChannel(Channel);
	}

	uint8 Logger::GetLogLevel(LoggerVerbosity Verbosity) const
	{
		return Math::LogTwoPowerOfTwo((uint8)Verbosity);
	}

	void Logger::GatherInfo(int8 VerbosityLevel, LoggerSource Source, int8& Hours, int8& Minutes, int8& Seconds, StringView& SourceString, StringView& VerbosityString) const
	{
		Timestamp Stamp = Time::Now();
		Hours = Stamp.Hours;
		Minutes = Stamp.Minutes;
		Seconds = Stamp.Seconds;

		VerbosityString = Enum::LoggerVerbosityToString(VerbosityLevel);
		SourceString = Enum::LoggerSourceToString(Source);
	}

	void Logger::Print(StringView Message, uint8 Verbosity) const
	{
		if (CheckOutput(LoggerOutput::Console))
		{
			Target->WriteToTerminal(Message, Colors[Verbosity]);
		}
		if (CheckOutput(LoggerOutput::IDE))
		{
			Target->WriteToDebugger(Message);
		}
		if (CheckOutput(LoggerOutput::File))
		{
			Handle->WriteText(Message);
		}
	}
}
