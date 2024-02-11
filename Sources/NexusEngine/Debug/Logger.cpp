#include "Core/NexusPch.h"
#include "Logger.h"

#include "Platform/Platform.h"

namespace NxEn
{
	// Keep the const char array sync with the Verbosity & Source enum in the h file
	static const char* VerbosityStrings[4] = { "Fatal  ", "Error  ", "Warning", "Info   " };
	static const char* SourceStrings[4] = { "Engine ", "Editor ", "App    ", "Project" };
	static const char* FormatString = "[%s][%s][%s][%i] %s\n";
	static uint8 Colors[4] = { 5, 1, 3, 2 };

	// TEMP: Remove once we have string
	static const uint16 MaxChars = 1024;
	static char Resolved[MaxChars];
	static char Formatted[MaxChars];

	Logger* Logger::Instance = new Logger(Logger::Verbosity::Info);

	Logger::Logger(Verbosity Verbosity)
		: VerbosityLevel(Verbosity)
	{
		Channels = Dictionary<uint16, bool>();
		AddChannel(0, true);
	}

	Logger::~Logger()
	{
	}

	void Logger::Log(Source Source, Verbosity Verbosity, uint16 Channel, const char* Message, ...)
	{
		if (Verbosity > VerbosityLevel)
		{
			return;
		}

		if (!CheckChannel(Channel))
		{
			return;
		}

		Platform* Platform = Platform::GetInstance();

		const char* DateString = Platform->GetTimestamp("%H:%M:%S");
		const char* VerbosityString = VerbosityStrings[(uint8)Verbosity];
		const char* SourceString = SourceStrings[(uint8)Source];
		uint8 Color = Colors[(uint8)Verbosity];
		
		NEXUS_VA(Message, vsnprintf(Resolved, MaxChars, Message, ArgList))
		snprintf(Formatted, MaxChars, FormatString, DateString, SourceString, VerbosityString, Channel, Message);

		Platform->WriteToConsole(Formatted, Color);
		Platform->WriteToOutput(Formatted);
	}

	void Logger::AddChannel(uint16 Channel, bool State)
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
}