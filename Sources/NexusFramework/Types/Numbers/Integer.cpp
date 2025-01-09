#include "Core/NexusFrameworkPch.h"
#include "Integer.h"

#include "External/StandardLibrary.h"

namespace NxEn::Integer
{
	constexpr int8 MinI8()
	{
		return CHAR_MIN;
	}

	constexpr int16 MinI16()
	{
		return SHRT_MIN;
	}

	constexpr int32 MinI32()
	{
		return INT_MIN;
	}

	constexpr int64 MinI64()
	{
		return LLONG_MIN;
	}

	constexpr uint8 MinUI8()
	{
		return 0;
	}

	constexpr uint16 MinUI16()
	{
		return 0;
	}

	constexpr uint32 MinUI32()
	{
		return 0;
	}

	constexpr uint64 MinUI64()
	{
		return 0;
	}

	constexpr int8 MaxI8()
	{
		return CHAR_MAX;
	}

	constexpr int16 MaxI16()
	{
		return SHRT_MAX;
	}

	constexpr int32 MaxI32()
	{
		return INT_MAX;
	}

	constexpr int64 MaxI64()
	{
		return LLONG_MAX;
	}

	constexpr uint8 MaxUI8()
	{
		return UCHAR_MAX;
	}

	constexpr uint16 MaxUI16()
	{
		return USHRT_MAX;
	}

	constexpr uint32 MaxUI32()
	{
		return UINT_MAX;
	}

	constexpr uint64 MaxUI64()
	{
		return ULLONG_MAX;
	}
}
