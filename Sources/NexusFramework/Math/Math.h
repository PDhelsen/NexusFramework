#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"

namespace NxFr
{
	namespace Math
	{
		NEXUS_FRAMEWORK_API	bool	Equals				(float		X, float	Y);
		NEXUS_FRAMEWORK_API	bool	Equals				(float		X, float	Y, float Epsilon);
		NEXUS_FRAMEWORK_API	bool	Equals				(double		X, double	Y);
		NEXUS_FRAMEWORK_API	bool	Equals				(double		X, double	Y, double Epsilon);

		NEXUS_FRAMEWORK_API	float	Lerp				(float		A, float	B, float T);
		NEXUS_FRAMEWORK_API	double	Lerp				(double		A, double	B, double T);
		NEXUS_FRAMEWORK_API	int8	Lerp				(int8		A, int8		B, float T);
		NEXUS_FRAMEWORK_API	int16	Lerp				(int16		A, int16	B, float T);
		NEXUS_FRAMEWORK_API	int32	Lerp				(int32		A, int32	B, float T);
		NEXUS_FRAMEWORK_API	int64	Lerp				(int64		A, int64	B, float T);
		NEXUS_FRAMEWORK_API	uint8	Lerp				(uint8		A, uint8	B, float T);
		NEXUS_FRAMEWORK_API	uint16	Lerp				(uint16		A, uint16	B, float T);
		NEXUS_FRAMEWORK_API	uint32	Lerp				(uint32		A, uint32	B, float T);
		NEXUS_FRAMEWORK_API	uint64	Lerp				(uint64		A, uint64	B, float T);

		NEXUS_FRAMEWORK_API	float	Min					(float		X, float	Y);
		NEXUS_FRAMEWORK_API	double	Min					(double		X, double	Y);
		NEXUS_FRAMEWORK_API	int8	Min					(int8		X, int8		Y);
		NEXUS_FRAMEWORK_API	int16	Min					(int16		X, int16	Y);
		NEXUS_FRAMEWORK_API	int32	Min					(int32		X, int32	Y);
		NEXUS_FRAMEWORK_API	int64	Min					(int64		X, int64	Y);
		NEXUS_FRAMEWORK_API	uint8	Min					(uint8		X, uint8	Y);
		NEXUS_FRAMEWORK_API	uint16	Min					(uint16		X, uint16	Y);
		NEXUS_FRAMEWORK_API	uint32	Min					(uint32		X, uint32	Y);
		NEXUS_FRAMEWORK_API	uint64	Min					(uint64		X, uint64	Y);
		NEXUS_FRAMEWORK_API	float	Max					(float		X, float	Y);
		NEXUS_FRAMEWORK_API	double	Max					(double		X, double	Y);
		NEXUS_FRAMEWORK_API	int8	Max					(int8		X, int8		Y);
		NEXUS_FRAMEWORK_API	int16	Max					(int16		X, int16	Y);
		NEXUS_FRAMEWORK_API	int32	Max					(int32		X, int32	Y);
		NEXUS_FRAMEWORK_API	int64	Max					(int64		X, int64	Y);
		NEXUS_FRAMEWORK_API	uint8	Max					(uint8		X, uint8	Y);
		NEXUS_FRAMEWORK_API	uint16	Max					(uint16		X, uint16	Y);
		NEXUS_FRAMEWORK_API	uint32	Max					(uint32		X, uint32	Y);
		NEXUS_FRAMEWORK_API	uint64	Max					(uint64		X, uint64	Y);
		NEXUS_FRAMEWORK_API	float	Clamp				(float		X, float	A = 0, float	B = 1);
		NEXUS_FRAMEWORK_API	double	Clamp				(double		X, double	A = 0, double	B = 1);
		NEXUS_FRAMEWORK_API	int8	Clamp				(int8		X, int8		A = 0, int8		B = 1);
		NEXUS_FRAMEWORK_API	int16	Clamp				(int16		X, int16	A = 0, int16	B = 1);
		NEXUS_FRAMEWORK_API	int32	Clamp				(int32		X, int32	A = 0, int32	B = 1);
		NEXUS_FRAMEWORK_API	int64	Clamp				(int64		X, int64	A = 0, int64	B = 1);
		NEXUS_FRAMEWORK_API	uint8	Clamp				(uint8		X, uint8	A = 0, uint8	B = 1);
		NEXUS_FRAMEWORK_API	uint16	Clamp				(uint16		X, uint16	A = 0, uint16	B = 1);
		NEXUS_FRAMEWORK_API	uint32	Clamp				(uint32		X, uint32	A = 0, uint32	B = 1);
		NEXUS_FRAMEWORK_API	uint64	Clamp				(uint64		X, uint64	A = 0, uint64	B = 1);

		NEXUS_FRAMEWORK_API	float	ToDegree			(float		X);
		NEXUS_FRAMEWORK_API	double	ToDegree			(double		X);
		NEXUS_FRAMEWORK_API	float	ToRadians			(float		X);
		NEXUS_FRAMEWORK_API	double	ToRadians			(double		X);

		NEXUS_FRAMEWORK_API	float	Acos				(float		X);
		NEXUS_FRAMEWORK_API	double	Acos				(double		X);
		NEXUS_FRAMEWORK_API	float	Asin				(float		X);
		NEXUS_FRAMEWORK_API	double	Asin				(double		X);
		NEXUS_FRAMEWORK_API	float	Atan				(float		X);
		NEXUS_FRAMEWORK_API	double	Atan				(double		X);
		NEXUS_FRAMEWORK_API	float	Atan				(float		Y,	float X);
		NEXUS_FRAMEWORK_API	double	Atan				(double		Y,	float X);
		NEXUS_FRAMEWORK_API	float	Cos					(float		X);
		NEXUS_FRAMEWORK_API	double	Cos					(double		X);
		NEXUS_FRAMEWORK_API	float	Cosh				(float		X);
		NEXUS_FRAMEWORK_API	double	Cosh				(double		X);
		NEXUS_FRAMEWORK_API	float	Sin					(float		X);
		NEXUS_FRAMEWORK_API	double	Sin					(double		X);
		NEXUS_FRAMEWORK_API	float	Sinh				(float		X);
		NEXUS_FRAMEWORK_API	double	Sinh				(double		X);
		NEXUS_FRAMEWORK_API	float	Tan					(float		X);
		NEXUS_FRAMEWORK_API	double	Tan					(double		X);
		NEXUS_FRAMEWORK_API	float	Tanh				(float		X);
		NEXUS_FRAMEWORK_API	double	Tanh				(double		X);

		NEXUS_FRAMEWORK_API	float	Exp					(float		X);
		NEXUS_FRAMEWORK_API	double	Exp					(double		X);
		NEXUS_FRAMEWORK_API	float	Log					(float		X);
		NEXUS_FRAMEWORK_API	double	Log					(double		X);
		NEXUS_FRAMEWORK_API	float	Log10				(float		X);
		NEXUS_FRAMEWORK_API	double	Log10				(double		X);
		NEXUS_FRAMEWORK_API	float	Pow					(float		X, float	Y);
		NEXUS_FRAMEWORK_API	double	Pow					(double		X, float	Y);
		NEXUS_FRAMEWORK_API	int8	Pow					(int8		X, int8		Y);
		NEXUS_FRAMEWORK_API	int16	Pow					(int16		X, int16	Y);
		NEXUS_FRAMEWORK_API	int32	Pow					(int32		X, int32	Y);
		NEXUS_FRAMEWORK_API	int64	Pow					(int64		X, int64	Y);
		NEXUS_FRAMEWORK_API	uint8	Pow					(uint8		X, uint8	Y);
		NEXUS_FRAMEWORK_API	uint16	Pow					(uint16		X, uint16	Y);
		NEXUS_FRAMEWORK_API	uint32	Pow					(uint32		X, uint32	Y);
		NEXUS_FRAMEWORK_API	uint64	Pow					(uint64		X, uint64	Y);
		NEXUS_FRAMEWORK_API	float	Sqrt				(float		X);
		NEXUS_FRAMEWORK_API	double	Sqrt				(double		X);
		NEXUS_FRAMEWORK_API float	Square				(float		X);
		NEXUS_FRAMEWORK_API double	Square				(double		X);
		NEXUS_FRAMEWORK_API int8	Square				(int8		X);
		NEXUS_FRAMEWORK_API int16	Square				(int16		X);
		NEXUS_FRAMEWORK_API int32	Square				(int32		X);
		NEXUS_FRAMEWORK_API int64	Square				(int64		X);
		NEXUS_FRAMEWORK_API uint8	Square				(uint8		X);
		NEXUS_FRAMEWORK_API uint16	Square				(uint16		X);
		NEXUS_FRAMEWORK_API uint32	Square				(uint32		X);
		NEXUS_FRAMEWORK_API uint64	Square				(uint64		X);

		NEXUS_FRAMEWORK_API	float	Abs					(float		X);
		NEXUS_FRAMEWORK_API	double	Abs					(double		X);
		NEXUS_FRAMEWORK_API	int8	Abs					(int8		X);
		NEXUS_FRAMEWORK_API	int16	Abs					(int16		X);
		NEXUS_FRAMEWORK_API	int32	Abs					(int32		X);
		NEXUS_FRAMEWORK_API	int64	Abs					(int64		X);
		NEXUS_FRAMEWORK_API	float	Ceil				(float		X);
		NEXUS_FRAMEWORK_API	double	Ceil				(double		X);
		NEXUS_FRAMEWORK_API	int32	CeilToInt			(float		X);
		NEXUS_FRAMEWORK_API	int64	CeilToInt			(double		X);
		NEXUS_FRAMEWORK_API	float	Floor				(float		X);
		NEXUS_FRAMEWORK_API	double	Floor				(double		X);
		NEXUS_FRAMEWORK_API	int32	FloorToInt			(float		X);
		NEXUS_FRAMEWORK_API	int64	FloorToInt			(double		X);
		NEXUS_FRAMEWORK_API	float	Frac				(float		X);
		NEXUS_FRAMEWORK_API	double	Frac				(double		X);
		NEXUS_FRAMEWORK_API	float	Round				(float		X);
		NEXUS_FRAMEWORK_API	double	Round				(double		X);
		NEXUS_FRAMEWORK_API	int32	RoundToInt			(float		X);
		NEXUS_FRAMEWORK_API	int64	RoundToInt			(double		X);
		NEXUS_FRAMEWORK_API	float	Modf				(float		X, float&	Y);
		NEXUS_FRAMEWORK_API	double	Modf				(double		X, double&	Y);

		NEXUS_FRAMEWORK_API	float	FMod				(float		X, float	Y);
		NEXUS_FRAMEWORK_API	double	FMod				(double		X, double	Y);
		NEXUS_FRAMEWORK_API	int8	Modulo				(int8		X, int8		Y);
		NEXUS_FRAMEWORK_API	int16	Modulo				(int16		X, int16	Y);
		NEXUS_FRAMEWORK_API	int32	Modulo				(int32		X, int32	Y);
		NEXUS_FRAMEWORK_API	int64	Modulo				(int64		X, int64	Y);
		NEXUS_FRAMEWORK_API	uint8	Modulo				(uint8		X, uint8	Y);
		NEXUS_FRAMEWORK_API	uint16	Modulo				(uint16		X, uint16	Y);
		NEXUS_FRAMEWORK_API	uint32	Modulo				(uint32		X, uint32	Y);
		NEXUS_FRAMEWORK_API	uint64	Modulo				(uint64		X, uint64	Y);
		NEXUS_FRAMEWORK_API	int8	IsMultiple			(int8		X, int8		Y);
		NEXUS_FRAMEWORK_API	int16	IsMultiple			(int16		X, int16	Y);
		NEXUS_FRAMEWORK_API	int32	IsMultiple			(int32		X, int32	Y);
		NEXUS_FRAMEWORK_API	int64	IsMultiple			(int64		X, int64	Y);
		NEXUS_FRAMEWORK_API	uint8	IsMultiple			(uint8		X, uint8	Y);
		NEXUS_FRAMEWORK_API	uint16	IsMultiple			(uint16		X, uint16	Y);
		NEXUS_FRAMEWORK_API	uint32	IsMultiple			(uint32		X, uint32	Y);
		NEXUS_FRAMEWORK_API	uint64	IsMultiple			(uint64		X, uint64	Y);

		NEXUS_FRAMEWORK_API bool	IsPowerOfTwo		(uint8		X);
		NEXUS_FRAMEWORK_API bool	IsPowerOfTwo		(uint16		X);
		NEXUS_FRAMEWORK_API bool	IsPowerOfTwo		(uint32		X);
		NEXUS_FRAMEWORK_API bool	IsPowerOfTwo		(uint64		X);
		NEXUS_FRAMEWORK_API uint8	NextPowerOfTwo		(uint8		X);
		NEXUS_FRAMEWORK_API uint16	NextPowerOfTwo		(uint16		X);
		NEXUS_FRAMEWORK_API uint32	NextPowerOfTwo		(uint32		X);
		NEXUS_FRAMEWORK_API uint64	NextPowerOfTwo		(uint64		X);
		NEXUS_FRAMEWORK_API	int8	LogTwoPowerOfTwo	(uint8		X);
		NEXUS_FRAMEWORK_API	int16	LogTwoPowerOfTwo	(uint16		X);
		NEXUS_FRAMEWORK_API	int32	LogTwoPowerOfTwo	(uint32		X);
		NEXUS_FRAMEWORK_API	int64	LogTwoPowerOfTwo	(uint64		X);

		NEXUS_FRAMEWORK_API bool	IsPrime				(uint8		X);
		NEXUS_FRAMEWORK_API bool	IsPrime				(uint16		X);
		NEXUS_FRAMEWORK_API bool	IsPrime				(uint32		X);
		NEXUS_FRAMEWORK_API bool	IsPrime				(uint64		X);
		NEXUS_FRAMEWORK_API uint8	NextPrime			(uint8		X);
		NEXUS_FRAMEWORK_API uint16	NextPrime			(uint16		X);
		NEXUS_FRAMEWORK_API uint32	NextPrime			(uint32		X);
		NEXUS_FRAMEWORK_API uint64	NextPrime			(uint64		X);
	}
}
