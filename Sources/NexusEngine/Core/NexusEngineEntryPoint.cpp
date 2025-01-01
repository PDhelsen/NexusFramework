#include "Core/NexusEnginePch.h"
#include "NexusEngineEntryPoint.h"

#include "IO/Path.h"
#include "IO/Directory.h"

#include "Core/NexusEngineGlobals.h"

namespace NxEn
{
	static Path GetDebugPath()
	{
		Path DebugPath = Path::GetWorkingDirectory() + "Debug";
		Directory DebugFolder = Directory(DebugPath);
		DebugFolder.Create();
		return DebugPath;
	}

	static void CreateLogger(const Path& Folder)
	{
		Logger* Logs = new Logger(true, LoggerVerbosity::All, LoggerOutput::All, Folder + "Logs.txt");
		Logs->AddChannel(LoggerChannel::Default, true);
		Logs->AddChannel(LoggerChannel::Verbose, false);
		Globals::Logs = Logs;
	}

	static void CreateStats(const Path& Folder)
	{
		Stats* Statistiques = new Stats(Folder + "Stats.csv");
		Statistiques->Initialize();
		Statistiques->StartRecording();
		Globals::Statistiques = Statistiques;
	}

	static void CreateIntruments(const Path& Folder)
	{
		Instruments* Instrumentor = Instruments::Create(Folder + "Instruments.json", false);
		Instrumentor->StartRecording();
		Globals::Instrumentor = Instrumentor;
	}

	static void CreateHandleManager()
	{
		HandleManager* Handles = new HandleManager(1024);
		Globals::Handles = Handles;
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

	static void DestroyHandleManager()
	{
		HandleManager* Handles = Globals::Handles;
		Globals::Handles = nullptr;

		delete Handles;
	}

	bool Initialize()
	{
		AllocatorContext Context(nullptr);

		Path DebugPath = GetDebugPath();
		CreateLogger(DebugPath);
		CreateStats(DebugPath);
		CreateIntruments(DebugPath);

		CreateHandleManager();

		return true;
	}

	bool Shutdown()
	{
		AllocatorContext Context(nullptr);

		DestroyHandleManager();

		DestroyInstruments();
		DestroyStats();
		DestroyLogger();

		return true;
	}
}
