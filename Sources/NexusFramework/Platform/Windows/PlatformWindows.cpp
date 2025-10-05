#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Platform/Windows/PlatformWindows.h"

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/IO/Path.h"

#include <windows.h>
#include <sys/stat.h>
#include <psapi.h>

namespace NxFr
{
	static Buffer& GetLocalBuffer() { static Buffer LocalBuffer(512, nullptr); return LocalBuffer; }

	static String ConvertPath(NxFr::StringView Path) { return StringUtility::Replace(Path, "/", "\\"); }

	void* PlatformWindows::LoadDll(StringView DllName)
	{
		if (Dlls.ContainsKey(DllName))
		{
			return Dlls[DllName];
		}

		HMODULE Dll = LoadLibraryA(DllName.C());
		if (Dll == nullptr)
		{
			NEXUS_LOG(Error, Default, "Failed to load library");
			return nullptr;
		}

		Dlls.Append(DllName, Dll);
		return Dll;
	}

	void PlatformWindows::UnloadDll(StringView DllName)
	{
		if (!Dlls.ContainsKey(DllName))
		{
			return;
		}

		HMODULE Dll = (HMODULE)Dlls[DllName];
		Dlls.Remove(DllName);
		FreeLibrary(Dll);
	}

	void* PlatformWindows::GetFromDll(StringView DllName, StringView FunctionName)
	{
		HMODULE Dll = (HMODULE)LoadDll(DllName);
		
		auto Function = GetProcAddress(Dll, FunctionName.C());
		if (!Function)
		{
			NEXUS_LOG(Error, Default, "Failed to load function");
		}

		return Function;
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

	Platform::MemoryInfo PlatformWindows::GetMemoryInfo() const
	{
		PROCESS_MEMORY_COUNTERS Info;
		GetProcessMemoryInfo(GetCurrentProcess(), &Info, sizeof(Info));

		return MemoryInfo {
			.CurrentUsage = Info.WorkingSetSize,
			.PeakUsage = Info.PeakWorkingSetSize,
			.CurrentAllocated = Info.PagefileUsage,
			.PeakAllocated = Info.PeakPagefileUsage
		};
	}

	void PlatformWindows::WaitForUserToCloseTerminal() const
	{
		uint32 c = getchar();
	}

	String PlatformWindows::ReadFromTerminal() const
	{
		static String Buffer;

		DWORD EventCount;
		GetNumberOfConsoleInputEvents(TerminalIn, &EventCount);

		for (uint64 Index = 0; Index < EventCount; ++Index)
		{
			DWORD Events;
			INPUT_RECORD InputRecord;
			ReadConsoleInputA(TerminalIn, &InputRecord, 1, &Events);

			if (InputRecord.EventType == KEY_EVENT && InputRecord.Event.KeyEvent.bKeyDown)
			{
				char Code = InputRecord.Event.KeyEvent.uChar.AsciiChar;
				char Converter[2] = { Code, StringUtility::NullChar };
				NxFr::StringView Character = Converter;

				if (InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
				{
					WriteToTerminal(StringUtility::NewLine);

					String Result = Buffer;
					Buffer.Clear();
					return Result;
				}
				else if (InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_BACK && !Buffer.IsEmpty())
				{
					WriteToTerminal(Backspace);

					Buffer.Terminate(Buffer.GetCount() - 1);
				}
				else if (Code >= 32 && Code < 127)
				{
					WriteToTerminal(Character);

					Buffer += Character;
				}
			}
		}

		return StringUtility::Empty;
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

	String PlatformWindows::OpenFileDialog(NxFr::StringView Title, NxFr::StringView Extension, NxFr::StringView Name, NxFr::StringView Path) const
	{
		String File = Name + "." + Extension;
		String Directory = ConvertPath(Path);

		Buffer& LocalBuffer = GetLocalBuffer();
		LocalBuffer.Clear();
		LocalBuffer.Set(File.C(), File.GetCount());

		OPENFILENAMEA OpenFileName;
		ZeroMemory(&OpenFileName, sizeof(OpenFileName));
		OpenFileName.lStructSize = sizeof(OpenFileName);
		OpenFileName.lpstrFile = LocalBuffer.GetPtr<char>();
		OpenFileName.nMaxFile = (uint32)LocalBuffer.GetCount();
		OpenFileName.lpstrTitle = Title.C();
		OpenFileName.nMaxFileTitle = (uint32)Title.GetCount();
		OpenFileName.lpstrInitialDir = Directory.C();
		OpenFileName.lpstrFilter = nullptr;
		OpenFileName.Flags = OFN_NOVALIDATE;

		bool Success = GetSaveFileNameA(&OpenFileName);
		return Success ? Path::Normalize(LocalBuffer.GetPtr<char>()) : StringUtility::Empty;
	}

	String PlatformWindows::GetWorkingDirectory() const
	{
		Buffer& LocalBuffer = GetLocalBuffer();

		DWORD Length = GetCurrentDirectoryA((DWORD)LocalBuffer.GetCount(), LocalBuffer.GetPtr<char>());
		NEXUS_ASSERT(Length != 0 && Length < LocalBuffer.GetCount(), Default, "BufferLogs overflowed when getting the current working directory");
		return Path::Normalize(StringView(LocalBuffer.GetPtr<char>(), Length));
	}

	void PlatformWindows::SetWorkingDirectory(StringView Path) const
	{
		SetCurrentDirectoryA(Path.C());
	}

	void PlatformWindows::DirectoryCreate(StringView Path) const
	{
		bool Result = CreateDirectoryA(Path.C(), nullptr);
		NEXUS_ASSERT(Result, Default, "Failed to create directory: %s", Path.C());
	}

	void PlatformWindows::DirectoryMove(StringView Path, StringView Target, bool Override) const
	{
		bool Result = MoveFileExA(Path.C(), Target.C(), Override ? MOVEFILE_REPLACE_EXISTING : 0);
		NEXUS_ASSERT(Result, Default, "Failed to move directory: %s", Path.C());
	}

	void PlatformWindows::DirectoryCopy(StringView Path, StringView Target, bool Override) const
	{
		NEXUS_ASSERT(false, Default, "Not supported at platform level");
	}

	void PlatformWindows::DirectoryDelete(StringView Path) const
	{
		bool Result = RemoveDirectoryA(Path.C());
		NEXUS_ASSERT(Result, Default, "Failed to delete directory: %s", Path.C());
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
				NEXUS_LOG(Error, Default, "Failed to open file %s", Temp.C());
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
		NEXUS_ASSERT(Result, Default, "Failed to create file: %s", Path.C());

		if (!KeepOpen)
		{
			FileClose(File);
		}

		return Result && KeepOpen ? File : nullptr;
	}

	void PlatformWindows::FileMove(StringView Path, StringView Target, bool Override) const
	{
		bool Result = MoveFileExA(Path.C(), Target.C(), Override ? MOVEFILE_REPLACE_EXISTING : 0);
		NEXUS_ASSERT(Result, Default, "Failed to move file: %s", Path.C());
	}

	void PlatformWindows::FileCopy(StringView Path, StringView Target, bool Override) const
	{
		bool Result = CopyFileA(Path.C(), Target.C(), !Override);
		NEXUS_ASSERT(Result, Default, "Failed to copy file: %s", Path.C());
	}

	void PlatformWindows::FileDelete(StringView Path) const
	{
		bool Result = DeleteFileA(Path.C());
		NEXUS_ASSERT(Result, Default, "Failed to delete file: %s", Path.C());
	}

	void* PlatformWindows::FileOpen(StringView Path, FileMode Mode) const
	{
		DWORD Attributes = 0;
		switch (Mode)
		{
		case NxFr::Platform::FileMode::Read: Attributes = GENERIC_READ; break;
		case NxFr::Platform::FileMode::Write: Attributes = GENERIC_WRITE; break;
		case NxFr::Platform::FileMode::Append: Attributes = FILE_APPEND_DATA; break;
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
		NEXUS_ASSERT(Result, Default, "Failed to open file: %s", Path.C());

		if (Result && Mode == FileMode::Append)
		{
			SetFilePointer(File, 0, nullptr, FILE_END);
		}

		return Result ? File : nullptr;
	}

	void PlatformWindows::FileClose(void* File) const
	{
		NEXUS_ASSERT(File, Default, "Invalid File");
		bool Result = CloseHandle(File);
		NEXUS_ASSERT(Result, Default, "Failed to close file");
	}

	uint64 PlatformWindows::FileSize(void* File) const
	{
		NEXUS_ASSERT(File, Default, "Invalid File");

		LARGE_INTEGER FileSize;
		bool Result = GetFileSizeEx(File, &FileSize);
		return FileSize.QuadPart;
	}

	void PlatformWindows::FileWriteByte(void* File, BufferView Data) const
	{
		NEXUS_ASSERT(File, Default, "Invalid File");
		NEXUS_ASSERT(Data.GetCount() <= Integer::MaxUI32, Default, "Currenlty support only file smaller that uint32 max value");

		DWORD Written = 0;
		bool Result = WriteFile(File, Data.GetPtr(), (DWORD)Data.GetCount(), &Written, nullptr);

		NEXUS_ASSERT(Result && Written == Data.GetCount(), Default, "Failed to write to file");
	}

	Buffer PlatformWindows::FileReadByte(void* File) const
	{
		NEXUS_ASSERT(File, Default, "Invalid File");

		Buffer Data = Buffer(FileSize(File));

		DWORD Read = 0;
		bool Result = ReadFile(File, Data.GetPtr(), (DWORD)Data.GetCount(), &Read, nullptr);

		NEXUS_ASSERT(Result && Read == Data.GetCount(), Default, "Failed to read to file");

		return Data;
	}

	void PlatformWindows::FileWriteText(void* File, StringView Text) const
	{
		NEXUS_ASSERT(File, Default, "Invalid File");

		String Content = StringUtility::Replace(Text, StringUtility::NewLine, NewLine);

		NEXUS_ASSERT(Content.GetCount() <= Integer::MaxUI32, Default, "Currenlty support only file smaller that uint32 max value");

		DWORD Written = 0;
		bool Result = WriteFile(File, Content.C(), (DWORD)Content.GetCount(), &Written, nullptr);

		NEXUS_ASSERT(Result && Written == Content.GetCount(), Default, "Failed to write to file");
	}

	String PlatformWindows::FileReadText(void* File) const
	{
		NEXUS_ASSERT(File, Default, "Invalid File");

		uint64 Size = FileSize(File);
		String Text = String(Size + 1);

		DWORD Read = 0;
		bool Result = ReadFile(File, Text.Characters(), (DWORD)Size, &Read, nullptr);
		Text.Validate();

		NEXUS_ASSERT(Result && Read == Size, Default, "Failed to read to file");

		Text.Assign(NewLine, StringUtility::NewLine);
		return Text;
	}

	PlatformWindows::PlatformWindows()
		: TerminalOut(nullptr), TerminalIn(nullptr), PerformanceFrequency(1.0)
	{
		InitializeTerminal();
		InitializePerformanceTimer();
	}

	PlatformWindows::~PlatformWindows()
	{
	}

	void PlatformWindows::InitializeTerminal()
	{
		TerminalOut = GetStdHandle(STD_OUTPUT_HANDLE);

		DWORD TerminalOutMode = 0;
		GetConsoleMode(TerminalOut, &TerminalOutMode);
		TerminalOutMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
		SetConsoleMode(TerminalOut, TerminalOutMode);

		SetConsoleOutputCP(CP_UTF8);

		TerminalIn = GetStdHandle(STD_INPUT_HANDLE);

		DWORD TerminalInMode = 0;
		GetConsoleMode(TerminalIn, &TerminalInMode);
		TerminalInMode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
		TerminalInMode |= ENABLE_PROCESSED_INPUT;
		SetConsoleMode(TerminalIn, TerminalInMode);
	}

	void PlatformWindows::InitializePerformanceTimer()
	{
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		PerformanceFrequency = 1.0 / (double)Frequency.QuadPart;
	}
}
