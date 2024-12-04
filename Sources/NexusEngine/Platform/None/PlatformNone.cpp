#include "Core/NexusEnginePch.h"
#include "PlatformNone.h"

namespace NxEn
{
    void PlatformNone::WaitForUserToCloseTerminal() const
    {
    }

    void PlatformNone::WriteToTerminal(StringView Message, TerminalColor Color) const
    {
    }

    void PlatformNone::WriteToDebugger(StringView Message) const
    {
    }

    void PlatformNone::ExecuteFromDll(StringView DllName, uint8 Ordinal) const
    {
    }

	double PlatformNone::GetProcessorTimer(double Unit) const
    {
		return 0.0;
    }

	Platform::PathType PlatformNone::GetPathType(StringView Path) const
	{
		return PathType::None;
	}

	String PlatformNone::GetWorkingDirectory() const
	{
		return String::Empty;
	}

	List<String> PlatformNone::DirectoryContent(StringView Path) const
    {
        return List<String>();
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

	bool PlatformNone::FileCreate(StringView Path) const
	{
		return false;
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
