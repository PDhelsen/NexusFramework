#include "Core/NexusEnginePch.h"
#include "NexusEngine.h"
#include "Memory/MemoryOperator.h"

namespace NxEn
{
	void HelloWorld()
	{ 
		NEXUS_LOG(Engine, Info, NxEn::LoggerChannel::Default, "Hello World")
	}
}
