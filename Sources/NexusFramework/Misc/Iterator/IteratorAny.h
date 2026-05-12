#pragma once

#include "NexusFramework/Misc/Iterator/Iterator.h"

namespace NxFr
{
	namespace Iterator
	{
		template<typename T>
		struct IteratorAny : public Iterator<T, IteratorAny<T>>
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
			IteratorAny(const I& Iterator, Allocator* Allctr = Allocator::Scope::Get())
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
