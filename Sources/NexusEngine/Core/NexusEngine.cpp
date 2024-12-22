#include "Core/NexusEnginePch.h"
#include "NexusEngine.h"
#include "Memory/MemoryOperator.h"

namespace NxEn
{
	void HelloWorld()
	{ 
		NEXUS_LOG(Info, NxEn::LoggerChannel::Default, "Hello World");
	}
}
