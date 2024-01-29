#include <iostream>
#include <windows.h>

#include "NexusEngine.h"

namespace NexusEngine
{
	typedef void(CALLBACK* ProjectFunction)();

	void ExecuteFromDll(const char* DllName, int Ordinal)
	{
		auto dll = LoadLibraryA(DllName);
		if (dll == nullptr)
		{
			std::cout << "Failed to load library" << std::endl;
			return;
		}

		ProjectFunction function = (ProjectFunction)GetProcAddress(dll, MAKEINTRESOURCEA(Ordinal));
		if (function == nullptr)
		{
			std::cout << "Failed to load Function" << std::endl;
			FreeLibrary(dll);
			return;
		}

		function();

		FreeLibrary(dll);
	}

	void WaitForUserCloseConsole()
	{
		std::cin.get();
	}

	void Log(const char* Message)
	{
		std::cout <<  Message << std::endl;
	}

	void LogEngine()
	{
		Log("Hello World (Engine)");
	}
}
