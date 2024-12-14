#include "Core/NexusEnginePch.h"
#include "PlatformNone.h"

namespace NxEn
{
    void PlatformNone::ExecuteFromDll(StringView DllName, uint8 Ordinal) const
    {
    }

	double PlatformNone::GetProcessorTimer(double Unit) const
    {
		return 0.0;
    }

	void PlatformNone::WaitForUserToCloseTerminal() const
    {
    }

    void PlatformNone::WriteToTerminal(StringView Message, TerminalColor Color) const
    {
    }

    void PlatformNone::WriteToDebugger(StringView Message) const
    {
    }

	Platform::PathType PlatformNone::GetPathType(StringView Path) const
	{
		return PathType::None;
	}

	String PlatformNone::GetWorkingDirectory() const
	{
		return String::Empty;
	}

	void PlatformNone::DirectoryCreate(StringView Path) const
	{
	}

	void PlatformNone::DirectoryMove(StringView Path, StringView Target, bool Override) const
	{
	}

	void PlatformNone::DirectoryCopy(StringView Path, StringView Target, bool Override) const
	{
	}

	void PlatformNone::DirectoryDelete(StringView Path) const
	{
	}

	List<String> PlatformNone::DirectoryContent(StringView Path) const
    {
        return List<String>();
    }

	void* PlatformNone::FileCreate(StringView Path, bool KeepOpen) const
	{
		return nullptr;
	}

	void PlatformNone::FileMove(StringView Path, StringView Target, bool Override) const
	{
	}

	void PlatformNone::FileCopy(StringView Path, StringView Target, bool Override) const
	{
	}

	void PlatformNone::FileDelete(StringView Path) const
	{
	}

	void* PlatformNone::FileOpen(StringView Path, FileMode Mode) const
	{
		return nullptr;
	}

	void PlatformNone::FileClose(void* File) const
	{
	}

	uint64 PlatformNone::FileSize(void* File) const
	{
		return 0;
	}

	void PlatformNone::FileWriteByte(void* File, BufferView<Byte> Data) const
	{
	}

	Buffer<Byte> PlatformNone::FileReadByte(void* File, Allocator* Allctr) const
	{
		return Buffer<Byte>(1, nullptr);
	}

    PlatformNone::PlatformNone()
    {
    }

    PlatformNone::~PlatformNone()
    {
    }
}
