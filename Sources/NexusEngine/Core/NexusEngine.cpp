#include "Core/NexusPch.h"
#include "NexusEngine.h"

namespace NxEn
{
	typedef void(CALLBACK* ProjectFunction)();

	void ExecuteFromDll(const char* DllName, int8 Ordinal)
	{
		auto dll = LoadLibraryA(DllName);
		if (dll == nullptr)
		{
			NEXUS_LOG(Engine, Error, 0, "Failed to load library")
			return;
		}

		ProjectFunction function = (ProjectFunction)GetProcAddress(dll, MAKEINTRESOURCEA(Ordinal));
		if (function == nullptr)
		{
			NEXUS_LOG(Engine, Error, 0, "Failed to load function")
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

	void HelloWorld()
	{ 
		NEXUS_LOG(Engine, Info, 0, "Hello World")
	}
}
