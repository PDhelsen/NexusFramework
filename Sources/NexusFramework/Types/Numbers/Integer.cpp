#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Types/Numbers/Integer.h"

#include "NexusFramework/External/StandardLibrary.h"

namespace NxFr::Integer
{
	int8 MinI8()
	{
		return CHAR_MIN;
	}

	int16 MinI16()
	{
		return SHRT_MIN;
	}

	int32 MinI32()
	{
		return INT_MIN;
	}

	int64 MinI64()
	{
		return LLONG_MIN;
	}

	uint8 MinUI8()
	{
		return 0;
	}

	uint16 MinUI16()
	{
		return 0;
	}

	uint32 MinUI32()
	{
		return 0;
	}

	uint64 MinUI64()
	{
		return 0;
	}

	int8 MaxI8()
	{
		return CHAR_MAX;
	}

	int16 MaxI16()
	{
		return SHRT_MAX;
	}

	int32 MaxI32()
	{
		return INT_MAX;
	}

	int64 MaxI64()
	{
		return LLONG_MAX;
	}

	uint8 MaxUI8()
	{
		return UCHAR_MAX;
	}

	uint16 MaxUI16()
	{
		return USHRT_MAX;
	}

	uint32 MaxUI32()
	{
		return UINT_MAX;
	}

	uint64 MaxUI64()
	{
		return ULLONG_MAX;
	}
}
