#pragma once

#include "NexusFramework/Misc/Hash/Hash.h"

#include "NexusFramework/Misc/Color/Color.h"

namespace NxFr
{
	namespace HashUtility
	{
		template<typename H>
		class Hasher<Color, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Color& Data)
			{
				State.Accumulate(Data.r);
				State.Accumulate(Data.g);
				State.Accumulate(Data.b);
				State.Accumulate(Data.a);
			}
		};
	}
}
