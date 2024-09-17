#include "Core/NexusEnginePch.h"
#include "Logger.h"

#include "Application/Time.h"
#include "Platform/Platform.h"

namespace NxEn
{
	// Keep the const char array sync with the Verbosity & Source enum in the h file
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION(LoggerSource, "Engine", "Editor", "App", "Project");
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(LoggerVerbosity, 4, "Fatal", "Error", "Warning", "Info");
	static Platform::TerminalColor Colors[4] = { Platform::TerminalColor::Magenta, Platform::TerminalColor::Red, Platform::TerminalColor::Yellow, Platform::TerminalColor::White };

	Logger* Logger::Instance = new Logger(LoggerVerbosity::All);

	Logger::Logger(LoggerVerbosity Verbosity)
		: VerbosityMask(Verbosity), StringBuilderMessage(1024), StringBuilderFormat(1024)
	{
		Channels = new Dictionary<String, bool, Hashing::Default>();

		AddChannel("Default", true);
		AddChannel("Assert", true);
		AddChannel("Performance", false);
		AddChannel("Routine", false);
	}

	Logger::~Logger()
	{
		delete Channels;
	}

	void Logger::AddChannel(const String& Channel, bool State /*true*/)
	{
		NEXUS_ASSERT(!HasChannel(Channel), "Already has channel : %s", Channel.C())
		Channels->Append(Channel, State);
	}

	void Logger::SetChannel(const String& Channel, bool State)
	{
		NEXUS_ASSERT(HasChannel(Channel), "Doesn't have channel : %s", Channel.C())
		Channels->Get(Channel) = State;
	}

	bool Logger::HasChannel(const String& Channel) const
	{
		return Channels->ContainsKey(Channel);
	}

	bool Logger::CheckChannel(const String& Channel) const
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

	bool Logger::ShouldPrint(LoggerVerbosity Verbosity, const String& Channel) const
	{
		return Platform::GetInstance() && CheckVerbosity(Verbosity) && CheckChannel(Channel);
	}

	uint8 Logger::GetLogLevel(LoggerVerbosity Verbosity) const
	{
		return Math::LogTwoPowerOfTwo((uint8)Verbosity);
	}

	void Logger::GatherInfo(int8 VerbosityLevel, LoggerSource Source, int8& Hours, int8& Minutes, int8 Seconds, StringView& SourceString, StringView& VerbosityString) const
	{
		Timestamp Stamp = Time::GetInstance()->Now();
		Hours = Stamp.Hours;
		Minutes = Stamp.Minutes;
		Seconds = Stamp.Seconds;

		VerbosityString = Enum::LoggerVerbosityToString(VerbosityLevel);
		SourceString = Enum::LoggerSourceToString(Source);
	}

	void Logger::Print(StringView Message, uint8 Verbosity) const
	{
		Platform* Platform = Platform::GetInstance();
		Platform->WriteToTerminal(Message, Colors[Verbosity]);
		Platform->WriteToDebugger(Message);
	}
}
