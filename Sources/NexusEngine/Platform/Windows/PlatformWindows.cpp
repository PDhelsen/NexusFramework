#include "Core/NexusEnginePch.h"
#include "PlatformWindows.h"

#include <windows.h>

#include "External/StandardLibrary.h"

namespace NxEn
{
	// https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	// Keep synced with the enum in the Platform.h
	static const String ConsoleColors[8] = { "\033[37m", "\033[30m", "\033[31m", "\033[32m", "\033[34m", "\033[33m", "\033[36m", "\033[35m" };
	static const String ConsoleFormatReset = "\033[m";

	void PlatformWindows::WaitForUserToCloseConsole() const
	{
		std::cin.get();
	}

	void PlatformWindows::WriteToConsole(StringView Message, ConsoleColor Color /*ConsoleColor::White*/) const
	{
		std::cout << ConsoleColors[(uint8)Color].C() << Message.C() << ConsoleFormatReset.C();
	}

	void PlatformWindows::WriteToOutput(StringView Message) const
	{
		OutputDebugStringA(Message.C());
	}

	void PlatformWindows::ExecuteFromDll(StringView DllName, uint8 Ordinal) const
	{
		auto Dll = LoadLibraryA(DllName.C());
		if (Dll == nullptr)
		{
			NEXUS_LOG(Engine, Error, "Default", "Failed to load library")
			return;
		}

		typedef void(CALLBACK* ProjectFunction)();
		ProjectFunction Function = (ProjectFunction)GetProcAddress(Dll, MAKEINTRESOURCEA(Ordinal));
		if (Function == nullptr)
		{
			NEXUS_LOG(Engine, Error, "Default", "Failed to load function")
			FreeLibrary(Dll);
			return;
		}

		Function();

		FreeLibrary(Dll);
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

		SetConsoleOutputCP(CP_UTF8);
	}
}
