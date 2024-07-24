#include "Core/NexusEnginePch.h"
#include "Logger.h"

#include "External/StandardLibrary.h"

#include "Application/Time.h"
#include "Platform/Platform.h"

namespace NxEn
{
	// Keep the const char array sync with the Verbosity & Source enum in the h file
	static const char* DateString = "%02d:%02d:%02d";
	static const char* FormatString = "[%s][%7s][%7s][%i] %s\n";
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION(LoggerSource, Logger::, "Engine", "Editor", "App", "Project");
	NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(LoggerVerbosity, Logger::, 4, "Fatal", "Error", "Warning", "Info");
	static Platform::ConsoleColor Colors[4] = { Platform::ConsoleColor::Magenta, Platform::ConsoleColor::Red, Platform::ConsoleColor::Yellow, Platform::ConsoleColor::White };

	// TEMP: Remove - String - Once we have string
	static const uint16 MaxChars = 1024;
	static char Date[MaxChars];
	static char Resolved[MaxChars];
	static char Formatted[MaxChars];

	Logger* Logger::Instance = new Logger(LoggerVerbosity::All);

	Logger::Logger(LoggerVerbosity Verbosity)
		: VerbosityMask(Verbosity)
	{
		Channels = new std::unordered_map<uint16, bool>();
		AddChannel(0, true);
	}

	Logger::~Logger()
	{
		delete Channels;
	}

	void Logger::Log(LoggerSource Source, LoggerVerbosity Verbosity, uint16 Channel, const char* Message, ...) const
	{
		Platform* Platform = Platform::GetInstance();
		if (Platform == nullptr)
		{
			return;
		}

		if (!CheckVerbosity(Verbosity))
		{
			return;
		}

		if (!CheckChannel(Channel))
		{
			return;
		}

		uint8 VerbosityIndex = LogTwoPowerOfTwo((uint8)Verbosity);

		Timestamp Stamp = Time::GetInstance()->Now();
		Platform::ConsoleColor Color = Colors[VerbosityIndex];
		const char* VerbosityString = LoggerVerbosityToString(VerbosityIndex);
		const char* SourceString = LoggerSourceToString(Source);
		
		NEXUS_VA(Message, vsnprintf(Resolved, MaxChars, Message, ArgList))
		snprintf(Date, MaxChars, DateString, Stamp.Hours, Stamp.Minutes, Stamp.Seconds);
		snprintf(Formatted, MaxChars, FormatString, Date, SourceString, VerbosityString, Channel, Resolved);

		Platform->WriteToConsole(Formatted, Color);
		Platform->WriteToOutput(Formatted);
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
}
