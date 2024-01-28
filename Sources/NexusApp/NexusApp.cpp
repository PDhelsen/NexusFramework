#include <iostream>
#include "NexusEngine.h"
#include "NexusEditor.h"

namespace NexusApp
{
	void LogApp()
	{
		std::cout << "Hello World (App)" << std::endl;
		NexusEngine::LogEngine();
		NexusEditor::LogEditor();
	}
}

int main()
{
	NexusApp::LogApp();
	std::cin.get();
}