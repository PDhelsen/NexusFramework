#pragma once

#include "Memory/Allocator/Allocator.h"
#include "Memory/Allocator/AllocatorContext.h"
#include "Memory/Memory.h"
#include "Misc/Templates.h"

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
		Delegate(Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Sbo(true), Comparable(false)
		{
			Clear();
		}

		template<typename F, typename EnableIf<!IsSameType<typename RemoveReference<F>::Type, typename RemoveReference<Delegate>::Type>::Value, bool>::Type E = true>
		Delegate(F&& Func, Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Sbo(true), Comparable(false)
		{
			if constexpr (IsSameType<typename RemoveReference<F>::Type, NullPtr>::Value)
			{
				Clear();
			}
			else
			{
				Bind(Forward<F>(Func));
			}
		}

		template<typename T, typename F, typename EnableIf<!IsSameType<typename RemoveReference<T>::Type, typename RemoveReference<Delegate>::Type>::Value, bool>::Type E = true>
		Delegate(T* Object, F&& Func, Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Sbo(true), Comparable(false)
		{
			Bind(Object, Forward<F>(Func));
		}

		template<typename T, typename EnableIf<!IsSameType<typename RemoveReference<T>::Type, typename RemoveReference<Delegate>::Type>::Value, bool>::Type E = true>
		Delegate(T& Object, Allocator* Allctr = AllocatorContext::Get())
			: Alloc(Allctr), Sbo(true), Comparable(false)
		{
			Bind(Object);
		}

		Delegate(const Delegate<R(Args...)>& Other)
			: Alloc(Other.Alloc), Sbo(Other.Sbo), Comparable(Other.Comparable)
		{
			Clone(Other, true);
		}

		Delegate(Delegate<R(Args...)>&& Other) noexcept
			: Alloc(Other.Alloc), Sbo(Other.Sbo), Comparable(Other.Comparable)
		{
			Clone(Other, false);
			Other.Clear();
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

			Clear();
			Clone(Other, true);

			return *this;
		}

		Delegate<R(Args...)>& operator=(Delegate<R(Args...)>&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

			Clear();
			Clone(Other, false);
			Other.Clear();

			return *this;
		}

		bool operator==(const Delegate<R(Args...)>& Other) const
		{
			if (Sbo != Other.Sbo)
			{
				return false;
			}

			if (!Comparable || !Other.Comparable)
			{
				return false;
			}

			return Memory::MemCompare(GetFunction(), Other.GetFunction(), GetSize(), GetSize());
		}

		bool operator!=(const Delegate<R(Args...)>& Other) const
		{
			return !(*this == Other);
		}

		R operator()(Args... args) const
		{
			return Invoke(args...);
		}

		explicit operator bool() const
		{
			return !IsNull();
		}

		template<typename F>
		void Bind(F&& Func)
		{
			Store(Forward<F>(Func), !IsLambda<RemoveReference<F>::Type>::Value);
		}

		template<typename T>
		void Bind(T& Object)
		{
			Store([&](Args... args) { return Object(args...); }, true);
		}

		template<typename T, typename F>
		void Bind(T* Object, F&& Func)
		{
			Store([Object, Func](Args... args) { return (Object->*Func)(args...); }, true);
		}

		R Invoke(Args... args) const
		{
			return GetFunction()->Invoke(args...);
		}

		void Clear()
		{
			Free();
		}

		bool IsNull() const { return !HasFunction(); }

	private:
		template<typename F>
		void Allocate(F&& Func, bool CanCompare)
		{
			uint64 Size = sizeof(RemoveReference<F>::Type);
			if (Size > SmallFunctionSize)
			{
				AllocatorContext Context(Alloc);
				Data.Large.Function = new Wrapper<F>(Forward<F>(Func));
				Sbo = false;
			}
			else
			{
				Data.Small.Size = Size + 8;
				new (Data.Small.Function) Wrapper<F>(Forward<F>(Func));
				Sbo = true;
			}
			Comparable = CanCompare;
		}

		void Clone(const Delegate<R(Args...)>& Other, bool Allocate)
		{
			if (!Other.Sbo)
			{
				if (Allocate)
				{
					AllocatorContext Context(Alloc);
					Data.Large.Function = Other.GetFunction()->Clone();
				}
				else
				{
					Data.Large.Function = Other.GetFunction();
				}
			}
			else
			{
				Other.GetFunction()->Clone(Data.Small.Function);
				Data.Small.Size = Other.Data.Small.Size;
			}

			Sbo = Other.Sbo;
			Comparable = Other.Comparable;
		}

		void Free()
		{
			if (!Sbo)
			{
				AllocatorContext Context(Alloc);
				delete Data.Large.Function;
			}

			Memory::MemSet(&Data, 0, sizeof(Data));
			Sbo = true;
			Comparable = true;
		}

		template<typename F>
		void Store(F&& Func, bool IsLambda)
		{
			Free();
			Allocate(Forward<F>(Func), IsLambda);
		}

		Interface* GetFunction() const { return (Interface*)(Sbo ? Data.Small.Function : Data.Large.Function); }
		uint64 GetSize() const { return Sbo ? Data.Small.Size : 0; }
		bool HasFunction() const { return Sbo ? Data.Small.Size != 0 : Data.Large.Function != nullptr; }

		inline static const uint8 SmallFunctionSize = 16;
		inline static const uint8 BufferSize = SmallFunctionSize + 8;

		union Storage
		{
			struct
			{
				void* Function;
			} Large;
			struct
			{
				Byte Function[BufferSize];
				uint64 Size;
			} Small;
		};

		Storage Data;
		Allocator* Alloc;
		bool Sbo;
		bool Comparable;
	};
}
