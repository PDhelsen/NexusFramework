#include "Core/NexusEnginePch.h"
#include "PlatformWindows.h"

#include <windows.h>

namespace NxEn
{
	// https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	// Keep synced with the enum in the Platform.h
	static const String TerminalColors[8] = { "\033[37m", "\033[30m", "\033[31m", "\033[32m", "\033[34m", "\033[33m", "\033[36m", "\033[35m" };
	static const String TerminalFormatReset = "\033[m";

	void PlatformWindows::WaitForUserToCloseTerminal() const
	{
		std::cin.get();
	}

	void PlatformWindows::WriteToTerminal(StringView Message, TerminalColor Color /*TerminalColor::White*/) const
	{
		std::cout << TerminalColors[(uint8)Color].C() << Message.C() << TerminalFormatReset.C();
	}

	void PlatformWindows::WriteToDebugger(StringView Message) const
	{
		OutputDebugStringA(Message.C());
	}

	void PlatformWindows::ExecuteFromDll(StringView DllName, uint8 Ordinal) const
	{
		auto Dll = LoadLibraryA(DllName.C());
		if (Dll == nullptr)
		{
			NEXUS_LOG(Engine, Error, LoggerChannel::Default, "Failed to load library")
			return;
		}

		typedef void(CALLBACK* ProjectFunction)();
		ProjectFunction Function = (ProjectFunction)GetProcAddress(Dll, MAKEINTRESOURCEA(Ordinal));
		if (Function == nullptr)
		{
			NEXUS_LOG(Engine, Error, LoggerChannel::Default, "Failed to load function")
			FreeLibrary(Dll);
			return;
		}

		Function();

		FreeLibrary(Dll);
	}

	PlatformWindows::PlatformWindows()
	{
		InitializeTerminal();
	}

	PlatformWindows::~PlatformWindows()
	{
	}

	void PlatformWindows::InitializeTerminal()
	{
		HANDLE TerminalOut = GetStdHandle(STD_OUTPUT_HANDLE);

		DWORD TerminalOutDefaultMode = 0;
		GetConsoleMode(TerminalOut, &TerminalOutDefaultMode);
		DWORD TerminalOutRequestMode = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
		DWORD TerminalOutMode = TerminalOutDefaultMode | TerminalOutRequestMode;
		SetConsoleMode(TerminalOut, TerminalOutMode);

		SetConsoleOutputCP(CP_UTF8);
	}
}
