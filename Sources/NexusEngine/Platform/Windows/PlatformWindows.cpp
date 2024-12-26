#include "Core/NexusEnginePch.h"
#include "PlatformWindows.h"

#include <windows.h>
#include <sys/stat.h>

namespace NxEn
{
	using DllFunction = Delegate<int64()>;

	static Buffer<char> StaticBuffer = Buffer<char>(512, nullptr);

	void PlatformWindows::ExecuteFromDll(StringView DllName, uint8 Ordinal) const
	{
		auto Dll = LoadLibraryA(DllName.C());
		if (Dll == nullptr)
		{
			NEXUS_LOG(Error, LoggerChannel::Default, "Failed to load library");
			return;
		}

		DllFunction Function = DllFunction(GetProcAddress(Dll, MAKEINTRESOURCEA(Ordinal)));
		if (!Function)
		{
			NEXUS_LOG(Error, LoggerChannel::Default, "Failed to load function");
			FreeLibrary(Dll);
			return;
		}

		Function.Invoke();

		FreeLibrary(Dll);
	}

	void PlatformWindows::Sleep(uint64 Milliseconds) const
	{
		::Sleep((DWORD)Milliseconds);
	}

	double PlatformWindows::GetProcessorTimer(double Unit) const
	{
		LARGE_INTEGER Counter;
		QueryPerformanceCounter(&Counter);
		return (double)Counter.QuadPart * Unit * PerformanceFrequency;
	}

	void PlatformWindows::WaitForUserToCloseTerminal() const
	{
		uint32 c = getchar();
	}

	void PlatformWindows::WriteToTerminal(StringView Message) const
	{
		printf(Message.C());
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
		DWORD Length = GetCurrentDirectoryA((DWORD)StaticBuffer.GetByteSize(), StaticBuffer.GetPtr());
		NEXUS_ASSERT(Length != 0 && Length < StaticBuffer.GetByteSize(), "Buffer overflowed when getting the current working directory");
		return Path::Normalize(StringView(StaticBuffer.GetPtr(), Length));
	}

	void PlatformWindows::DirectoryCreate(StringView Path) const
	{
		bool Result = CreateDirectoryA(Path.C(), nullptr);
		NEXUS_ASSERT(Result, "Failed to create directory: %s", Path.C());
	}

	void PlatformWindows::DirectoryMove(StringView Path, StringView Target, bool Override) const
	{
		bool Result = MoveFileExA(Path.C(), Target.C(), Override ? MOVEFILE_REPLACE_EXISTING : 0);
		NEXUS_ASSERT(Result, "Failed to move directory: %s", Path.C());
	}

	void PlatformWindows::DirectoryCopy(StringView Path, StringView Target, bool Override) const
	{
		NEXUS_ASSERT(false, "Not supported at platform level");
	}

	void PlatformWindows::DirectoryDelete(StringView Path) const
	{
		bool Result = RemoveDirectoryA(Path.C());
		NEXUS_ASSERT(Result, "Failed to delete directory: %s", Path.C());
	}

	List<String> PlatformWindows::DirectoryContent(StringView Path) const
	{
		List<String> Result;
		String Temp = Path + "*";

		WIN32_FIND_DATAA Data;
		HANDLE File = FindFirstFileA(Temp.C(), &Data);

		do
		{
			if (File == INVALID_HANDLE_VALUE)
			{
				NEXUS_LOG(Error, NxEn::LoggerChannel::Default, "Failed to open file %s", Temp.C());
				Result.Clear();
				return Result;
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

		return Result;
	}

	void* PlatformWindows::FileCreate(StringView Path, bool KeepOpen) const
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
		NEXUS_ASSERT(Result, "Failed to create file: %s", Path.C());

		if (!KeepOpen)
		{
			FileClose(File);
		}

		return Result && KeepOpen ? File : nullptr;
	}

	void PlatformWindows::FileMove(StringView Path, StringView Target, bool Override) const
	{
		bool Result = MoveFileExA(Path.C(), Target.C(), Override ? MOVEFILE_REPLACE_EXISTING : 0);
		NEXUS_ASSERT(Result, "Failed to move file: %s", Path.C());
	}

	void PlatformWindows::FileCopy(StringView Path, StringView Target, bool Override) const
	{
		bool Result = CopyFileA(Path.C(), Target.C(), !Override);
		NEXUS_ASSERT(Result, "Failed to copy file: %s", Path.C());
	}

	void PlatformWindows::FileDelete(StringView Path) const
	{
		bool Result = DeleteFileA(Path.C());
		NEXUS_ASSERT(Result, "Failed to delete file: %s", Path.C());
	}

	void* PlatformWindows::FileOpen(StringView Path, FileMode Mode) const
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
		NEXUS_ASSERT(Result, "Failed to open file: %s", Path.C());

		if (Result && Mode == FileMode::Append)
		{
			SetFilePointer(File, 0, nullptr, FILE_END);
		}

		return Result ? File : nullptr;
	}

	void PlatformWindows::FileClose(void* File) const
	{
		bool Result = CloseHandle(File);
		NEXUS_ASSERT(Result, "Failed to close file");
	}

	uint64 PlatformWindows::FileSize(void* File) const
	{
		LARGE_INTEGER FileSize;
		bool Result = GetFileSizeEx(File, &FileSize);
		return FileSize.QuadPart;
	}

	void PlatformWindows::FileWriteByte(void* File, BufferView<Byte> Data) const
	{
		NEXUS_ASSERT(Data.GetByteSize() <= Integer::MaxUI32(), "Currenlty support only file smaller that uint32 max value");

		DWORD Written = 0;
		bool Result = WriteFile(File, Data.GetPtr(), (DWORD)Data.GetByteSize(), &Written, nullptr);

		NEXUS_ASSERT(Result && Written == Data.GetByteSize(), "Failed to write to file");
	}

	Buffer<Byte> PlatformWindows::FileReadByte(void* File, Allocator* Allctr) const
	{
		Buffer<Byte> Data = Buffer<Byte>(FileSize(File), Allctr);

		DWORD Read = 0;
		bool Result = ReadFile(File, Data.GetPtr(), (DWORD)Data.GetByteSize(), &Read, nullptr);

		NEXUS_ASSERT(Result && Read == Data.GetByteSize(), "Failed to read to file");

		return Data;
	}

	void PlatformWindows::FileWriteText(void* File, StringView Text) const
	{
		String Content = Text.ToString();
		Content.Replace("\n", "\r\n");

		NEXUS_ASSERT(Content.GetCount() <= Integer::MaxUI32(), "Currenlty support only file smaller that uint32 max value");

		DWORD Written = 0;
		bool Result = WriteFile(File, Content.C(), (DWORD)Content.GetCount(), &Written, nullptr);

		NEXUS_ASSERT(Result && Written == Content.GetCount(), "Failed to write to file");
	}

	String PlatformWindows::FileReadText(void* File, Allocator* Allctr) const
	{
		uint64 Size = FileSize(File);
		char* Text = (char*)Memory::Allocate(Size + 1, Allctr);

		DWORD Read = 0;
		bool Result = ReadFile(File, Text, (DWORD)Size, &Read, nullptr);

		NEXUS_ASSERT(Result && Read == Size, "Failed to read to file");

		String Content = String::Create(Text, Size + 1, Size, Allctr);
		Content.Replace("\r\n", "\n");
		return Content;
	}

	PlatformWindows::PlatformWindows()
		: Console(nullptr), PerformanceFrequency(1.0)
	{
		InitializeTerminal();
		InitializePerformanceTimer();
	}

	PlatformWindows::~PlatformWindows()
	{
	}

	void PlatformWindows::InitializeTerminal()
	{
		Console = GetStdHandle(STD_OUTPUT_HANDLE);

		DWORD TerminalOutDefaultMode = 0;
		GetConsoleMode(Console, &TerminalOutDefaultMode);
		DWORD TerminalOutRequestMode = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
		DWORD TerminalOutMode = TerminalOutDefaultMode | TerminalOutRequestMode;
		SetConsoleMode(Console, TerminalOutMode);

		SetConsoleOutputCP(CP_UTF8);
	}

	void PlatformWindows::InitializePerformanceTimer()
	{
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		PerformanceFrequency = 1.0 / (double)Frequency.QuadPart;
	}
}
