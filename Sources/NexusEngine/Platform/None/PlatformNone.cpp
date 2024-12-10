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

	bool PlatformNone::DirectoryMove(StringView Path, StringView Target, bool Override) const
	{
		return false;
	}

	bool PlatformNone::DirectoryCopy(StringView Path, StringView Target, bool Override) const
	{
		return false;
	}

	bool PlatformNone::DirectoryDelete(StringView Path) const
	{
		return false;
	}

	bool PlatformNone::DirectoryContent(StringView Path, List<String>& Result) const
    {
        return false;
    }

	bool PlatformNone::FileCreate(StringView Path, void** Handle) const
	{
		return false;
	}

	bool PlatformNone::FileMove(StringView Path, StringView Target, bool Override) const
	{
		return false;
	}

	bool PlatformNone::FileCopy(StringView Path, StringView Target, bool Override) const
	{
		return false;
	}

	bool PlatformNone::FileDelete(StringView Path) const
	{
		return false;
	}

	bool PlatformNone::FileOpen(StringView Path, void** Handle) const
	{
		return false;
	}

	bool PlatformNone::FileClose(void* File) const
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
