#pragma once

namespace NxFr
{
	using NullPtr = decltype(nullptr);

	template <typename...>
	using HasTrait = void;

	template<typename T, typename U>
	struct IsSameType
	{
		static const bool Value = false;
	};

	template<typename T>
	struct IsSameType<T, T>
	{
		static const bool Value = true;
	};

	template<bool B, typename T = void>
	struct EnableIf
	{
	};

	template<typename T>
	struct EnableIf<true, T>
	{
		using Type = T;
	};

	template<typename T>
	struct IsPointer
	{
		static bool Check()
		{
			return false;
		}
	};

	template<typename T>
	struct IsPointer<T*>
	{
		static bool Check()
		{
			return true;
		}
	};

	template<typename T> 
	struct RemoveReference 
	{ 
		using Type = T;
	};

	template<typename T> 
	struct RemoveReference<T&> 
	{ 
		using Type = T;
	};

	template<typename T> 
	struct RemoveReference<T&&> 
	{ 
		using Type = T;
	};

	template<typename T>
	constexpr typename RemoveReference<T>::Type&& Move(T&& Arg)
	{
		return static_cast<typename RemoveReference<T>::Type&&>(Arg);
	}

	template<typename T>
	constexpr T&& Forward(typename RemoveReference<T>::Type& Arg)
	{
		return static_cast<T&&>(Arg);
	}

	template<typename T>
	constexpr T&& Forward(typename RemoveReference<T>::Type&& Arg)
	{
		return static_cast<T&&>(Arg);
	}

	template <typename T, typename = void>
	struct IsLambda
	{
		static const bool Value = false;
	};

	template <typename T>
	struct IsLambda<T, HasTrait<decltype(&T::operator())>>
	{
		static const bool Value = true;
	};
}
