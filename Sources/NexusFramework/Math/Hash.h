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
		class HashProcess<Vector<D, T>, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Vector<D, T>& Data)
			{
				for (uint8 I = 0; I < D; ++I)
				{
					HashProcess<T, H>::Accumulate(State, Data[I]);
				}
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Vector<D, T>& Data)
			{
				HashProcess<Vector<D, T>, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Euler, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Euler& Data)
			{
				HashProcess<float, H>::Accumulate(State, Data.x);
				HashProcess<float, H>::Accumulate(State, Data.y);
				HashProcess<float, H>::Accumulate(State, Data.z);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Euler& Data)
			{
				HashProcess<Euler, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<AxisAngle, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const AxisAngle& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Axis);
				HashProcess<float, H>::Accumulate(State, Data.Angle);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const AxisAngle& Data)
			{
				HashProcess<AxisAngle, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Quaternion, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Quaternion& Data)
			{
				HashProcess<float, H>::Accumulate(State, Data.w);
				HashProcess<float, H>::Accumulate(State, Data.x);
				HashProcess<float, H>::Accumulate(State, Data.y);
				HashProcess<float, H>::Accumulate(State, Data.z);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Quaternion& Data)
			{
				HashProcess<Quaternion, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<uint8 R, uint8 C, typename T, typename H>
		class HashProcess<Matrix<R, C, T>, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Matrix<R, C, T>& Data)
			{
				for (uint8 I = 0; I < Matrix<R, C, T>::Count; ++I)
				{
					HashProcess<T, H>::Accumulate(State, Data[I]);
				}
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Matrix<R, C, T>& Data)
			{
				HashProcess<Matrix<R, C, T>, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Ray, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Ray& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Origin);
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Direction);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Ray& Data)
			{
				HashProcess<Ray, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Plane, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Plane& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Normal);
				HashProcess<float, H>::Accumulate(State, Data.Distance);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Plane& Data)
			{
				HashProcess<Plane, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Triangle, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Triangle& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.A);
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.B);
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.C);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Triangle& Data)
			{
				HashProcess<Triangle, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Rectangle, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Rectangle& Data)
			{
				HashProcess<Vector<2, float>, H>::Accumulate(State, Data.Center);
				HashProcess<Vector<2, float>, H>::Accumulate(State, Data.Extents);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Rectangle& Data)
			{
				HashProcess<Rectangle, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Circle, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Circle& Data)
			{
				HashProcess<Vector<2, float>, H>::Accumulate(State, Data.Center);
				HashProcess<float, H>::Accumulate(State, Data.Radius);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Circle& Data)
			{
				HashProcess<Circle, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Box, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Box& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Center);
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Extents);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Box& Data)
			{
				HashProcess<Box, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Cuboid, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Cuboid& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Center);
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Extents);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Cuboid& Data)
			{
				HashProcess<Cuboid, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};

		template<typename H>
		class HashProcess<Sphere, H>
		{
		public:
			static void Accumulate(HashStrategy<H>& State, const Sphere& Data)
			{
				HashProcess<Vector<3, float>, H>::Accumulate(State, Data.Center);
				HashProcess<float, H>::Accumulate(State, Data.Radius);
			}

			static typename H::HashLength Hash(HashStrategy<H>& State, const Sphere& Data)
			{
				HashProcess<Sphere, H>::Accumulate(State, Data);
				return State.Hash();
			}
		};
	}
}
