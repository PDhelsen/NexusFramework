#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Core/NexusFrameworkCommon.h"

#include "NexusFramework/IO/Path.h"
#include "NexusFramework/IO/Directory.h"
#include "NexusFramework/Misc/Arguments.h"
#include "NexusFramework/Debug/Logger/Logger.h"
#include "NexusFramework/Debug/Profiler/Profiler.h"

namespace NxFr
{
	namespace Globals
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

		Arguments* Args = nullptr;

		Logger* Logs = nullptr;
		Stats* Statistiques = nullptr;
		Instruments* Instrumentor = nullptr;
		MemoryTracker* Memory = nullptr;

		void ParseArgs(uint64 ArgC, char* ArgV[])
		{
			Args = new Arguments();
			Args->ParseExe(ArgC, ArgV);
			Args->Print();
		}

		void ReleaseArgs()
		{
			NEXUS_DELETE(Args);

		}
		void SetupPathsAndFolders()
		{
			Root = Path::GetWorkingDirectory();
			Assets = Path::Combine(Root, "Assets");
			Configs = Path::Combine(Root, "Configs");
			Libraries = Path::Combine(Root, "Libraries");
			Resources = Path::Combine(Root, "Resources");
			Scripts = Path::Combine(Root, "Scripts");
			Sources = Path::Combine(Root, "Sources");
			Builds = Path::Combine(Root, "builds");
			Cooked = Path::Combine(Root, "cooked");
			Saved = Path::Combine(Root, "saved");
			Temp = Path::Combine(Root, "temp");

			Directory(Cooked).Create();
			Directory(Saved).Create();
			Directory(Temp).Create();
		}

		void CleanupFolders()
		{
			Directory(Temp).Delete();
		}

		Log* GetLogger()
		{
			return Logs;
		}

		void CreateDebug(StringView Path)
		{
			Memory = new MemoryTracker();
			Memory->StartRecording();

			Logs = new Logger(LoggerVerbosity::All, LoggerOutput::All, Path::Combine(Path, "logs.txt"), true);
			Logs->AddChannel(LoggerChannel::Default, true);
			Logs->AddChannel(LoggerChannel::Verbose, false);

			Instrumentor = new ChromeTracing(Path::Combine(Path, "instruments.json"), false, true);
			Instrumentor->StartRecording();

			Statistiques = new Stats(Path::Combine(Path, "stats.csv"));
			Statistiques->Initialize();
			Statistiques->StartRecording();
		}

		void DestroyDebug()
		{
			Instrumentor->StopRecording();
			NEXUS_DELETE(Instrumentor);

			Statistiques->StopRecording();
			NEXUS_DELETE(Statistiques);

			NEXUS_DELETE(Logs);

			Memory->StopRecording();
			NEXUS_DELETE(Memory);
		}
	}

	void Initialize(uint64 ArgC, char* ArgV[])
	{
		Globals::ParseArgs(ArgC, ArgV);
		Globals::SetupPathsAndFolders();
		Globals::CreateDebug(Path::Combine(Globals::Saved, "debug"));
	}

	void Shutdown()
	{
		Globals::DestroyDebug();
		Globals::CleanupFolders();
		Globals::ReleaseArgs();
	}
}
