#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Threading/Threading.h"

namespace NxFr
{
	namespace Threading
	{
		static uint64 MainThread = Thread::ThreadId();

		bool IsMainThread()
		{
			return MainThreadId() == Thread::ThreadId();
		}

		uint64 MainThreadId()
		{
			return MainThread;
		}

		uint64 MaxThreadCount()
		{
			return Math::Max(1llu, Platform::GetInstance()->GetProcessorCount() - 1);
		}

		void Dispatch(uint64 Count, Delegate<void(uint64)> Function)
		{
			uint64 Test = MainThreadId();
			uint64 ThreadCount = Math::Min(MaxThreadCount(), Count);
			ThreadPool Pool(ThreadCount);

			for (uint64 Index = 0; Index < Count; ++Index)
			{
				Pool.Submit([=]()
				{
					Function.Invoke(Index);
				});
			}

			Pool.Wait();
		}

		void Dispatch(uint64 Count, uint64 Size, const Delegate<void(uint64)>& Function)
		{
			uint64 GroupCount = Math::Ceil((float)Count / (float)Size);
			uint64 ThreadCount = Math::Min(MaxThreadCount(), GroupCount);
			ThreadPool Pool(ThreadCount);

			for (uint64 Index = 0; Index < GroupCount; ++Index)
			{
				Pool.Submit([=]()
				{
					uint64 Begin = Index * Size;
					uint64 End = Math::Min(Begin + Size, Count);
					for (uint64 It = Begin; It < End; ++It)
					{
						Function(It);
					}
				});
			}

			Pool.Wait();
		}
	}
}
