#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Platform/Platform.h"

namespace NxFr
{
	class PlatformWindows : public Platform
	{
		friend Platform;

	public:
		inline static const String NewLine = "\r\n";
		inline static const String Backspace = "\b \b";

		NEXUS_FRAMEWORK_API void* LoadDll(StringView DllName) override;
		NEXUS_FRAMEWORK_API void UnloadDll(StringView DllName) override;
		NEXUS_FRAMEWORK_API void* GetFromDll(StringView DllName, StringView FunctionName) override;

		NEXUS_FRAMEWORK_API uint64 ThreadId() const override;
		NEXUS_FRAMEWORK_API void ThreadYield() const override;
		NEXUS_FRAMEWORK_API void ThreadSleep(uint64 Milliseconds) const override;
		NEXUS_FRAMEWORK_API void* ThreadCreate(Thread* Instance) const override;
		NEXUS_FRAMEWORK_API void ThreadDestroy(void* Handle) const override;
		NEXUS_FRAMEWORK_API void ThreadJoin(void* Handle) const override;
		NEXUS_FRAMEWORK_API void ThreadDetach(void* Handle) const override;
		NEXUS_FRAMEWORK_API void* ThreadMutexCreate() const override;
		NEXUS_FRAMEWORK_API void ThreadMutexDestroy(void* Handle) const override;
		NEXUS_FRAMEWORK_API void ThreadMutexLock(void* Handle) const override;
		NEXUS_FRAMEWORK_API void ThreadMutexUnlock(void* Handle) const override;
		NEXUS_FRAMEWORK_API void* ThreadConditionCreate() const override;
		NEXUS_FRAMEWORK_API void ThreadConditionDestroy(void* Handle) const override;
		NEXUS_FRAMEWORK_API void ThreadConditionWait(void* Handle, void* Target) const override;
		NEXUS_FRAMEWORK_API void ThreadConditionSignal(void* Handle) const override;
		NEXUS_FRAMEWORK_API void ThreadConditionBroadcast(void* Handle) const override;
		NEXUS_FRAMEWORK_API int64 ThreadAtomicIncrement(int64* Instance) const override;
		NEXUS_FRAMEWORK_API int64 ThreadAtomicDecrement(int64* Instance) const override;
		NEXUS_FRAMEWORK_API int64 ThreadAtomicAdd(int64* Instance, int64 Value) const override;
		NEXUS_FRAMEWORK_API int64 ThreadAtomicLoad(int64* Instance) const override;
		NEXUS_FRAMEWORK_API void ThreadAtomicStore(int64* Instance, int64 Value) const override;
		NEXUS_FRAMEWORK_API bool ThreadAtomicCompareExchange(int64* Instance, int64 Value, int64 Expected) const override;

		NEXUS_FRAMEWORK_API double GetProcessorTimer(double Unit = 1.0) const override;
		NEXUS_FRAMEWORK_API uint64 GetProcessId() const override;

		NEXUS_FRAMEWORK_API MemoryInfo GetMemoryInfo() const override;

		NEXUS_FRAMEWORK_API void WaitForUserToCloseTerminal() const override;
		NEXUS_FRAMEWORK_API String ReadFromTerminal() const override;
		NEXUS_FRAMEWORK_API void WriteToTerminal(StringView Message) const override;
		NEXUS_FRAMEWORK_API void WriteToDebugger(StringView Message) const override;

		NEXUS_FRAMEWORK_API PathType GetPathType(StringView Path) const override;
		NEXUS_FRAMEWORK_API String OpenFileDialog(NxFr::StringView Title, NxFr::StringView Extension, NxFr::StringView Name, NxFr::StringView Path) const;
		NEXUS_FRAMEWORK_API String GetWorkingDirectory() const override;
		NEXUS_FRAMEWORK_API void SetWorkingDirectory(StringView Path) const override;

		NEXUS_FRAMEWORK_API void DirectoryCreate(StringView Path) const override;
		NEXUS_FRAMEWORK_API void DirectoryMove(StringView Path, StringView Target, bool Override = false) const override;
		NEXUS_FRAMEWORK_API void DirectoryCopy(StringView Path, StringView Target, bool Override = false) const override;
		NEXUS_FRAMEWORK_API void DirectoryDelete(StringView Path) const override;
		NEXUS_FRAMEWORK_API List<String> DirectoryContent(StringView Path) const override;

		NEXUS_FRAMEWORK_API void* FileCreate(StringView Path, bool KeepOpen) const override;
		NEXUS_FRAMEWORK_API void FileMove(StringView Path, StringView Target, bool Override = false) const override;
		NEXUS_FRAMEWORK_API void FileCopy(StringView Path, StringView Target, bool Override = false) const override;
		NEXUS_FRAMEWORK_API void FileDelete(StringView Path) const override;
		NEXUS_FRAMEWORK_API void* FileOpen(StringView Path, FileMode Mode) const override;
		NEXUS_FRAMEWORK_API void FileClose(void* File) const override;
		NEXUS_FRAMEWORK_API uint64 FileSize(void* File) const override;
		NEXUS_FRAMEWORK_API void FileWriteByte(void* File, BufferView Data) const override;
		NEXUS_FRAMEWORK_API Buffer FileReadByte(void* File) const override;
		NEXUS_FRAMEWORK_API void FileWriteText(void* File, StringView Text) const override;
		NEXUS_FRAMEWORK_API String FileReadText(void* File) const override;

		NEXUS_FRAMEWORK_API inline PlatformTarget GetTarget() override { return PlatformTarget::Windows; }

	protected:
		PlatformWindows();
		virtual ~PlatformWindows();

	private:
		void InitializeTerminal();
		void InitializePerformanceTimer();

		void* TerminalOut;
		void* TerminalIn;
		double PerformanceFrequency;
	};
}
