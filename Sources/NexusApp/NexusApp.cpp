#include "Debug/Logger.h"

#include "NexusEngine.h"
#include "NexusEditor.h"
#include "NexusApp.h"


namespace NexusApp
{
	void LogApp()
	{
		NEXUS_LOG(App, Info, 0, "Hello World")
	}
}

int main()
{
	NexusEngine::LogEngine();
	NexusEditor::LogEditor();
	NexusApp::LogApp();

	// TODO: Handle Different dll name and functions
	NexusEngine::ExecuteFromDll("NexusSandbox", 1);

	NexusEngine::WaitForUserCloseConsole();
}