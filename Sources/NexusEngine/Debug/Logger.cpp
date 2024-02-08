#include <windows.h>
#include <time.h>
#include <iostream>

#include "Logger.h"

namespace NxEn
{
	static const char* VerbosityStrings[4] = { "Fatal  ", "Error  ", "Warning", "Info   " };
	static const char* SourceStrings[4] = { "Engine ", "Editor ", "App    ", "Project" };
	static const char* FormatString = "[%s][%s][%s][%i] %s\n";

	// https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	static const char* ColorsConsoleStrings[4] = { "\033[35m", "\033[31m", "\033[33m", "\033[32m" };
	static const char* ColorsConsoleReset = "\033[m";

	static const int MaxChars = 1024;
	static char Resolved[MaxChars];
	static char Formatted[MaxChars];
	static char Time[MaxChars];

	Logger* Logger::Instance = new Logger(Logger::Verbosity::Info);

	Logger::Logger(Verbosity Verbosity)
		: VerbosityLevel(Verbosity)
	{
		Channels = std::unordered_map<int, bool>();
		AddChannel(0, true);

		HANDLE ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD ConsoleOutDefaultMode = 0;
		GetConsoleMode(ConsoleOut, &ConsoleOutDefaultMode);
		DWORD ConsoleOutRequestMode = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
		DWORD ConsoleOutMode = ConsoleOutDefaultMode | ConsoleOutRequestMode;
		SetConsoleMode(ConsoleOut, ConsoleOutMode);
	}

	Logger::~Logger()
	{
	}

	void Logger::Log(Source Source, Verbosity Verbosity, int Channel, const char* Message, ...)
	{
		if (!CheckVerbosityLevel(Verbosity))
		{
			return;
		}

		if (!CheckChannel(Channel))
		{
			return;
		}

		va_list ArgList;
		va_start(ArgList, Message);
		vsnprintf(Resolved, MaxChars, Message, ArgList);
		va_end(ArgList);

		const char* DateString = DateToString();
		const char* VerbosityString = VerbosityToString(Verbosity);
		const char* SourceString = SourceToString(Source);

		const char* MessageFormatted = Format(Resolved, DateString, SourceString, VerbosityString, Channel);

		PrintToConsole(MessageFormatted, Verbosity);
		PrintToOutput(MessageFormatted);
	}

	// TODO: Assert on dictionnary content manipulation
	void Logger::AddChannel(int Channel, bool State)
	{
		Channels.emplace(Channel, State);
	}

	void Logger::SetChannel(int Channel, bool State)
	{
		Channels[Channel] = State;
	}

	inline bool Logger::HasChannel(int Channel) const
	{
		return Channels.find(Channel) != Channels.end();
	}

	inline bool Logger::CheckChannel(int Channel) const
	{
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
		return VerbosityStrings[(int)Verbosity];
	}

	inline const char* Logger::SourceToString(Source Source) const
	{
		return SourceStrings[(int)Source];
	}

	inline const char* Logger::ColorizeConsole(Verbosity Verbosity) const
	{
		return ColorsConsoleStrings[(int)Verbosity];
	}

	inline const char* Logger::Format(const char* Message, const char* Date, const char* Source, const char* Verbosity, int Channel) const
	{
		snprintf(Formatted, MaxChars, FormatString, Date, Source, Verbosity, Channel, Message);
		return Formatted;
	}

	inline void Logger::PrintToConsole(const char* Message, Verbosity Verbosity) const
	{
		const char* Color = ColorizeConsole(Verbosity);
		std::cout << Color << Message << ColorsConsoleReset;
	}

	inline void Logger::PrintToOutput(const char* Message) const
	{
		OutputDebugStringA(Message);
	}
}