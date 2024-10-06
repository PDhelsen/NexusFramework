#include "NexusEditor.h"
#include "Memory/MemoryOperator.h"

#include "Core/NexusEngineBase.h"

namespace NxEd
{
	void HelloWorld()
	{
		NEXUS_LOG(Editor, Info, NxEn::LoggerChannel::Default, "Hello World")
	}
}
