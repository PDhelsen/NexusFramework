#include "Core/NexusEnginePch.h"
#include "NexusEngine.h"

#include "Core/NexusEngineGlobals.h"
#include "Memory/MemoryOperator.h"

#define NEXUS_LOGS_FLUSH true
#define NEXUS_LOGS_VERBOSITY LoggerVerbosity::All
#define NEXUS_LOGS_OUTPUT LoggerOutput::All
#define NEXUS_LOGS_PATH Path::GetWorkingDirectory() + "Logs.txt"

#define NEXUS_STATISTIQUES_PATH Path::GetWorkingDirectory() + "Stats.csv"

#define NEXUS_INSTRUMENTOR_PATH Path::GetWorkingDirectory() + "Instruments.json"
#define NEXUS_INSTRUMENTOR_START false
#define NEXUS_INSTRUMENTOR_TOOL NEXUS_DEFAULT_INSTRUMENTOR

#define NEXUS_HANDLES_COUNT 1024

namespace NxEn
{
	void HelloWorld()
	{ 
		NEXUS_LOG(Info, NxEn::LoggerChannel::Default, "Hello World");
	}

	bool Initialize()
	{
		AllocatorContext Context(nullptr);

		Globals::Logs = new Logger(NEXUS_LOGS_FLUSH, NEXUS_LOGS_VERBOSITY, NEXUS_LOGS_OUTPUT, NEXUS_LOGS_PATH);
		Globals::Logs->AddChannel(LoggerChannel::Default, true);
		Globals::Logs->AddChannel(LoggerChannel::Assert, true);
		Globals::Logs->AddChannel(LoggerChannel::Performance, false);
		Globals::Logs->AddChannel(LoggerChannel::Routine, false);
		Globals::Logs->AddChannel(LoggerChannel::UnitTest, false);
		Globals::Statistiques = new Stats(NEXUS_STATISTIQUES_PATH);
		Globals::Statistiques->Initialize();
		Globals::Statistiques->StartRecording();
		Globals::Instrumentor = Instruments::Create(NEXUS_INSTRUMENTOR_PATH, NEXUS_INSTRUMENTOR_START, NEXUS_INSTRUMENTOR_TOOL);
		Globals::Instrumentor->StartRecording();

		Globals::Handles = new HandleManager(NEXUS_HANDLES_COUNT);


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
