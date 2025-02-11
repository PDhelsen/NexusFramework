#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

#include "NexusFramework/IO/Path.h"
#include "NexusFramework/IO/Directory.h"

#include "NexusFramework/Core/NexusFrameworkGlobals.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxFr
{
	static void CreateFolders()
	{
		Directory(Paths::Cooked).Create();
		Directory(Paths::Saved).Create();
	}

	static Path GetDebugPath()
	{
		Path DebugPath = Paths::Saved + "debug";
		Directory(DebugPath).Create();
		return DebugPath;
	}

	static void CreateLogger(const Path& Folder)
	{
		Logger* Logs = new Logger(true, LoggerVerbosity::All, LoggerOutput::All, Folder + "logs.txt");
		Logs->AddChannel(LoggerChannel::Default, true);
		Logs->AddChannel(LoggerChannel::Verbose, false);
		Globals::Logs = Logs;
	}

	static void CreateStats(const Path& Folder)
	{
		Stats* Statistiques = new Stats(Folder + "stats.csv");
		Statistiques->Initialize();
		Statistiques->StartRecording();
		Globals::Statistiques = Statistiques;
	}

	static void CreateIntruments(const Path& Folder)
	{
		Instruments* Instrumentor = Instruments::Create(Folder + "instruments.json", false);
		Instrumentor->StartRecording();
		Globals::Instrumentor = Instrumentor;
	}

	static void DestroyLogger()
	{
		Logger* Logs = Globals::Logs;
		Globals::Logs = nullptr;

		delete Logs;
	}

	static void DestroyStats()
	{
		Stats* Statistiques = Globals::Statistiques;
		Globals::Statistiques = nullptr;

		Statistiques->StopRecording();
		delete Statistiques;
	}

	static void DestroyInstruments()
	{
		Instruments* Instrumentor = Globals::Instrumentor;
		Globals::Instrumentor = nullptr;

		Instrumentor->StopRecording();
		Instruments::Destroy(Instrumentor);
	}

	bool Initialize()
	{
		AllocatorContext Context(nullptr);

		CreateFolders();
		Path DebugPath = GetDebugPath();

		CreateLogger(DebugPath);
		CreateStats(DebugPath);
		CreateIntruments(DebugPath);

		return true;
	}

	bool Shutdown()
	{
		AllocatorContext Context(nullptr);

		DestroyInstruments();
		DestroyStats();
		DestroyLogger();

		Platform::GetInstance()->ClearDll();

		return true;
	}
}
