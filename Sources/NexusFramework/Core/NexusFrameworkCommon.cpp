#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Core/NexusFrameworkCommon.h"

#include "NexusFramework/IO/Path.h"
#include "NexusFramework/IO/Directory.h"
#include "NexusFramework/Threading/Thread.h"
#include "NexusFramework/Platform/Windows/PlatformWindows.h"
#include "NexusFramework/Misc/Arguments/Arguments.h"
#include "NexusFramework/Debug/Logger/Logger.h"
#include "NexusFramework/Debug/Profiler/Stats/Stats.h"
#include "NexusFramework/Debug/Profiler/Instruments/ChromeTracing.h"
#include "NexusFramework/Debug/Profiler/Memory/MemoryTracker.h"

namespace NxFr
{
	void Initialize(uint64 ArgC, char* ArgV[])
	{
		Globals::CreatePlatform();
		Globals::CreateArgs(ArgC, ArgV);
		Globals::CreatePathsAndFolders();
		Globals::CreateDebug(Path::Combine(Globals::Paths::Saved, "debug"));
	}

	void Shutdown()
	{
		Globals::DestroyDebug();
		Globals::DestroyTempFolder();
		Globals::DestroyArgs();
	}

	namespace Globals
	{
		namespace Paths
		{
			String Root;
			String Assets;
			String Configs;
			String Libraries;
			String Resources;
			String Scripts;
			String Sources;
			String Builds;
			String Cooked;
			String Saved;
			String Temp;
		}

		namespace Debug
		{
			Logger* Logs = nullptr;
			Stats* Statistiques = nullptr;
			Instruments* Instrumentor = nullptr;
			MemoryTracker* Memory = nullptr;
		}

		Arguments* Args = nullptr;
		Platform* PlatformTarget = nullptr;
		uint64 MainThreadId = 0;

		void CreatePlatform()
		{
#if NEXUS_WINDOWS
			PlatformTarget = new PlatformWindows();
			MainThreadId = Thread::ThreadId();
#else
			PlatformTarget = nullptr;
			MainThreadId = 0;
#endif
		}

		void CreateArgs(uint64 ArgC, char* ArgV[])
		{
			Args = new Arguments();
			Args->ParseExe(ArgC, ArgV);
			Args->Print();
		}

		void DestroyArgs()
		{
			NEXUS_DELETE(Args);
		}

		void CreatePathsAndFolders()
		{
			Paths::Root = Path::GetWorkingDirectory();
			Paths::Assets = Path::Combine(Paths::Root, "Assets");
			Paths::Configs = Path::Combine(Paths::Root, "Configs");
			Paths::Libraries = Path::Combine(Paths::Root, "Libraries");
			Paths::Resources = Path::Combine(Paths::Root, "Resources");
			Paths::Scripts = Path::Combine(Paths::Root, "Scripts");
			Paths::Sources = Path::Combine(Paths::Root, "Sources");
			Paths::Builds = Path::Combine(Paths::Root, "builds");
			Paths::Cooked = Path::Combine(Paths::Root, "cooked");
			Paths::Saved = Path::Combine(Paths::Root, "saved");
			Paths::Temp = Path::Combine(Paths::Root, "temp");

			Directory(Paths::Cooked).Create();
			Directory(Paths::Saved).Create();
			Directory(Paths::Temp).Create();
		}

		void DestroyTempFolder()
		{
			Directory(Paths::Temp).Delete();
		}

		Log* GetLogger()
		{
			return Debug::Logs;
		}

		void CreateDebug(StringView Path)
		{
			Debug::Memory = new MemoryTracker();
			Debug::Memory->StartRecording();

			Debug::Logs = new Logger(LoggerVerbosity::All, LoggerOutput::All, Path::Combine(Path, "logs.txt"), true);
			Debug::Logs->AddChannel(LoggerChannel::Default, true);
			Debug::Logs->AddChannel(LoggerChannel::Verbose, false);

			Debug::Instrumentor = new ChromeTracing(Path::Combine(Path, "instruments.json"), false, true);
			Debug::Instrumentor->StartRecording();

			Debug::Statistiques = new Stats(Path::Combine(Path, "stats.csv"));
			Debug::Statistiques->Initialize();
			Debug::Statistiques->StartRecording();
		}

		void DestroyDebug()
		{
			Debug::Instrumentor->StopRecording();
			NEXUS_DELETE(Debug::Instrumentor);

			Debug::Statistiques->StopRecording();
			NEXUS_DELETE(Debug::Statistiques);

			NEXUS_DELETE(Debug::Logs);

			Debug::Memory->StopRecording();
			NEXUS_DELETE(Debug::Memory);
		}
	}
}
