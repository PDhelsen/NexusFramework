#include "Core/NexusEnginePch.h"
#include "NexusEngineEntryPoint.h"

#include "IO/Path.h"
#include "IO/Directory.h"
#include "Debug/Logger/Logger.h"

#include "Core/NexusEngineGlobals.h"

namespace NxEn
{
	void HelloWorld()
	{
		NEXUS_LOG(Info, Default, "Hello World");
	}

	bool Initialize()
	{
		AllocatorContext Context(nullptr);

		Path DebugPath = Path::GetWorkingDirectory() + "Debug";
		Directory DebugDirectory = Directory(DebugPath);
		DebugDirectory.Create();

		Logger* Logs = new Logger(true, LoggerVerbosity::All, LoggerOutput::All, DebugPath + "Logs.txt");
		Logs->AddChannel(LoggerChannel::Default, true);
		Logs->AddChannel(LoggerChannel::Verbose, false);
		Globals::Logs = Logs;
		Globals::Statistiques = new Stats(DebugPath + "Stats.csv");
		Globals::Statistiques->Initialize();
		Globals::Statistiques->StartRecording();
		Globals::Instrumentor = Instruments::Create(DebugPath + "Instruments.json", false);
		Globals::Instrumentor->StartRecording();

		Globals::Handles = new HandleManager(1024);

		return true;
	}

	bool Shutdown()
	{
		AllocatorContext Context(nullptr);

		delete Globals::Handles;

		Globals::Instrumentor->StopRecording();
		Instruments::Destroy(Globals::Instrumentor);
		Globals::Statistiques->StopRecording();
		delete Globals::Statistiques;
		delete (Logger*)Globals::Logs;

		return true;
	}
}
