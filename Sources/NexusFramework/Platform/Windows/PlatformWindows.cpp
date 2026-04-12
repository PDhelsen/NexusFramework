#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Platform/Windows/PlatformWindows.h"

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/External/Intrinsics.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/IO/Path.h"

#include <windows.h>
#include <shellapi.h>
#include <process.h>
#include <sys/stat.h>
#include <psapi.h>

namespace NxFr
{
	static SYSTEM_INFO SysInfos;
	static Buffer& GetLocalBuffer() { static Buffer LocalBuffer(512, nullptr); return LocalBuffer; }
	static String ConvertPathToWindows(NxFr::StringView Path) { return StringUtility::Replace(Path, "/", "\\"); }
	static String ConvertPathToNexus(NxFr::StringView Path) { return StringUtility::Replace(Path, "\\", "/"); }

	static StringView TerminalColorToPrefix(Platform::TerminalColor Color)
	{
		switch (Color)
		{
		case NxFr::Platform::TerminalColor::Black: return "\033[30m";
		case NxFr::Platform::TerminalColor::Red: return "\033[31m";
		case NxFr::Platform::TerminalColor::Green: return "\033[32m";
		case NxFr::Platform::TerminalColor::Blue: return "\033[34m";
		case NxFr::Platform::TerminalColor::Yellow: return "\033[33m";
		case NxFr::Platform::TerminalColor::Cyan: return "\033[36m";
		case NxFr::Platform::TerminalColor::Magenta: return "\033[35m";
		case NxFr::Platform::TerminalColor::White: return "\033[37m";
		}

		return "\033[m";
	}

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

	uint64 PlatformWindows::ThreadId() const
	{
		return GetCurrentThreadId();
	}

	void PlatformWindows::ThreadYield() const
	{
		SwitchToThread();
	}

	void PlatformWindows::ThreadSleep(uint64 Milliseconds) const
	{
		Sleep((DWORD)Milliseconds);
	}

	void* PlatformWindows::ThreadCreate(Thread* Instance) const
	{
		HANDLE Handle = (HANDLE)_beginthreadex(NULL, 0, [](void* Ptr) { Platform::ThreadRun(static_cast<Thread*>(Ptr)); return uint32(0); }, Instance, 0, NULL);
		NEXUS_ASSERT(Handle, Default, "Failed to create thread");
		return Handle;
	}

	void PlatformWindows::ThreadDestroy(void* Handle) const
	{
		CloseHandle(Handle);
	}

	void PlatformWindows::ThreadJoin(void* Handle) const
	{
		WaitForSingleObject(Handle, INFINITE);
	}

	void PlatformWindows::ThreadDetach(void* Handle) const
	{
		//CloseHandle(Handle);
	}

	void* PlatformWindows::ThreadMutexCreate() const
	{
		CRITICAL_SECTION* Section = (CRITICAL_SECTION*)HeapAlloc(GetProcessHeap(), 0, sizeof(CRITICAL_SECTION));
		InitializeCriticalSection(Section);
		return Section;
	}

	void PlatformWindows::ThreadMutexDestroy(void* Handle) const
	{
		DeleteCriticalSection((CRITICAL_SECTION*)Handle);
		HeapFree(GetProcessHeap(), 0, Handle);
	}

	void PlatformWindows::ThreadMutexLock(void* Handle) const
	{
		EnterCriticalSection((CRITICAL_SECTION*)Handle);
	}

	void PlatformWindows::ThreadMutexUnlock(void* Handle) const
	{
		LeaveCriticalSection((CRITICAL_SECTION*)Handle);
	}

	void* PlatformWindows::ThreadConditionCreate() const
	{
		CONDITION_VARIABLE* Variable = (CONDITION_VARIABLE*)HeapAlloc(GetProcessHeap(), 0, sizeof(CONDITION_VARIABLE));
		InitializeConditionVariable(Variable);
		return Variable;
	}

	void PlatformWindows::ThreadConditionDestroy(void* Handle) const
	{
		HeapFree(GetProcessHeap(), 0, Handle);
	}

	void PlatformWindows::ThreadConditionWait(void* Handle, void* Target) const
	{
		SleepConditionVariableCS((CONDITION_VARIABLE*)Handle, (CRITICAL_SECTION*)Target, INFINITE);
	}

	void PlatformWindows::ThreadConditionSignal(void* Handle) const
	{
		WakeConditionVariable((CONDITION_VARIABLE*)Handle);
	}

	void PlatformWindows::ThreadConditionBroadcast(void* Handle) const
	{
		WakeAllConditionVariable((CONDITION_VARIABLE*)Handle);
	}

	int64 PlatformWindows::ThreadAtomicIncrement(int64* Instance) const
	{
		return NEXUS_THREAD_INTERLOCKED_INCREMENT(Instance);
	}

	int64 PlatformWindows::ThreadAtomicDecrement(int64* Instance) const
	{
		return NEXUS_THREAD_INTERLOCKED_DECREMENT(Instance);
	}

	int64 PlatformWindows::ThreadAtomicAdd(int64* Instance, int64 Value) const
	{
		return NEXUS_THREAD_INTERLOCKED_ADD(Instance, Value) + Value;
	}

	int64 PlatformWindows::ThreadAtomicLoad(int64* Instance) const
	{
		return NEXUS_THREAD_INTERLOCKED_COMPAREEXCHANGE(Instance, 0, 0);
	}

	void PlatformWindows::ThreadAtomicStore(int64* Instance, int64 Value) const
	{
		NEXUS_THREAD_INTERLOCKED_EXCHANGE(Instance, Value);
	}

	bool PlatformWindows::ThreadAtomicCompareExchange(int64* Instance, int64 Value, int64 Expected) const
	{
		return NEXUS_THREAD_INTERLOCKED_COMPAREEXCHANGE(Instance, Value, Expected) == Expected;
	}

	double PlatformWindows::GetProcessorTimer(double Unit) const
	{
		LARGE_INTEGER Counter;
		QueryPerformanceCounter(&Counter);
		return (double)Counter.QuadPart * Unit * PerformanceFrequency;
	}

	uint64 PlatformWindows::GetProcessId() const
	{
		return GetCurrentProcessId();
	}

	uint64 PlatformWindows::GetProcessorCount() const
	{
		return SysInfos.dwNumberOfProcessors;
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
		WriteToTerminal("\nPress Enter to close ...");

		int8 Value;
		DWORD Read;
		while (true)
		{
			ReadConsoleA(TerminalIn, &Value, 1, &Read, NULL);
			if (Value == '\r')
			{
				break;
			}
		}
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

	void PlatformWindows::WriteToTerminal(StringView Message, TerminalColor Color) const
	{
		printf(TerminalColorToPrefix(Color).C());
		printf(Message.C());
		printf(TerminalColorToPrefix(TerminalColor::None).C());
	}

	void PlatformWindows::WriteToDebugger(StringView Message) const
	{
		OutputDebugStringA(Message.C());
	}

	void PlatformWindows::OpenExplorer(StringView Path) const
	{
		ShellExecuteA(nullptr, "open", "explorer.exe", Path.C(), nullptr, SW_SHOWNORMAL);
	}

	Platform::PathType PlatformWindows::GetPathType(StringView Path) const
	{
		WIN32_FILE_ATTRIBUTE_DATA Data;
		if (!GetFileAttributesExA(Path.C(), GetFileExInfoStandard, &Data))
		{
			return PathType::None;
		}

		if (Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			return PathType::Directory;
		}

		return PathType::File;
	}

	String PlatformWindows::OpenFileDialog(NxFr::StringView Title, NxFr::StringView Extension, NxFr::StringView Name, NxFr::StringView Path) const
	{
		String File = Name + "." + Extension;
		String Directory = ConvertPathToWindows(Path);

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
		return Success ? ConvertPathToNexus(LocalBuffer.GetPtr<char>()) : StringUtility::Empty;
	}

	String PlatformWindows::GetWorkingDirectory() const
	{
		Buffer& LocalBuffer = GetLocalBuffer();

		DWORD Length = GetCurrentDirectoryA((DWORD)LocalBuffer.GetCount(), LocalBuffer.GetPtr<char>());
		NEXUS_ASSERT(Length != 0 && Length < LocalBuffer.GetCount(), Default, "BufferLogs overflowed when getting the current working directory");
		return ConvertPathToNexus(StringView(LocalBuffer.GetPtr<char>(), Length));
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
			bool IsDirectory = (Data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
			if (Name == "." || Name == "..")
			{
				continue;
			}

			Temp.Clear();
			Temp += Path;
			Temp += Name;
			Temp += IsDirectory ? Path::SeparatorFolder : StringUtility::Empty;
			Temp = ConvertPathToNexus(Temp);

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
		GetSystemInfo(&SysInfos);
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
