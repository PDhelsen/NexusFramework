#include "NexusEngine.h"
#include "NexusEditor.h"
#include "NexusApp.h"

namespace NexusApp
{
	void LogApp()
	{
		NexusEngine::Log("Hello World (App)");
	}
}

int main()
{
	NexusEngine::LogEngine();
	NexusEditor::LogEditor();
	NexusApp::LogApp();

	NexusEngine::ExecuteFromDll("NexusSandbox", 1);

	NexusEngine::WaitForUserCloseConsole();
}