#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Platform/Platform.h"

namespace NxFr
{
	class NEXUS_FRAMEWORK_API PlatformWindows : public Platform
	{
	public:
		NEXUS_NOCOPY_NOMOVE(PlatformWindows)
		PlatformWindows();
		virtual ~PlatformWindows();

		void* LoadDll(StringView DllName) override;
		void UnloadDll(StringView DllName) override;
		void* GetFromDll(StringView DllName, StringView FunctionName) override;

		uint64 ThreadId() const override;
		void ThreadYield() const override;
		void ThreadSleep(uint64 Milliseconds) const override;
		void* ThreadCreate(Thread* Instance) const override;
		void ThreadDestroy(void* Handle) const override;
		void ThreadJoin(void* Handle) const override;
		void ThreadDetach(void* Handle) const override;
		void* ThreadMutexCreate() const override;
		void ThreadMutexDestroy(void* Handle) const override;
		void ThreadMutexLock(void* Handle) const override;
		void ThreadMutexUnlock(void* Handle) const override;
		void* ThreadConditionCreate() const override;
		void ThreadConditionDestroy(void* Handle) const override;
		void ThreadConditionWait(void* Handle, void* Target) const override;
		void ThreadConditionSignal(void* Handle) const override;
		void ThreadConditionBroadcast(void* Handle) const override;
		int64 ThreadAtomicIncrement(int64* Instance) const override;
		int64 ThreadAtomicDecrement(int64* Instance) const override;
		int64 ThreadAtomicAdd(int64* Instance, int64 Value) const override;
		int64 ThreadAtomicLoad(int64* Instance) const override;
		void ThreadAtomicStore(int64* Instance, int64 Value) const override;
		bool ThreadAtomicCompareExchange(int64* Instance, int64 Value, int64 Expected) const override;

		double GetProcessorTimer(double Unit = 1.0) const override;
		uint64 GetProcessId() const override;
		uint64 GetProcessorCount() const override;

		MemoryInfo GetMemoryInfo() const override;

		void WaitForUserToCloseTerminal() const override;
		String ReadFromTerminal() const override;
		void WriteToTerminal(StringView Message, TerminalColor Color = TerminalColor::White) const override;
		void WriteToDebugger(StringView Message) const override;

		void OpenExplorer(StringView Path) const override;
		PathType GetPathType(StringView Path) const override;
		String OpenFileDialog(NxFr::StringView Title, NxFr::StringView Extension, NxFr::StringView Name, NxFr::StringView Path) const;
		String GetWorkingDirectory() const override;
		void SetWorkingDirectory(StringView Path) const override;

		void DirectoryCreate(StringView Path) const override;
		void DirectoryMove(StringView Path, StringView Target, bool Override = false) const override;
		void DirectoryCopy(StringView Path, StringView Target, bool Override = false) const override;
		void DirectoryDelete(StringView Path) const override;
		List<String> DirectoryContent(StringView Path) const override;

		void* FileCreate(StringView Path, bool KeepOpen) const override;
		void FileMove(StringView Path, StringView Target, bool Override = false) const override;
		void FileCopy(StringView Path, StringView Target, bool Override = false) const override;
		void FileDelete(StringView Path) const override;
		void* FileOpen(StringView Path, FileMode Mode) const override;
		void FileClose(void* File) const override;
		uint64 FileSize(void* File) const override;
		void FileWriteByte(void* File, BufferView Data) const override;
		Buffer FileReadByte(void* File) const override;
		void FileWriteText(void* File, StringView Text) const override;
		String FileReadText(void* File) const override;
	};
}
