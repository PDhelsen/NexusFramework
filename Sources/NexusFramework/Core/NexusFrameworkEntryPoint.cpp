#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Core/NexusFrameworkEntryPoint.h"

#include "NexusFramework/Misc/Arguments.h"
#include "NexusFramework/IO/Path.h"
#include "NexusFramework/IO/Directory.h"

#include "NexusFramework/Core/NexusFrameworkGlobals.h"
#include "NexusFramework/Core/NexusFrameworkPaths.h"

namespace NxFr
{
	static String GetDebugPath()
	{
		String DebugPath = Path::Combine(Paths::Saved, "debug");
		Directory(DebugPath).Create();
		return DebugPath;
	}

	static void CreateLogger(StringView Folder)
	{
		Logger* Logs = new Logger(LoggerVerbosity::All, LoggerOutput::All, Path::Combine(Folder, "logs.txt"), true);
		Logs->AddChannel(LoggerChannel::Default, true);
		Logs->AddChannel(LoggerChannel::Verbose, false);
		Globals::Logs = Logs;
	}

	static void CreateStats(StringView Folder)
	{
		Stats* Statistiques = new Stats(Path::Combine(Folder, "stats.csv"));
		Statistiques->Initialize();
		Statistiques->StartRecording();
		Globals::Statistiques = Statistiques;
	}

	static void CreateIntruments(StringView Folder)
	{
		Instruments* Instrumentor = Instruments::Create(Path::Combine(Folder, "instruments.json"), false, true);
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

	void Initialize(uint64 ArgC, char* ArgV[])
	{
		AllocatorContext Context(nullptr);

		Arguments::Parse(ArgC, ArgV);

		Paths::SetupPathsAndFolders();
		String DebugPath = GetDebugPath();

		CreateLogger(DebugPath);
		CreateStats(DebugPath);
		CreateIntruments(DebugPath);
	}

	void Shutdown()
	{
		AllocatorContext Context(nullptr);

		DestroyInstruments();
		DestroyStats();
		DestroyLogger();

		Paths::CleanupFolders();
	}
}
