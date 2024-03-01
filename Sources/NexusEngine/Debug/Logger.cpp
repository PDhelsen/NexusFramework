#include "Core/NexusEnginePch.h"
#include "Logger.h"

#include "Platform/Platform.h"

namespace NxEn
{
	// Keep the const char array sync with the Verbosity & Source enum in the h file
	static const char* DateString = "%02d:%02d:%02d";
	static const char* VerbosityStrings[4] = { "Fatal  ", "Error  ", "Warning", "Info   " };
	static const char* SourceStrings[4] = { "Engine ", "Editor ", "App    ", "Project" };
	static const char* FormatString = "[%s][%s][%s][%i] %s\n";
	static Platform::ConsoleColor Colors[4] = { Platform::ConsoleColor::Magenta, Platform::ConsoleColor::Red, Platform::ConsoleColor::Yellow, Platform::ConsoleColor::White };

	// TEMP: Remove once we have string
	static const uint16 MaxChars = 1024;
	static char Date[MaxChars];
	static char Resolved[MaxChars];
	static char Formatted[MaxChars];

	Logger* Logger::Instance = new Logger(LoggerVerbosity::Info);

	Logger::Logger(LoggerVerbosity Verbosity)
		: VerbosityMask(0)
	{
		Channels = Dictionary<uint16, bool>();

		AddChannel(0, true);
		SetVerbosity(Verbosity, true, true);
	}

	Logger::~Logger()
	{
	}

	void Logger::Log(LoggerSource Source, LoggerVerbosity Verbosity, uint16 Channel, const char* Message, ...) const
	{
		if (!CheckVerbosity(Verbosity))
		{
			return;
		}

		if (!CheckChannel(Channel))
		{
			return;
		}

		Timestamp Stamp = Time::GetInstance()->Now();
		Platform::ConsoleColor Color = Colors[(uint8)Verbosity];
		const char* VerbosityString = VerbosityStrings[(uint8)Verbosity];
		const char* SourceString = SourceStrings[(uint8)Source];
		
		NEXUS_VA(Message, vsnprintf(Resolved, MaxChars, Message, ArgList))
		snprintf(Date, MaxChars, DateString, Stamp.Hours, Stamp.Minutes, Stamp.Seconds);
		snprintf(Formatted, MaxChars, FormatString, Date, SourceString, VerbosityString, Channel, Resolved);

		Platform* Platform = Platform::GetInstance();
		Platform->WriteToConsole(Formatted, Color);
		Platform->WriteToOutput(Formatted);
	}

	void Logger::AddChannel(uint16 Channel, bool State /*true*/)
	{
		NEXUS_ASSERT(!HasChannel(Channel), "Already has channel : %d", Channel)
		Channels.emplace(Channel, State);
	}

	void Logger::SetChannel(uint16 Channel, bool State)
	{
		NEXUS_ASSERT(HasChannel(Channel), "Doesn't have channel : %d", Channel)
		Channels[Channel] = State;
	}

	bool Logger::HasChannel(uint16 Channel) const
	{
		return Channels.find(Channel) != Channels.end();
	}

	bool Logger::CheckChannel(uint16 Channel) const
	{
		NEXUS_ASSERT(HasChannel(Channel), "Doesn't have channel : %d", Channel)
		return Channels.at(Channel);
	}

	bool Logger::CheckVerbosity(LoggerVerbosity Verbosity) const
	{
		return CheckBit(VerbosityMask, (uint8)Verbosity);
	}

	void Logger::SetVerbosity(LoggerVerbosity Verbosity, bool State, bool All /*false*/)
	{
		if (!All)
		{
			SetBit(VerbosityMask, (uint8)Verbosity, State);
			return;
		}

		for (int8 Offset = (uint8)Verbosity; Offset >= 0; Offset--)
		{
			SetBit(VerbosityMask, (uint8)Offset, State);
		}
	}
}