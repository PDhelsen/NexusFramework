#include "Core/NexusEnginePch.h"
#include "PlatformWindows.h"

#include <windows.h>

namespace NxEn
{
	using DllFunction = Delegate<int64()>;

	// https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
	// Keep synced with the enum in the Platform.h
	static const String TerminalColors[8] = { "\033[37m", "\033[30m", "\033[31m", "\033[32m", "\033[34m", "\033[33m", "\033[36m", "\033[35m" };
	static const String TerminalFormatReset = "\033[m";
	static const uint64 BufferSize = 512;
	static char Buffer[BufferSize];

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

		DllFunction Function = DllFunction(GetProcAddress(Dll, MAKEINTRESOURCEA(Ordinal)));
		if (!Function)
		{
			NEXUS_LOG(Engine, Error, LoggerChannel::Default, "Failed to load function")
			FreeLibrary(Dll);
			return;
		}

		Function.Invoke();

		FreeLibrary(Dll);
	}

	double PlatformWindows::GetProcessorTimer(double Unit) const
	{
		LARGE_INTEGER Counter;
		QueryPerformanceCounter(&Counter);
		return (double)Counter.QuadPart * Unit * PerformanceFrequency;
	}

	Platform::PathType PlatformWindows::GetPathType(StringView Path) const
	{
		struct _stati64 Buffer;

		if (_stati64(Path.C(), &Buffer) == 0)
		{
			if (Buffer.st_mode & _S_IFREG)
			{
				return PathType::File;
			}
			else if (Buffer.st_mode & _S_IFDIR)
			{
				return PathType::Directory;
			}
			else
			{
				return PathType::Other;
			}
		}

		return PathType::None;
	}

	String PlatformWindows::GetWorkingDirectory() const
	{
		DWORD Length = GetCurrentDirectoryA(BufferSize, Buffer);
		NEXUS_ASSERT(Length != 0 && Length < BufferSize, "Buffer overflowed when getting the current working directory");
		return Path::Normalize(StringView(Buffer, Length));
	}

	List<String> PlatformWindows::DirectoryContent(StringView Path) const
	{
		List<String> Content;
		String Temp = Path + "*";

		WIN32_FIND_DATAA Data;
		HANDLE Handle = FindFirstFileA(Temp.C(), &Data);

		if (Handle == INVALID_HANDLE_VALUE) {
			NEXUS_LOG(Engine, Error, NxEn::LoggerChannel::Default, "Failed to open directory %s", Path.C());
			return List<String>();
		}

		do
		{
			StringView Name = Data.cFileName;
			if (Name == "." || Name == "..")
			{
				continue;
			}

			Temp.Clear();
			Temp += Path;
			Temp += Name;
			Path::Normalize(Temp);

			Content.Append(Temp);

		} while (FindNextFileA(Handle, &Data) != 0);

		FindClose(Handle);

		return Content;
	}

	bool PlatformWindows::DirectoryCreate(StringView Path) const
	{
		return CreateDirectoryA(Path.C(), nullptr);
	}

	bool PlatformWindows::DirectoryMove(StringView Path, StringView Target) const
	{
		return MoveFileA(Path.C(), Target.C());
	}

	bool PlatformWindows::DirectoryDelete(StringView Path) const
	{
		return RemoveDirectoryA(Path.C());
	}

	bool PlatformWindows::FileCreate(StringView Path) const
	{
		HANDLE fileHandle = CreateFileA(
			Path.C(),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		if (fileHandle == INVALID_HANDLE_VALUE) {
			NEXUS_LOG(Engine, Error, NxEn::LoggerChannel::Default, "Failed to create file %s", Path.C());
			return false;
		}

		CloseHandle(fileHandle);
		return true;
	}

	bool PlatformWindows::FileMove(StringView Path, StringView Target) const
	{
		return MoveFileA(Path.C(), Target.C());
	}

	bool PlatformWindows::FileDelete(StringView Path) const
	{
		return DeleteFileA(Path.C());
	}

	PlatformWindows::PlatformWindows()
	{
		InitializeTerminal();
		InitializePerformanceTimer();
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

	void PlatformWindows::InitializePerformanceTimer()
	{
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		PerformanceFrequency = 1.0 / (double)Frequency.QuadPart;
	}
}
