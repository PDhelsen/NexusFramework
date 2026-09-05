#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Misc/Pattern/Context.h"

namespace NxFr
{
	Allocator::Scope::Scope(Allocator* Instance)
	{
		Allocator::GetContexts().Push(Instance);
	}

	Allocator::Scope::~Scope()
	{
		Allocator::GetContexts().Pop();
	}

	Context<Allocator>& Allocator::GetContexts()
	{
		static thread_local Context<Allocator> Contexts;
		return Contexts;
	}

	Allocator* Allocator::TryGet()
	{
		return Allocator::GetContexts().TryGet();
	}
}
