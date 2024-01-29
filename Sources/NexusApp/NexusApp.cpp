#include <iostream>
#include <windows.h>
#include "NexusEngine.h"
#include "NexusEditor.h"

namespace NexusApp
{
	typedef void(CALLBACK* ProjectFunction)();

	void LogProject()
	{
		auto dll = LoadLibraryA("NexusSandbox");
		if (dll == nullptr)
		{
			std::cout << "Failed to load library" << std::endl;
			return;
		}

		int ordinal = 1;
		ProjectFunction function = (ProjectFunction)GetProcAddress(dll, MAKEINTRESOURCEA(ordinal));
		if (function == nullptr)
		{
			std::cout << "Failed to load Function" << std::endl;
			FreeLibrary(dll);
			return;
		}

		function();

		FreeLibrary(dll);
	}

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
	NexusApp::LogProject();
	std::cin.get();
}