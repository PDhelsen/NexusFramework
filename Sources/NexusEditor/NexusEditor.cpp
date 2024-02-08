#include "Debug/Logger.h"

#include "NexusEditor.h"

namespace NxEd
{
	void HelloWorld()
	{
		NEXUS_LOG(Editor, Info, 0, "Hello World")
	}
}
