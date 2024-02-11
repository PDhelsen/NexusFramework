#include "Core/NexusPch.h"
#include "Logger.h"

#include "Platform/Platform.h"

namespace NxEn
{
	static const char* VerbosityStrings[4] = { "Fatal  ", "Error  ", "Warning", "Info   " };
	static const char* SourceStrings[4] = { "Engine ", "Editor ", "App    ", "Project" };
	static const char* FormatString = "[%s][%s][%s][%i] %s\n";



	static const uint16 MaxChars = 1024;
	static char Resolved[MaxChars];
	static char Formatted[MaxChars];
	static char Time[MaxChars];

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
		if (!CheckVerbosityLevel(Verbosity))
		{
			return;
		}

		if (!CheckChannel(Channel))
		{
			return;
		}

		NEXUS_VA(Message, vsnprintf(Resolved, MaxChars, Message, ArgList))

		const char* DateString = DateToString();
		const char* VerbosityString = VerbosityToString(Verbosity);
		const char* SourceString = SourceToString(Source);

		const char* MessageFormatted = Format(Resolved, DateString, SourceString, VerbosityString, Channel);

		Platform* Platform = Platform::GetInstance();
		Platform->WriteToConsole(MessageFormatted, Verbosity);
		Platform->WriteToOutput(MessageFormatted, Verbosity);
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

	inline bool Logger::HasChannel(uint16 Channel) const
	{
		return Channels.find(Channel) != Channels.end();
	}

	inline bool Logger::CheckChannel(uint16 Channel) const
	{
		NEXUS_ASSERT(HasChannel(Channel), "Doesn't have channel : %d", Channel)
		return Channels.at(Channel);
	}

	inline bool Logger::CheckVerbosityLevel(Verbosity Verbosity) const
	{
		return Verbosity <= VerbosityLevel;
	}

	inline const char* Logger::DateToString() const
	{
		time_t rawtime;
		time(&rawtime);

		struct tm timeinfo;
		localtime_s(&timeinfo, &rawtime);

		strftime(Time, MaxChars, "%H:%M:%S", &timeinfo);
		return Time;
	}
	 
	inline const char* Logger::VerbosityToString(Verbosity Verbosity) const
	{
		return VerbosityStrings[(uint8)Verbosity];
	}

	inline const char* Logger::SourceToString(Source Source) const
	{
		return SourceStrings[(uint8)Source];
	}

	inline const char* Logger::Format(const char* Message, const char* Date, const char* Source, const char* Verbosity, uint16 Channel) const
	{
		snprintf(Formatted, MaxChars, FormatString, Date, Source, Verbosity, Channel, Message);
		return Formatted;
	}
}