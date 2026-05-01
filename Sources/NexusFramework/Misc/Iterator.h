#pragma once

#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	namespace Iterator
	{
		template<typename T>
		using Predicate = Delegate<bool(const T&)>;

		template<typename T, typename I>
		struct Iterator
		{
		public:
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

		template<typename T>
		struct IteratorAny
		{
		private:
			struct Interface
			{
				virtual ~Interface() = default;

				virtual void Increment() = 0;
				virtual void Decrement() = 0;
				virtual T& Get() = 0;
				virtual const T& Get() const = 0;
				virtual uint64 Id() const = 0;
				virtual bool Equals(const Interface* Other) const = 0;
				virtual Interface* Clone() const = 0;
			};

			template<typename T, typename I>
			struct Wrapper : public Interface
			{
				Wrapper(I Iterator) : It(Iterator) {}
				Wrapper(const Wrapper<T, I>& Other) : It(Other.It) {}
				~Wrapper() = default;

				void Increment() override { It.Increment(); }
				void Decrement() override { It.Increment(); }
				T& Get() override { return It.Get(); }
				const T& Get() const override { return It.Get(); }
				uint64 Id() const override { return It.Id(); }
				bool Equals(const Interface* Other) const override { return It.Equals(dynamic_cast<const Wrapper<T, I>*>(Other)->It); }
				Interface* Clone() const override { return new Wrapper<T, I>(*this); }

				I It;
			};

		public:
			template<typename I>
			IteratorAny(const I& Iterator, Allocator* Allctr = AllocatorContext::Get())
			{
				It = new Wrapper<T, I>(Iterator);
			}

			IteratorAny(const IteratorAny& Other)
			{
				It = Other.It->Clone();
			}

			~IteratorAny()
			{
				delete It;
			}

			IteratorAny& operator=(const IteratorAny& Other)
			{
				if (this == &Other)
				{
					return;
				}

				delete It;
				It = Other.It->Clone();
			}

			IteratorAny& operator++()
			{
				Increment();
				return *this;
			}

			IteratorAny& operator--()
			{
				Decrement();
				return *this;
			}

			T& operator*()
			{
				return Get();
			}

			const T& operator*() const
			{
				return Get();
			}

			T* operator->()
			{
				return &Get();
			}

			const T* operator->() const
			{
				return &Get();
			}

			bool operator==(const IteratorAny& Other) const
			{
				return Equals(Other);
			}

			bool operator!=(const IteratorAny& Other) const
			{
				return !Equals(Other);
			}

			IteratorAny& Next(uint64 Iteration = 1)
			{
				Iterate(Iteration);
				return *this;
			}

			IteratorAny& Previous(uint64 Iteration = 1)
			{
				Reverse(Iteration);
				return *this;
			}

			void Iterate(uint64 Iteration = 1)
			{
				while (Iteration-- > 0)
				{
					Increment();
				}
			}

			void Reverse(uint64 Iteration = 1)
			{
				while (Iteration-- > 0)
				{
					Decrement();
				}
			}

			void Increment()
			{
				It->Increment();
			}

			void Decrement()
			{
				It->Decrement();
			}

			T& Get()
			{
				return It->Get();
			}

			const T& Get() const
			{
				return It->Get();
			}

			uint64 Id() const
			{
				return It->Id();
			}

			bool Equals(const IteratorAny& Other) const
			{
				return It->Equals(Other.It);
			}

		private:
			Interface* It;
		};
	}
}
