#include "Core/NexusEnginePch.h"
#include "PlatformWindows.h"

#include <windows.h>

namespace NxEn
{
	// https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	// Keep synced with the enum in the Platform.h
	static const char* ConsoleColors[8] = { "\033[37m", "\033[30m", "\033[31m", "\033[32m", "\033[34m", "\033[33m", "\033[36m", "\033[35m" };
	static const char* ConsoleFormatReset = "\033[m";

	// TEMP: Remove once we have string
	static const uint16 MaxChars = 128;
	static char TimeBuffer[MaxChars];

	void PlatformWindows::WaitForUserToCloseConsole() const
	{
		std::cin.get();
	}

	void PlatformWindows::WriteToConsole(const char* Message, ConsoleColor Color /*ConsoleColor::White*/) const
	{
		std::cout << ConsoleColors[(uint8)Color] << Message << ConsoleFormatReset;
	}

	void PlatformWindows::WriteToOutput(const char* Message) const
	{
		OutputDebugStringA(Message);
	}

	void PlatformWindows::ExecuteFromDll(const char* DllName, uint8 Ordinal) const
	{
		auto dll = LoadLibraryA(DllName);
		if (dll == nullptr)
		{
			NEXUS_LOG(Engine, Error, 0, "Failed to load library")
			return;
		}

		typedef void(CALLBACK* ProjectFunction)();
		ProjectFunction function = (ProjectFunction)GetProcAddress(dll, MAKEINTRESOURCEA(Ordinal));
		if (function == nullptr)
		{
			NEXUS_LOG(Engine, Error, 0, "Failed to load function")
			FreeLibrary(dll);
			return;
		}

		function();

		FreeLibrary(dll);
	}

	// TODO: Move to Time
	const char* PlatformWindows::GetTimestamp(const char* Format) const
	{
		std::time_t Time;
		std::tm TimeInfo;
		
		time(&Time);
		localtime_s(&TimeInfo, &Time);

		std::strftime(TimeBuffer, MaxChars, Format, &TimeInfo);
		return TimeBuffer;
	}

	PlatformWindows::PlatformWindows()
	{
		InitializeConsole();
	}

	PlatformWindows::~PlatformWindows()
	{
	}

	void PlatformWindows::InitializeConsole()
	{
		HANDLE ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD ConsoleOutDefaultMode = 0;
		GetConsoleMode(ConsoleOut, &ConsoleOutDefaultMode);
		DWORD ConsoleOutRequestMode = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
		DWORD ConsoleOutMode = ConsoleOutDefaultMode | ConsoleOutRequestMode;
		SetConsoleMode(ConsoleOut, ConsoleOutMode);
	}
}