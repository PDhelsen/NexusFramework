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

	bool PlatformNone::DirectoryCreate(StringView Path) const
	{
		return false;
	}

	bool PlatformNone::DirectoryMove(StringView Path, StringView Target) const
	{
		return false;
	}

	bool PlatformNone::DirectoryDelete(StringView Path) const
	{
		return false;
	}

	List<String> PlatformNone::DirectoryContent(StringView Path) const
    {
        return List<String>();
    }

	void* PlatformNone::FileOpen(StringView Path) const
	{
		return nullptr;
	}

	bool PlatformNone::FileClose(void* File) const
	{
		return false;
	}

	void* PlatformNone::FileCreate(StringView Path) const
	{
		return nullptr;
	}

	bool PlatformNone::FileMove(StringView Path, StringView Target) const
	{
		return false;
	}

	bool PlatformNone::FileDelete(StringView Path) const
	{
		return false;
	}

    PlatformNone::PlatformNone()
    {
    }

    PlatformNone::~PlatformNone()
    {
    }
}
