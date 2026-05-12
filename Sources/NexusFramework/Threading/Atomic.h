#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"

namespace NxFr
{
	struct NEXUS_FRAMEWORK_API Atomic
	{
	public:
		NEXUS_NOCOPY_NOMOVE(Atomic)
		Atomic(int64 Value = 0);
		~Atomic();

		int64 Add(int64 Target);
		int64 Increment();
		int64 Decrement();
		int64 Load() const;
		void Store(int64 Target);
		bool CompareExchange(int64 Target, int64 Expected);

	private:
		alignas(8) mutable int64 Value;
	};
}

