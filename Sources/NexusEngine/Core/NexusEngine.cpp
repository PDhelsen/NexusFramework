#include "Core/NexusEnginePch.h"
#include "NexusEngine.h"

#include "Core/NexusEngineGlobals.h"
#include "Memory/MemoryOperator.h"

namespace NxEn
{
	void HelloWorld()
	{ 
		NEXUS_LOG(Info, NxEn::LoggerChannel::Default, "Hello World");
	}

	bool Initialize()
	{
		AllocatorContext Context(nullptr);

		Path DebugPath = Path::GetWorkingDirectory() + "Debug";
		Directory DebugDirectory = Directory(DebugPath);
		DebugDirectory.Create();

		Globals::Logs = new Logger(true, LoggerVerbosity::All, LoggerOutput::All, DebugPath + "Logs.txt");
		Globals::Logs->AddChannel(LoggerChannel::Default, true);
		Globals::Logs->AddChannel(LoggerChannel::Assert, true);
		Globals::Logs->AddChannel(LoggerChannel::Performance, false);
		Globals::Logs->AddChannel(LoggerChannel::Routine, false);
		Globals::Logs->AddChannel(LoggerChannel::UnitTest, false);
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
		delete Globals::Logs;

		return true;
	}
}
