#pragma once

#include "NexusFramework/Types/Numeric/Integer.h"

namespace NxFr
{
	namespace Iterator
	{
		template<typename T, typename I>
		struct Iterator
		{
		public:
			T* operator->()
			{
				return &Cast().Get();
			}

			const T* operator->() const
			{
				return &Cast().Get();
			}

			bool operator==(const I& Other) const
			{
				return Cast().Equals(Other);
			}

			bool operator!=(const I& Other) const
			{
				return !Cast().Equals(Other);
			}

			I& operator++()
			{
				Cast().Increment();
				return Cast();
			}

			I operator++(int32)
			{
				I Temp = Cast();
				Cast().Increment();
				return Temp;
			}

			I& operator--()
			{
				Cast().Decrement();
				return Cast();
			}

			I operator--(int32)
			{
				I Temp = Cast();
				Cast().Decrement();
				return Temp;
			}

			T& operator*()
			{
				return Cast().Get();
			}

			const T& operator*() const
			{
				return Cast().Get();
			}

			I& Next(uint64 Iteration = 1)
			{
				Iterate(Iteration);
				return Cast();
			}

			I& Previous(uint64 Iteration = 1)
			{
				Reverse(Iteration);
				return Cast();
			}

			void Iterate(uint64 Iteration = 1)
			{
				while (Iteration-- > 0)
				{
					Cast().Increment();
				}
			}

			void Reverse(uint64 Iteration = 1)
			{
				while (Iteration-- > 0)
				{
					Cast().Decrement();
				}
			}

			// virtual void Increment() = 0;
			// virtual void Decrement() = 0;
			// virtual T& Get() = 0;
			// virtual const T& Get() const = 0;
			// virtual uint64 Id() const = 0;
			// virtual bool Equals(const I& Other) const = 0;

		private:
			I& Cast() { return static_cast<I&>(*this); }
			const I& Cast() const { return static_cast<const I&>(*this); }
		};
	}
}
