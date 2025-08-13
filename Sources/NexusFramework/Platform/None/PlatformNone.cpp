#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Platform/None/PlatformNone.h"

namespace NxFr
{
	void* PlatformNone::LoadDll(StringView DllName)
	{
		return nullptr;
	}

	void PlatformNone::UnloadDll(StringView DllName)
	{
	}

	void* PlatformNone::GetFromDll(StringView DllName, StringView FunctionName)
    {
		return nullptr;
	}

	void PlatformNone::Sleep(uint64 Milliseconds) const
	{
	}

	double PlatformNone::GetProcessorTimer(double Unit) const
    {
		return 0.0;
    }

	Platform::MemoryInfo PlatformNone::GetMemoryInfo() const
	{
		return (MemoryInfo)0;
	}

	void PlatformNone::WaitForUserToCloseTerminal() const
    {
    }

    void PlatformNone::WriteToTerminal(StringView Message) const
    {
    }

    void PlatformNone::WriteToDebugger(StringView Message) const
    {
    }

	Platform::PathType PlatformNone::GetPathType(StringView Path) const
	{
		return PathType::None;
	}

	String PlatformNone::OpenFileDialog(NxFr::StringView Title, NxFr::StringView Extension, NxFr::StringView Name, NxFr::StringView Path) const
	{
		return StringUtility::Empty;
	}

	String PlatformNone::GetWorkingDirectory() const
	{
		return StringUtility::Empty;
	}

	void PlatformNone::SetWorkingDirectory(StringView Path) const
	{
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

	void PlatformNone::FileWriteByte(void* File, BufferView Data) const
	{
	}

	Buffer PlatformNone::FileReadByte(void* File) const
	{
		return Buffer();
	}

	void PlatformNone::FileWriteText(void* File, StringView Text) const
	{
	}

	String PlatformNone::FileReadText(void* File) const
	{
		return StringUtility::Empty;
	}

    PlatformNone::PlatformNone()
    {
    }

    PlatformNone::~PlatformNone()
    {
    }
}
