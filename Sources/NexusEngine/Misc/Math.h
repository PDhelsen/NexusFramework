#pragma once

#include "Core/NexusEngine.h"
#include "Types/Integer.h"

namespace NxEn
{
	namespace Math
	{
		NEXUS_ENGINE_API			float	Min					(float		X, float	Y);
		NEXUS_ENGINE_API			double	Min					(double		X, double	Y);
		NEXUS_ENGINE_API			int8	Min					(int8		X, int8		Y);
		NEXUS_ENGINE_API			int16	Min					(int16		X, int16	Y);
		NEXUS_ENGINE_API			int32	Min					(int32		X, int32	Y);
		NEXUS_ENGINE_API			int64	Min					(int64		X, int64	Y);
		NEXUS_ENGINE_API			uint8	Min					(uint8		X, uint8	Y);
		NEXUS_ENGINE_API			uint16	Min					(uint16		X, uint16	Y);
		NEXUS_ENGINE_API			uint32	Min					(uint32		X, uint32	Y);
		NEXUS_ENGINE_API			uint64	Min					(uint64		X, uint64	Y);
		NEXUS_ENGINE_API			float	Max					(float		X, float	Y);
		NEXUS_ENGINE_API			double	Max					(double		X, double	Y);
		NEXUS_ENGINE_API			int8	Max					(int8		X, int8		Y);
		NEXUS_ENGINE_API			int16	Max					(int16		X, int16	Y);
		NEXUS_ENGINE_API			int32	Max					(int32		X, int32	Y);
		NEXUS_ENGINE_API			int64	Max					(int64		X, int64	Y);
		NEXUS_ENGINE_API			uint8	Max					(uint8		X, uint8	Y);
		NEXUS_ENGINE_API			uint16	Max					(uint16		X, uint16	Y);
		NEXUS_ENGINE_API			uint32	Max					(uint32		X, uint32	Y);
		NEXUS_ENGINE_API			uint64	Max					(uint64		X, uint64	Y);

		NEXUS_ENGINE_API			float	Acos				(float		X);
		NEXUS_ENGINE_API			double	Acos				(double		X);
		NEXUS_ENGINE_API			float	Asin				(float		X);
		NEXUS_ENGINE_API			double	Asin				(double		X);
		NEXUS_ENGINE_API			float	Atan				(float		X);
		NEXUS_ENGINE_API			double	Atan				(double		X);
		NEXUS_ENGINE_API			float	Atan				(float		Y,	float X);
		NEXUS_ENGINE_API			double	Atan				(double		Y,	float X);
		NEXUS_ENGINE_API			float	Cos					(float		X);
		NEXUS_ENGINE_API			double	Cos					(double		X);
		NEXUS_ENGINE_API			float	Cosh				(float		X);
		NEXUS_ENGINE_API			double	Cosh				(double		X);
		NEXUS_ENGINE_API			float	Sin					(float		X);
		NEXUS_ENGINE_API			double	Sin					(double		X);
		NEXUS_ENGINE_API			float	Sinh				(float		X);
		NEXUS_ENGINE_API			double	Sinh				(double		X);
		NEXUS_ENGINE_API			float	Tan					(float		X);
		NEXUS_ENGINE_API			double	Tan					(double		X);
		NEXUS_ENGINE_API			float	Tanh				(float		X);
		NEXUS_ENGINE_API			double	Tanh				(double		X);

		NEXUS_ENGINE_API			float	Exp					(float		X);
		NEXUS_ENGINE_API			double	Exp					(double		X);
		NEXUS_ENGINE_API			float	Log					(float		X);
		NEXUS_ENGINE_API			double	Log					(double		X);
		NEXUS_ENGINE_API			float	Log10				(float		X);
		NEXUS_ENGINE_API			double	Log10				(double		X);
		NEXUS_ENGINE_API			float	Pow					(float		X, float	Y);
		NEXUS_ENGINE_API			double	Pow					(double		X, float	Y);
		NEXUS_ENGINE_API			int8	Pow					(int8		X, int8		Y);
		NEXUS_ENGINE_API			int16	Pow					(int16		X, int16	Y);
		NEXUS_ENGINE_API			int32	Pow					(int32		X, int32	Y);
		NEXUS_ENGINE_API			int64	Pow					(int64		X, int64	Y);
		NEXUS_ENGINE_API			uint8	Pow					(uint8		X, uint8	Y);
		NEXUS_ENGINE_API			uint16	Pow					(uint16		X, uint16	Y);
		NEXUS_ENGINE_API			uint32	Pow					(uint32		X, uint32	Y);
		NEXUS_ENGINE_API			uint64	Pow					(uint64		X, uint64	Y);
		NEXUS_ENGINE_API			float	Sqrt				(float		X);
		NEXUS_ENGINE_API			double	Sqrt				(double		X);
		NEXUS_ENGINE_API constexpr	float	Square				(float		X);
		NEXUS_ENGINE_API constexpr	double	Square				(double		X);
		NEXUS_ENGINE_API constexpr	int8	Square				(int8		X);
		NEXUS_ENGINE_API constexpr	int16	Square				(int16		X);
		NEXUS_ENGINE_API constexpr	int32	Square				(int32		X);
		NEXUS_ENGINE_API constexpr	int64	Square				(int64		X);
		NEXUS_ENGINE_API constexpr	uint8	Square				(uint8		X);
		NEXUS_ENGINE_API constexpr	uint16	Square				(uint16		X);
		NEXUS_ENGINE_API constexpr	uint32	Square				(uint32		X);
		NEXUS_ENGINE_API constexpr	uint64	Square				(uint64		X);

		NEXUS_ENGINE_API			float	Abs					(float		X);
		NEXUS_ENGINE_API			double	Abs					(double		X);
		NEXUS_ENGINE_API			int8	Abs					(int8		X);
		NEXUS_ENGINE_API			int16	Abs					(int16		X);
		NEXUS_ENGINE_API			int32	Abs					(int32		X);
		NEXUS_ENGINE_API			int64	Abs					(int64		X);
		NEXUS_ENGINE_API			float	Ceil				(float		X);
		NEXUS_ENGINE_API			double	Ceil				(double		X);
		NEXUS_ENGINE_API			int32	CeilToInt			(float		X);
		NEXUS_ENGINE_API			int64	CeilToInt			(double		X);
		NEXUS_ENGINE_API			float	Floor				(float		X);
		NEXUS_ENGINE_API			double	Floor				(double		X);
		NEXUS_ENGINE_API			int32	FloorToInt			(float		X);
		NEXUS_ENGINE_API			int64	FloorToInt			(double		X);
		NEXUS_ENGINE_API			float	Frac				(float		X);
		NEXUS_ENGINE_API			double	Frac				(double		X);
		NEXUS_ENGINE_API			float	Round				(float		X);
		NEXUS_ENGINE_API			double	Round				(double		X);
		NEXUS_ENGINE_API			int32	RoundToInt			(float		X);
		NEXUS_ENGINE_API			int64	RoundToInt			(double		X);
		NEXUS_ENGINE_API			float	Modf				(float		X, float&	Y);
		NEXUS_ENGINE_API			double	Modf				(double		X, double&	Y);

		NEXUS_ENGINE_API			float	FMod				(float		X, float	Y);
		NEXUS_ENGINE_API			double	FMod				(double		X, double	Y);
		NEXUS_ENGINE_API			int8	Modulo				(int8		X, int8		Y);
		NEXUS_ENGINE_API			int16	Modulo				(int16		X, int16	Y);
		NEXUS_ENGINE_API			int32	Modulo				(int32		X, int32	Y);
		NEXUS_ENGINE_API			int64	Modulo				(int64		X, int64	Y);
		NEXUS_ENGINE_API			uint8	Modulo				(uint8		X, uint8	Y);
		NEXUS_ENGINE_API			uint16	Modulo				(uint16		X, uint16	Y);
		NEXUS_ENGINE_API			uint32	Modulo				(uint32		X, uint32	Y);
		NEXUS_ENGINE_API			uint64	Modulo				(uint64		X, uint64	Y);

		NEXUS_ENGINE_API constexpr	bool	IsPowerOfTwo		(uint8		X);
		NEXUS_ENGINE_API constexpr	bool	IsPowerOfTwo		(uint16		X);
		NEXUS_ENGINE_API constexpr	bool	IsPowerOfTwo		(uint32		X);
		NEXUS_ENGINE_API constexpr	bool	IsPowerOfTwo		(uint64		X);
		NEXUS_ENGINE_API constexpr	uint8	NextPowerOfTwo		(uint8		X);
		NEXUS_ENGINE_API constexpr	uint16	NextPowerOfTwo		(uint16		X);
		NEXUS_ENGINE_API constexpr	uint32	NextPowerOfTwo		(uint32		X);
		NEXUS_ENGINE_API constexpr	uint64	NextPowerOfTwo		(uint64		X);
		NEXUS_ENGINE_API			int8	LogTwoPowerOfTwo	(uint8		X);
		NEXUS_ENGINE_API			int16	LogTwoPowerOfTwo	(uint16		X);
		NEXUS_ENGINE_API			int32	LogTwoPowerOfTwo	(uint32		X);
		NEXUS_ENGINE_API			int64	LogTwoPowerOfTwo	(uint64		X);

		NEXUS_ENGINE_API constexpr	bool	IsPrime				(uint8		X);
		NEXUS_ENGINE_API constexpr	bool	IsPrime				(uint16		X);
		NEXUS_ENGINE_API constexpr	bool	IsPrime				(uint32		X);
		NEXUS_ENGINE_API constexpr	bool	IsPrime				(uint64		X);
		NEXUS_ENGINE_API constexpr	uint8	NextPrime			(uint8		X);
		NEXUS_ENGINE_API constexpr	uint16	NextPrime			(uint16		X);
		NEXUS_ENGINE_API constexpr	uint32	NextPrime			(uint32		X);
		NEXUS_ENGINE_API constexpr	uint64	NextPrime			(uint64		X);

		NEXUS_ENGINE_API constexpr	float	InfinityF			();
		NEXUS_ENGINE_API constexpr	double	InfinityD			();
		NEXUS_ENGINE_API constexpr	float	MinF				();
		NEXUS_ENGINE_API constexpr	double	MinD				();
		NEXUS_ENGINE_API constexpr	int8	MinI8				();
		NEXUS_ENGINE_API constexpr	int16	MinI16				();
		NEXUS_ENGINE_API constexpr	int32	MinI32				();
		NEXUS_ENGINE_API constexpr	int64	MinI64				();
		NEXUS_ENGINE_API constexpr	uint8	MinUI8				();
		NEXUS_ENGINE_API constexpr	uint16	MinUI16				();
		NEXUS_ENGINE_API constexpr	uint32	MinUI32				();
		NEXUS_ENGINE_API constexpr	uint64	MinUI64				();
		NEXUS_ENGINE_API constexpr	float	MaxF				();
		NEXUS_ENGINE_API constexpr	double	MaxD				();
		NEXUS_ENGINE_API constexpr	int8	MaxI8				();
		NEXUS_ENGINE_API constexpr	int16	MaxI16				();
		NEXUS_ENGINE_API constexpr	int32	MaxI32				();
		NEXUS_ENGINE_API constexpr	int64	MaxI64				();
		NEXUS_ENGINE_API constexpr	uint8	MaxUI8				();
		NEXUS_ENGINE_API constexpr	uint16	MaxUI16				();
		NEXUS_ENGINE_API constexpr	uint32	MaxUI32				();
		NEXUS_ENGINE_API constexpr	uint64	MaxUI64				();
	}
}
