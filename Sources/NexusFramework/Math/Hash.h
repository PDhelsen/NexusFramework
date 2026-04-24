#pragma once

#include "NexusFramework/Misc/Hash.h"
#include "NexusFramework/Math/Vector.h"
#include "NexusFramework/Math/Rotation.h"
#include "NexusFramework/Math/Matrix.h"
#include "NexusFramework/Math/Shape.h"

namespace NxFr
{
	namespace Hashing
	{
		template<uint8 D, typename T, typename H>
		class Hasher<Vector<D, T>, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Vector<D, T>& Data)
			{
				for (uint8 I = 0; I < D; ++I)
				{
					State.Accumulate(Data[I]);
				}
			}
		};

		template<typename H>
		class Hasher<AxisAngle, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const AxisAngle& Data)
			{
				State.Accumulate(Data.Axis);
				State.Accumulate(Data.Angle);
			}
		};

		template<typename H>
		class Hasher<Euler, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Euler& Data)
			{
				State.Accumulate(Data.x);
				State.Accumulate(Data.y);
				State.Accumulate(Data.z);
			}
		};

		template<typename H>
		class Hasher<Quaternion, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Quaternion& Data)
			{
				State.Accumulate(Data.w);
				State.Accumulate(Data.x);
				State.Accumulate(Data.y);
				State.Accumulate(Data.z);
			}
		};

		template<uint8 R, uint8 C, typename T, typename H>
		class Hasher<Matrix<R, C, T>, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Matrix<R, C, T>& Data)
			{
				for (uint8 I = 0; I < Matrix<R, C, T>::Count; ++I)
				{
					State.Accumulate(Data[I]);
				}
			}
		};

		template<typename H>
		class Hasher<Ray, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Ray& Data)
			{
				State.Accumulate(Data.Origin);
				State.Accumulate(Data.Direction);
			}
		};

		template<typename H>
		class Hasher<Plane, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Plane& Data)
			{
				State.Accumulate(Data.Normal);
				State.Accumulate(Data.Distance);
			}
		};

		template<typename H>
		class Hasher<Triangle, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Triangle& Data)
			{
				State.Accumulate(Data.A);
				State.Accumulate(Data.B);
				State.Accumulate(Data.C);
			}
		};

		template<typename H>
		class Hasher<Rectangle, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Rectangle& Data)
			{
				State.Accumulate(Data.Center);
				State.Accumulate(Data.Extents);
			}
		};

		template<typename H>
		class Hasher<Circle, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Circle& Data)
			{
				State.Accumulate(Data.Center);
				State.Accumulate(Data.Radius);
			}
		};

		template<typename H>
		class Hasher<Box, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Box& Data)
			{
				State.Accumulate(Data.Center);
				State.Accumulate(Data.Extents);
			}
		};

		template<typename H>
		class Hasher<Cube, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Cube& Data)
			{
				State.Accumulate(Data.Center);
				State.Accumulate(Data.Orientation);
				State.Accumulate(Data.Extents);
			}
		};

		template<typename H>
		class Hasher<Sphere, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const Sphere& Data)
			{
				State.Accumulate(Data.Center);
				State.Accumulate(Data.Radius);
			}
		};
	}
}
