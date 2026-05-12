#pragma once

#include "NexusFramework/Misc/Hash/Hash.h"

namespace NxFr
{
	namespace Hashing
	{
		template<typename T, typename H>
		struct Hasher<T*, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const T* Data)
			{
				State.Accumulate(reinterpret_cast<uint64>(Data));
			}
		};

		template<typename H>
		struct Hasher<nullptr_t, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const nullptr_t* Data)
			{
				State.Accumulate((uint64)0);
			}
		};
	}
}
