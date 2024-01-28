#include <iostream>
#include "NexusEngine.h"
#include "NexusEditor.h"

namespace NexusEditor
{
	void LogEditor()
	{
		std::cout << "Hello World (Editor)" << std::endl;
		NexusEngine::LogEngine();
	}
}
