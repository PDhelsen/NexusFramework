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
	static Buffer<char> TempBuffer = Buffer<char>(512, nullptr);

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
		DWORD Length = GetCurrentDirectoryA((DWORD)TempBuffer.GetByteSize(), TempBuffer.GetPtr());
		NEXUS_ASSERT(Length != 0 && Length < TempBuffer.GetByteSize(), "Buffer overflowed when getting the current working directory");
		return Path::Normalize(StringView(TempBuffer.GetPtr(), Length));
	}

	bool PlatformWindows::DirectoryCreate(StringView Path) const
	{
		return CreateDirectoryA(Path.C(), nullptr);
	}

	bool PlatformWindows::DirectoryMove(StringView Path, StringView Target, bool Override) const
	{
		return MoveFileExA(Path.C(), Target.C(), Override ? MOVEFILE_REPLACE_EXISTING : 0);
	}

	bool PlatformWindows::DirectoryCopy(StringView Path, StringView Target, bool Override) const
	{
		NEXUS_ASSERT(false, "Not supported at platform level");
		return false;
	}

	bool PlatformWindows::DirectoryDelete(StringView Path) const
	{
		return RemoveDirectoryA(Path.C());
	}

	bool PlatformWindows::DirectoryContent(StringView Path, List<String>& Result) const
	{
		Result.Clear();
		String Temp = Path + "*";

		WIN32_FIND_DATAA Data;
		HANDLE File = FindFirstFileA(Temp.C(), &Data);

		do
		{
			if (File == INVALID_HANDLE_VALUE)
			{
				NEXUS_LOG(Engine, Error, NxEn::LoggerChannel::Default, "Failed to open file %s", Temp.C());
				Result.Clear();
				return false;
			}

			StringView Name = Data.cFileName;
			if (Name == "." || Name == "..")
			{
				continue;
			}

			Temp.Clear();
			Temp += Path;
			Temp += Name;
			Path::Normalize(Temp);

			Result.Append(Temp);

		} while (FindNextFileA(File, &Data) != 0);

		FindClose(File);

		return true;
	}

	bool PlatformWindows::FileCreate(StringView Path, void** Handle) const
	{
		HANDLE File = CreateFileA(
			Path.C(),
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		bool Result = File != INVALID_HANDLE_VALUE;

		if (Result)
		{
			if (Handle == nullptr)
			{
				FileClose(File);
			}
			else
			{
				*Handle = File;
			}
		}

		return Result;
	}

	bool PlatformWindows::FileMove(StringView Path, StringView Target, bool Override) const
	{
		return MoveFileExA(Path.C(), Target.C(), Override ? MOVEFILE_REPLACE_EXISTING : 0);
	}

	bool PlatformWindows::FileCopy(StringView Path, StringView Target, bool Override) const
	{
		return CopyFileA(Path.C(), Target.C(), !Override);
	}

	bool PlatformWindows::FileDelete(StringView Path) const
	{
		return DeleteFileA(Path.C());
	}

	bool PlatformWindows::FileOpen(StringView Path, void** Handle, FileMode Mode) const
	{
		DWORD Attributes = 0;
		switch (Mode)
		{
		case NxEn::Platform::FileMode::Read: Attributes = GENERIC_READ; break;
		case NxEn::Platform::FileMode::Write: Attributes = GENERIC_WRITE; break;
		case NxEn::Platform::FileMode::Append: Attributes = FILE_APPEND_DATA; break;
		}

		HANDLE File = CreateFileA(
			Path.C(),
			Attributes,
			0,
			NULL,
			Mode == FileMode::Write ? TRUNCATE_EXISTING : OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		bool Result = File != INVALID_HANDLE_VALUE;

		if (Result)
		{
			*Handle = File;
			if (Mode == FileMode::Append)
			{
				SetFilePointer(File, 0, nullptr, FILE_END);
			}
		}

		return Result;
	}

	bool PlatformWindows::FileClose(void* File) const
	{
		return CloseHandle(File);
	}

	bool PlatformWindows::FileSize(void* File, uint64* Size) const
	{
		LARGE_INTEGER FileSize;
		bool Result = GetFileSizeEx(File, &FileSize);
		*Size = Result ? FileSize.QuadPart : 0;
		return Result;
	}

	bool PlatformWindows::FileWriteByte(void* File, BufferView<Byte> Data) const
	{
		NEXUS_ASSERT(Data.GetByteSize() <= Integer::MaxUI32(), "Currenlty support only file smaller that uint32 max value");

		DWORD Written = 0;
		bool Result = WriteFile(File, Data.GetPtr(), (DWORD)Data.GetByteSize(), &Written, nullptr);
		return Result && Written == Data.GetByteSize();
	}

	bool PlatformWindows::FileReadByte(void* File, Buffer<Byte>& Data) const
	{
		NEXUS_ASSERT(Data.GetByteSize() <= Integer::MaxUI32(), "Currenlty support only file smaller that uint32 max value");

		DWORD Written = 0;
		bool Result = ReadFile(File, Data.GetPtr(), (DWORD)Data.GetByteSize(), &Written, nullptr);
		return Result && Written == Data.GetByteSize();
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
