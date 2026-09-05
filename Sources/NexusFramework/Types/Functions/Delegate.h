#pragma once

#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Memory/Allocator/Allocator.h"
#include "NexusFramework/Memory/Memory.h"
#include "NexusFramework/Debug/Logger/Log.h"

namespace NxFr
{
	template<typename Signature>
	class Delegate;

	template<typename R, typename... Args>
	class Delegate<R(Args...)>
	{
	public:
		using A = uint64;
		using S = R(*)(void*, Args...);
		using C = void(*)(Allocator*, void*, void*, bool);
		using D = void(*)(Allocator*, void*);
		using FF = R(*)(Args...);
		template <typename T>
		using FM = R(T::*)(Args...);
		template <typename T>
		using FCM = R(T::*)(Args...) const;

		Delegate(Allocator* Allctr = Allocator::TryGet())
			: Allctr(Allctr), Function(nullptr), Copier(nullptr), Destroyer(nullptr)
		{
			Reset();
		}

		Delegate(NullPtr Ptr, Allocator* Allctr = Allocator::TryGet())
			: Allctr(Allctr), Function(nullptr), Copier(nullptr), Destroyer(nullptr)
		{
			Reset();
		}

		Delegate(FF Func, Allocator* Allctr = Allocator::TryGet())
			: Allctr(Allctr), Function(nullptr), Copier(nullptr), Destroyer(nullptr)
		{
			Bind(Func);
		}

		template<typename T>
		Delegate(T* Target, FM<T> Method, Allocator* Allctr = Allocator::TryGet())
			: Allctr(Allctr), Function(nullptr), Copier(nullptr), Destroyer(nullptr)
		{
			Bind(Target, Method);
		}

		template<typename T>
		Delegate(const T* Target, FCM<T> Method, Allocator* Allctr = Allocator::TryGet())
			: Allctr(Allctr), Function(nullptr), Copier(nullptr), Destroyer(nullptr)
		{
			Bind(Target, Method);
		}

		template<typename FC, typename = EnableIf<!IsSameType<typename DecayConst<typename DecayReference<FC>::Type>::Type, Delegate<R(Args...)>>::Value && !IsSameType<typename DecayConst<typename DecayReference<FC>::Type>::Type, NullPtr>::Value>::Type>
		Delegate(FC& Func, Allocator* Allctr = Allocator::TryGet())
			: Allctr(Allctr), Function(nullptr), Copier(nullptr), Destroyer(nullptr)
		{
			Bind(Func);
		}

		template<typename FL, typename = EnableIf<!IsSameType<typename DecayConst<typename DecayReference<FL>::Type>::Type, Delegate<R(Args...)>>::Value && !IsSameType<typename DecayConst<typename DecayReference<FL>::Type>::Type, NullPtr>::Value>::Type>
		Delegate(FL&& Func, Allocator* Allctr = Allocator::TryGet())
			: Allctr(Allctr), Function(nullptr), Copier(nullptr), Destroyer(nullptr)
		{
			Bind(Forward<FL>(Func));
		}

		Delegate(const Delegate<R(Args...)>& Other)
			: Allctr(Other.Allctr), Function(nullptr), Copier(nullptr), Destroyer(nullptr)
		{
			Copy(Other);
		}

		Delegate(Delegate<R(Args...)>&& Other) noexcept
			: Allctr(Other.Allctr), Function(nullptr), Copier(nullptr), Destroyer(nullptr)
		{
			Move(Other);
		}

		~Delegate()
		{
			Reset();
		}

		Delegate<R(Args...)>& operator=(const Delegate<R(Args...)>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			Copy(Other);
			return *this;
		}

		Delegate<R(Args...)>& operator=(Delegate<R(Args...)>&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

			Move(Other);
			return *this;
		}

		explicit operator bool() const
		{
			return !IsNull();
		}

		R operator()(Args... args) const
		{
			return Invoke(Forward<Args>(args)...);
		}

		bool operator==(const Delegate<R(Args...)>& Other) const
		{
			return Equals(Other);
		}

		bool operator!=(const Delegate<R(Args...)>& Other) const
		{
			return !(*this == Other);
		}

		void Bind(FF Func)
		{
			Reset();
			new (Buffer) FFData{ Func };
			Function = [](void* Data, Args... args) -> R
			{
				FFData* Info = static_cast<FFData*>(Data);
				return (*Info->Pointer)(Forward<Args>(args)...);
			};
		}

		template<typename T>
		void Bind(T* Target, FM<T> Method)
		{
			NX_ASSERT_STATIC(sizeof(FMData<T>) <= BufferSize, "Member function too large for Delegate Buffer");

			Reset();
			new (Buffer) FMData<T>{ Target, Method };
			Function = [](void* Data, Args... args) -> R
			{
				FMData<T>* Info = static_cast<FMData<T>*>(Data);
				return (Info->Instance->*(Info->Member))(Forward<Args>(args)...);
			};
		}

		template<typename T>
		void Bind(const T* Target, FCM<T> Method)
		{
			NX_ASSERT_STATIC(sizeof(FCMData<T>) <= BufferSize, "Member function too large for Delegate Buffer");

			Reset();
			new (Buffer) FCMData<T>{ Target, Method };
			Function = [](void* Data, Args... args) -> R
			{
				FCMData<T>* Info = static_cast<FCMData<T>*>(Data);
				return (Info->Instance->*(Info->Member))(Forward<Args>(args)...);
			};
		}

		template<typename FC, typename = EnableIf<!IsSameType<typename DecayConst<typename DecayReference<FC>::Type>::Type, Delegate<R(Args...)>>::Value && !IsSameType<typename DecayConst<typename DecayReference<FC>::Type>::Type, NullPtr>::Value>::Type>
		void Bind(FC& Func)
		{
			Reset();
			new (Buffer) FCData<FC>{ Func };
			Function = [](void* Data, Args... args) -> R
			{
				FCData<FC>* Info = static_cast<FCData<FC>*>(Data);
				return (Info->Pointer)(Forward<Args>(args)...);
			};
		}

		template<typename FL, typename = EnableIf<!IsSameType<typename DecayConst<typename DecayReference<FL>::Type>::Type, Delegate<R(Args...)>>::Value && !IsSameType<typename DecayConst<typename DecayReference<FL>::Type>::Type, NullPtr>::Value>::Type>
		void Bind(FL&& Func)
		{
#pragma warning(push)
#pragma warning(disable : 4172)
			using Lambda = typename DecayReference<FL>::Type;

			Reset();
			if constexpr (sizeof(Lambda) > BufferSize)
			{
				Lambda* Info = Memory::Create<Lambda>(Allctr, Forward<FL>(Func));
				new (Buffer) FLData{ Info };
				Function = [](void* Data, Args... args) -> R
				{
					Lambda* Info = static_cast<Lambda*>(static_cast<FLData*>(Data)->Pointer);
					return (*Info)(Forward<Args>(args)...);
				};
				Copier = [](Allocator* Allctr, void* Data, void* Instance, bool Steal)
				{
					FLData* Info = static_cast<FLData*>(Data);
					if (Steal)
					{
						new (Instance) FLData{ Info->Pointer };
						Info->Pointer = nullptr;
					}
					else
					{
						Lambda* Copy = Memory::Create<Lambda>(Allctr, *static_cast<Lambda*>(Info->Pointer));
						new (Instance) FLData{ Copy };
					}
				};
				Destroyer = [](Allocator* Allctr, void* Data)
				{
					Lambda* Info = static_cast<Lambda*>(static_cast<FLData*>(Data)->Pointer);
					Memory::Destroy<Lambda>(Info, Allctr);
				};
			}
			else
			{
				new (Buffer) Lambda(Forward<FL>(Func));
				Function = [](void* Data, Args... args) -> R
				{
					auto* Info = static_cast<Lambda*>(Data);
					return (*Info)(Forward<Args>(args)...);
				};
				Copier = [](Allocator* Allctr, void* Other, void* Instance, bool Steal)
				{
					const Lambda* Info = static_cast<const Lambda*>(Other);
					new (Instance) Lambda(*Info);
				};
				Destroyer = [](Allocator* Allctr, void* Data)
				{
					Lambda* Info = static_cast<Lambda*>(Data);
					Info->~Lambda();
				};
			}
#pragma warning(pop)
		}

		R Invoke(Args... args) const
		{
			NX_ASSERT(!IsNull(), Default, "Delegate is null");
			return Function(Buffer, Forward<Args>(args)...);
		}

		bool Equals(const Delegate<R(Args...)>& Other) const
		{
			if (IsComplex() || Other.IsComplex())
			{
				return false;
			}

			return Function == Other.Function && Memory::MemCompare(Buffer, Other.Buffer, BufferSize);
		}

		bool IsNull() const
		{
			return Function == nullptr;
		}

	private:
		struct FFData
		{
			FF Pointer;
		};
		template<typename T>
		struct FMData
		{
			T* Instance;
			FM<T> Member;
		};
		template<typename T>
		struct FCMData
		{
			const T* Instance;
			FCM<T> Member;
		};
		template<typename FC>
		struct FCData
		{
			FC& Pointer;
		};
		struct FLData
		{
			void* Pointer;
		};

		static constexpr uint64 BufferSize = 32;

		void Reset()
		{
			if (IsComplex())
			{
				Destroyer(Allctr, Buffer);
			}

			Function = nullptr;
			Copier = nullptr;
			Destroyer = nullptr;

			Memory::MemSet(Buffer, 0, BufferSize);
		}

		void Copy(const Delegate<R(Args...)>& Other)
		{
			Reset();

			Function = Other.Function;
			Copier = Other.Copier;
			Destroyer = Other.Destroyer;

			if (Other.IsComplex())
			{
				Other.Copier(Allctr, Other.Buffer, Buffer, false);
			}
			else
			{
				Memory::MemCopy(Other.Buffer, Buffer, BufferSize);
			}
		}

		void Move(Delegate<R(Args...)>& Other)
		{
			Reset();

			Function = Other.Function;
			Copier = Other.Copier;
			Destroyer = Other.Destroyer;

			if (Other.IsComplex())
			{
				Other.Copier(Allctr, Other.Buffer, Buffer, Allctr == Other.Allctr);
			}
			else
			{
				Memory::MemCopy(Other.Buffer, Buffer, BufferSize);
			}

			Other.Function = nullptr;
			Other.Copier = nullptr;
			Other.Destroyer = nullptr;
		}

		bool IsComplex() const
		{
			return Copier != nullptr || Destroyer != nullptr;
		}

		mutable alignas(Memory::DefaultAlignement) Byte Buffer[BufferSize];
		Allocator* Allctr;
		S Function;
		C Copier;
		D Destroyer;
	};
}
