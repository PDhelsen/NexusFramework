#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Buffer.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Threading/Thread.h"

namespace NxFr
{
	enum class PlatformTarget : uint8
	{
		None,
		Windows
	};

	class Platform
	{
	public:
		enum class PathType : uint32
		{
			None, File, Directory, Other
		};

		enum class FileMode
		{
			Read, Write, Append
		};

		struct MemoryInfo
		{
		public:
			uint64 CurrentUsage;
			uint64 PeakUsage;
			uint64 CurrentAllocated;
			uint64 PeakAllocated;
		};

		NEXUS_FRAMEWORK_API static Platform* GetInstance();

		template<typename R, typename... Args>
		Delegate<R(Args...)> GetFunctionFromDll(StringView DllName, StringView FunctionName);

		NEXUS_FRAMEWORK_API virtual void* LoadDll(StringView DllName) = 0;
		NEXUS_FRAMEWORK_API virtual void UnloadDll(StringView DllName) = 0;
		NEXUS_FRAMEWORK_API virtual void* GetFromDll(StringView DllName, StringView FunctionName) = 0;
		NEXUS_FRAMEWORK_API virtual void ClearDll();

		NEXUS_FRAMEWORK_API virtual uint64 ThreadId() const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadYield() const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadSleep(uint64 Milliseconds) const = 0;
		NEXUS_FRAMEWORK_API virtual void* ThreadCreate(Thread* Instance) const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadDestroy(void* Handle) const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadJoin(void* Handle) const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadDetach(void* Handle) const = 0;
		NEXUS_FRAMEWORK_API virtual void* ThreadMutexCreate() const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadMutexDestroy(void* Handle) const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadMutexLock(void* Handle) const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadMutexUnlock(void* Handle) const = 0;
		NEXUS_FRAMEWORK_API virtual void* ThreadConditionCreate() const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadConditionDestroy(void* Handle) const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadConditionWait(void* Handle, void* Target) const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadConditionSignal(void* Handle) const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadConditionBroadcast(void* Handle) const = 0;
		NEXUS_FRAMEWORK_API virtual int64 ThreadAtomicIncrement(int64* Instance) const = 0;
		NEXUS_FRAMEWORK_API virtual int64 ThreadAtomicDecrement(int64* Instance) const = 0;
		NEXUS_FRAMEWORK_API virtual int64 ThreadAtomicAdd(int64* Instance, int64 Value) const = 0;
		NEXUS_FRAMEWORK_API virtual int64 ThreadAtomicLoad(int64* Instance) const = 0;
		NEXUS_FRAMEWORK_API virtual void ThreadAtomicStore(int64* Instance, int64 Value) const = 0;
		NEXUS_FRAMEWORK_API virtual bool ThreadAtomicCompareExchange(int64* Instance, int64 Value, int64 Expected) const = 0;

		NEXUS_FRAMEWORK_API virtual double GetProcessorTimer(double Unit = 1.0) const = 0;
		NEXUS_FRAMEWORK_API virtual uint64 GetProcessId() const = 0;
		NEXUS_FRAMEWORK_API virtual uint64 GetProcessorCount() const = 0;

		NEXUS_FRAMEWORK_API virtual MemoryInfo GetMemoryInfo() const = 0;

		NEXUS_FRAMEWORK_API virtual void WaitForUserToCloseTerminal() const = 0;
		NEXUS_FRAMEWORK_API virtual String ReadFromTerminal() const = 0;
		NEXUS_FRAMEWORK_API virtual void WriteToTerminal(StringView Message) const = 0;
		NEXUS_FRAMEWORK_API virtual void WriteToDebugger(StringView Message) const = 0;

		NEXUS_FRAMEWORK_API virtual void OpenExplorer(StringView Path) const = 0;
		NEXUS_FRAMEWORK_API virtual PathType GetPathType(StringView Path) const = 0;
		NEXUS_FRAMEWORK_API virtual String OpenFileDialog(NxFr::StringView Title, NxFr::StringView Extension, NxFr::StringView Name, NxFr::StringView Path) const = 0;
		NEXUS_FRAMEWORK_API virtual String GetWorkingDirectory() const = 0;
		NEXUS_FRAMEWORK_API virtual void SetWorkingDirectory(StringView Path) const = 0;

		NEXUS_FRAMEWORK_API virtual void DirectoryCreate(StringView Path) const = 0;
		NEXUS_FRAMEWORK_API virtual void DirectoryMove(StringView Path, StringView Target, bool Override = false) const = 0;
		NEXUS_FRAMEWORK_API virtual void DirectoryCopy(StringView Path, StringView Target, bool Override = false) const = 0;
		NEXUS_FRAMEWORK_API virtual void DirectoryDelete(StringView Path) const = 0;
		NEXUS_FRAMEWORK_API virtual List<String> DirectoryContent(StringView Path) const = 0;

		NEXUS_FRAMEWORK_API virtual void* FileCreate(StringView Path, bool KeepOpen) const = 0;
		NEXUS_FRAMEWORK_API virtual void FileMove(StringView Path, StringView Target, bool Override = false) const = 0;
		NEXUS_FRAMEWORK_API virtual void FileCopy(StringView Path, StringView Target, bool Override = false) const = 0;
		NEXUS_FRAMEWORK_API virtual void FileDelete(StringView Path) const = 0;
		NEXUS_FRAMEWORK_API virtual void* FileOpen(StringView Path, FileMode Mode) const = 0;
		NEXUS_FRAMEWORK_API virtual void FileClose(void* File) const = 0;
		NEXUS_FRAMEWORK_API virtual uint64 FileSize(void* File) const = 0;
		NEXUS_FRAMEWORK_API virtual void FileWriteByte(void* File, BufferView Data) const = 0;
		NEXUS_FRAMEWORK_API virtual Buffer FileReadByte(void* File) const = 0;
		NEXUS_FRAMEWORK_API virtual void FileWriteText(void* File, StringView Text) const = 0;
		NEXUS_FRAMEWORK_API virtual String FileReadText(void* File) const = 0;

		NEXUS_FRAMEWORK_API inline virtual PlatformTarget GetTarget() { return PlatformTarget::None; }

	protected:
		Platform() = default;
		virtual ~Platform() = default;

		static void ThreadRun(Thread* Instance);

		Dictionary<String, void*> Dlls;
	};

	template<typename R, typename ...Args>
	inline Delegate<R(Args...)> Platform::GetFunctionFromDll(StringView DllName, StringView FunctionName)
	{
		typedef R(*Type)(Args...);
		return Delegate<R(Args...)>((Type)GetFromDll(DllName, FunctionName));
	}
}
