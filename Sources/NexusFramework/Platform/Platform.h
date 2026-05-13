#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Buffer.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Threading/Thread.h"

namespace NxFr
{
	class NEXUS_FRAMEWORK_API Platform
	{
	public:
		enum class TerminalColor : uint8
		{
			None, Black, Red, Green, Blue, Yellow, Cyan, Magenta, White
		};

		enum class PathType : uint32
		{
			None, File, Directory, Other
		};

		enum class FileMode
		{
			None, Read, Write, Append
		};

		struct NEXUS_FRAMEWORK_API MemoryInfo
		{
		public:
			uint64 CurrentUsage;
			uint64 PeakUsage;
			uint64 CurrentAllocated;
			uint64 PeakAllocated;
		};

		NEXUS_NOCOPY_NOMOVE(Platform)
		Platform();
		virtual ~Platform();

		virtual double GetProcessorTimer(double Unit = 1.0) const = 0;
		virtual uint64 GetProcessId() const = 0;
		virtual uint64 GetProcessorCount() const = 0;
		virtual MemoryInfo GetMemoryInfo() const = 0;

		virtual void WaitForUserToCloseTerminal() const = 0;
		virtual String ReadFromTerminal() const = 0;
		virtual void WriteToTerminal(StringView Message, TerminalColor Color = TerminalColor::White) const = 0;
		virtual void WriteToDebugger(StringView Message) const = 0;

		virtual PathType GetPathType(StringView Path) const = 0;
		virtual void OpenExplorer(StringView Path) const = 0;
		virtual String OpenFileDialog(NxFr::StringView Title, NxFr::StringView Extension, NxFr::StringView Name, NxFr::StringView Path) const = 0;
		virtual String GetWorkingDirectory() const = 0;
		virtual void SetWorkingDirectory(StringView Path) const = 0;
		
		virtual void* FileCreate(StringView Path, bool KeepOpen) const = 0;
		virtual void FileMove(StringView Path, StringView Target, bool Override = false) const = 0;
		virtual void FileCopy(StringView Path, StringView Target, bool Override = false) const = 0;
		virtual void FileDelete(StringView Path) const = 0;
		virtual void* FileOpen(StringView Path, FileMode Mode) const = 0;
		virtual void FileClose(void* File) const = 0;
		virtual uint64 FileSize(void* File) const = 0;
		virtual void FileWriteByte(void* File, BufferView Data) const = 0;
		virtual Buffer FileReadByte(void* File) const = 0;
		virtual void FileWriteText(void* File, StringView Text) const = 0;
		virtual String FileReadText(void* File) const = 0;

		virtual void DirectoryCreate(StringView Path) const = 0;
		virtual void DirectoryMove(StringView Path, StringView Target, bool Override = false) const = 0;
		virtual void DirectoryCopy(StringView Path, StringView Target, bool Override = false) const = 0;
		virtual void DirectoryDelete(StringView Path) const = 0;
		virtual List<String> DirectoryContent(StringView Path) const = 0;

		template<typename R, typename... Args>
		Delegate<R(Args...)> GetFunctionFromDll(StringView DllName, StringView FunctionName)
		{
			typedef R(*Type)(Args...);
			return Delegate<R(Args...)>((Type)GetFromDll(DllName, FunctionName));
		}
		virtual void* LoadDll(StringView DllName) = 0;
		virtual void UnloadDll(StringView DllName) = 0;
		virtual void* GetFromDll(StringView DllName, StringView FunctionName) = 0;
		virtual void ClearDll();

		virtual uint64 ThreadId() const = 0;
		virtual void ThreadYield() const = 0;
		virtual void ThreadSleep(uint64 Milliseconds) const = 0;
		virtual void* ThreadCreate(Thread* Instance) const = 0;
		virtual void ThreadRun(Thread* Instance) const;
		virtual void ThreadDestroy(void* Handle) const = 0;
		virtual void ThreadJoin(void* Handle) const = 0;
		virtual void ThreadDetach(void* Handle) const = 0;
		virtual void* ThreadMutexCreate() const = 0;
		virtual void ThreadMutexDestroy(void* Handle) const = 0;
		virtual void ThreadMutexLock(void* Handle) const = 0;
		virtual void ThreadMutexUnlock(void* Handle) const = 0;
		virtual void* ThreadConditionCreate() const = 0;
		virtual void ThreadConditionDestroy(void* Handle) const = 0;
		virtual void ThreadConditionWait(void* Handle, void* Target) const = 0;
		virtual void ThreadConditionSignal(void* Handle) const = 0;
		virtual void ThreadConditionBroadcast(void* Handle) const = 0;
		virtual int64 ThreadAtomicIncrement(int64* Instance) const = 0;
		virtual int64 ThreadAtomicDecrement(int64* Instance) const = 0;
		virtual int64 ThreadAtomicAdd(int64* Instance, int64 Value) const = 0;
		virtual int64 ThreadAtomicLoad(int64* Instance) const = 0;
		virtual void ThreadAtomicStore(int64* Instance, int64 Value) const = 0;
		virtual bool ThreadAtomicCompareExchange(int64* Instance, int64 Value, int64 Expected) const = 0;

	protected:
		Dictionary<String, void*> Dlls;
	};
}
