#pragma once

#include "NexusFramework/Memory/Handle/Handle.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	template<typename T>
	struct IsHandle
	{
		static const bool Value = false;
	};

	template<typename T>
	struct IsHandle<Handle<T>>
	{
		static const bool Value = true;
	};

	template<typename T>
	struct Decay<Handle<T>>
	{
		using Type = T;
	};

	template<typename T>
	struct Decay<Handle<const T>>
	{
		using Type = T;
	};

	template<typename T>
	struct DecayPointer<Handle<T>>
	{
		using Type = T;
	};

	template<typename T>
	struct DecayReference<Handle<T>>
	{
		using Type = T;
	};
}
