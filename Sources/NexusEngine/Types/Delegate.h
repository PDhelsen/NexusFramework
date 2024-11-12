#pragma once

#include "Misc/References.h"
#include "Memory/Allocator/Allocator.h"
#include "Memory/Memory.h"

namespace NxEn
{
	template<typename Signature>
	class Delegate;

	template<typename R, typename... Args>
	class Delegate<R(Args...)>
	{
	private:
		class Interface
		{
		public:
			virtual ~Interface() = default;
			virtual R Invoke(Args... args) = 0;
			virtual Interface* Clone(void* Pointer = nullptr) const = 0;
		};

		template<typename F>
		class Wrapper : public Interface
		{
		public:
			Wrapper(F&& Func)
				: Target(Move(Func))
			{
			}

			R Invoke(Args... args) override
			{
				return Target(args...);
			}

			Interface* Clone(void* Pointer) const override
			{
				if (Pointer)
				{
					return new (Pointer) Wrapper(*this);
				}
				else
				{
					return new Wrapper(*this);
				}
			}

		private:
			Wrapper(const Wrapper& Other)
				: Target(Other.Target)
			{
			}

			F Target;
		};

	public:
		Delegate()
			: Allctr(nullptr)
		{
			Function.Large = nullptr;
		}

		template<typename F>
		explicit Delegate(F&& Func)
			: Allctr(nullptr)
		{
			Bind(Forward<F>(Func));
		}

		template<typename T, typename F>
		explicit Delegate(T* Object, F&& Func)
			: Allctr(nullptr)
		{
			Bind(Object, Forward<F>(Func));
		}

		template<typename T>
		explicit Delegate(T& Object)
			: Allctr(nullptr)
		{
			Bind(Object);
		}

		Delegate(const Delegate<R(Args...)>& Other)
			: Allctr(Other.Allctr)
		{
			Clone(Other, true);
		}

		Delegate(Delegate<R(Args...)>&& Other) noexcept
			: Allctr(Other.Allctr)
		{
			Clone(Other, false);

			Other.Allctr = nullptr;
			Other.Function.Large = nullptr;
		}

		~Delegate()
		{
			Free();
		}

		Delegate<R(Args...)>& operator=(const Delegate<R(Args...)>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			Free();
			Clone(Other, true);
			Allctr = Other.Allctr;

			return *this;
		}

		Delegate<R(Args...)>& operator=(Delegate<R(Args...)>&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

			Free();
			Clone(Other, false);
			Allctr = Other.Allctr;

			Other.Allctr = nullptr;
			Other.Function.Large = nullptr;

			return *this;
		}

		R operator()(Args... args) const
		{
			return Invoke(args...);
		}

		template<typename T, typename F>
		void Bind(T* Object, F&& Func)
		{
			Bind([Object, Func](Args... args) { return (Object->*Func)(args...); });
		}

		template<typename T>
		void Bind(T& Object)
		{
			Bind([&](Args... args) { return Object(args...); });
		}

		template<typename F>
		void Bind(F&& Func)
		{
			Free();
			Allocate(Forward<F>(Func));
		}

		R Invoke(Args... args) const
		{
			return GetFunction()->Invoke(args...);
		}

		void Clear()
		{
			Free();
		}

		bool IsNull() const { return Function.Large == nullptr; }

	private:
		template<typename F>
		void Allocate(F&& Func)
		{
			uint64 Size = sizeof(RemoveReference<F>::Type);
			if (Size > SmallFunctionSize)
			{
				Allctr = Memory::GetActiveAllocator();
				Function.Large = new Wrapper<F>(Forward<F>(Func));
			}
			else
			{
				new (Function.Small) Wrapper<F>(Forward<F>(Func));
			}
		}

		void Clone(const Delegate<R(Args...)>& Other, bool Allocate)
		{
			if (Other.Allctr)
			{
				if (Allocate)
				{
					AllocatorActive Active(Allctr);
					Function.Large = Other.GetFunction()->Clone();
				}
				else
				{
					Function.Large = Other.GetFunction();
				}
			}
			else
			{
				Other.GetFunction()->Clone(Function.Small);
			}
		}

		void Free()
		{
			if (Allctr)
			{
				AllocatorActive Active(Allctr);
				delete Function.Large;
			}

			Function.Large = nullptr;
			Allctr = nullptr;
		}

		Interface* GetFunction() const { return (Interface*)(Allctr ? Function.Large : Function.Small); }

		static const uint8 SmallFunctionSize = 24;

		union Storage
		{
			void* Large;
			Byte Small[SmallFunctionSize];
		};

		Allocator* Allctr;
		Storage Function;
	};
}
