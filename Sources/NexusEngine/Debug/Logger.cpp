#include "Core/NexusEnginePch.h"
#include "Logger.h"

#include "External/StandardLibrary.h"

#include "Application/Time.h"
#include "Platform/Platform.h"

namespace NxEn
{
	// Keep the const char array sync with the Verbosity & Source enum in the h file
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION(LoggerSource, Logger::, "Engine", "Editor", "App", "Project");
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(LoggerVerbosity, Logger::, 4, "Fatal", "Error", "Warning", "Info");
	static Platform::ConsoleColor Colors[4] = { Platform::ConsoleColor::Magenta, Platform::ConsoleColor::Red, Platform::ConsoleColor::Yellow, Platform::ConsoleColor::White };

	Logger* Logger::Instance = new Logger(LoggerVerbosity::All);

	Logger::Logger(LoggerVerbosity Verbosity)
		: VerbosityMask(Verbosity)
	{
		StringBuilderMessage = String(1024);
		StringBuilderFormat = String(1024);

		Channels = new std::unordered_map<uint16, bool>();
		AddChannel(0, true);
	}

	Logger::~Logger()
	{
		delete Channels;
	}

	void Logger::AddChannel(uint16 Channel, bool State /*true*/)
	{
		NEXUS_ASSERT(!HasChannel(Channel), "Already has channel : %d", Channel)
		Channels->emplace(Channel, State);
	}

	void Logger::SetChannel(uint16 Channel, bool State)
	{
		NEXUS_ASSERT(HasChannel(Channel), "Doesn't have channel : %d", Channel)
		Channels->at(Channel) = State;
	}

	bool Logger::HasChannel(uint16 Channel) const
	{
		return Channels->find(Channel) != Channels->end();
	}

	bool Logger::CheckChannel(uint16 Channel) const
	{
		NEXUS_ASSERT(HasChannel(Channel), "Doesn't have channel : %d", Channel)
		return Channels->at(Channel);
	}

	bool Logger::CheckVerbosity(LoggerVerbosity Verbosity) const
	{
		return CheckFlag(VerbosityMask, Verbosity);
	}

	void Logger::SetVerbosity(LoggerVerbosity Verbosity, bool State)
	{
		VerbosityMask = SetFlag(VerbosityMask, Verbosity, State);
	}

	bool Logger::ShouldPrint(LoggerVerbosity Verbosity, uint16 Channel) const
	{
		return Platform::GetInstance() && CheckVerbosity(Verbosity) && CheckChannel(Channel);
	}

	uint8 Logger::GetLogLevel(LoggerVerbosity Verbosity) const
	{
		return LogTwoPowerOfTwo((uint8)Verbosity);
	}

	void Logger::GatherInfo(int8 VerbosityLevel, LoggerSource Source, int8& Hours, int8& Minutes, int8 Seconds, StringView& SourceString, StringView& VerbosityString) const
	{
		Timestamp Stamp = Time::GetInstance()->Now();
		Hours = Stamp.Hours;
		Minutes = Stamp.Minutes;
		Seconds = Stamp.Seconds;

		VerbosityString = LoggerVerbosityToString(VerbosityLevel);
		SourceString = LoggerSourceToString(Source);
	}

	void Logger::Print(const String& Message, uint8 Verbosity) const
	{
		Platform* Platform = Platform::GetInstance();
		Platform->WriteToConsole(Message.C(), Colors[Verbosity]);
		Platform->WriteToOutput(Message.C());
	}
}
