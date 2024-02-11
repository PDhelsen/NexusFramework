#include "Core/NexusPch.h"
#include "PlatformWindows.h"

#include <windows.h>

namespace NxEn
{
	// https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	static const char* ColorsConsoleStrings[4] = { "\033[35m", "\033[31m", "\033[33m", "\033[32m" };
	static const char* ColorsConsoleReset = "\033[m";

	void PlatformWindows::WaitForUserCloseConsole() const
	{
		std::cin.get();
	}

	void PlatformWindows::WriteToConsole(const char* Message, Logger::Verbosity Verbosity) const
	{
		std::cout << ColorsConsoleStrings[(uint8)Verbosity] << Message << ColorsConsoleReset;
	}

	void PlatformWindows::WriteToOutput(const char* Message, Logger::Verbosity Verbosity) const
	{
		OutputDebugStringA(Message);
	}

	void PlatformWindows::ExecuteFromDll(const char* DllName, int8 Ordinal) const
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