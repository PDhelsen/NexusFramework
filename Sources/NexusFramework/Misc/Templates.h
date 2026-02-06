#pragma once

#include "NexusFramework/Memory/Handle/Handle.h"

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

	template<typename Derived, typename Base>
	class InheritFrom
	{
	private:
		static uint8   Test(Base*);
		static uint32  Test(...);

	public:
		static constexpr bool Value = sizeof(Test(static_cast<Derived*>(nullptr))) == sizeof(char);
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

	template<typename T, typename = void>
	struct HasSimilar
	{
		constexpr static bool Value = false;
	};

	template<typename T>
	struct HasSimilar<T, decltype(void(typename T::Similar{}))>
	{
		constexpr static bool Value = true;
	};

	template<typename T, bool = HasSimilar<T>::Value>
	struct SimilarOf
	{
		using Type = T;
	};

	template <typename T>
	struct SimilarOf<T, true>
	{
		using Type = typename T::Similar;
	};

	template<typename T>
	struct IsHandle
	{
		using Type = T;
		static constexpr bool Value = false;
	};

	template<typename T>
	struct IsHandle<NxFr::Handle<T>>
	{
		using Type = T;
		static constexpr bool Value = true;
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
	struct RemovePointer
	{
		using Type = T;
	};

	template<typename T>
	struct RemovePointer<T*>
	{
		using Type = T;
	};

	template<typename T>
	struct RemovePointer<Handle<T>>
	{
		using Type = T;
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

	template<uint64... Indices>
	struct IndexSequence
	{
	};

	template<uint64 N, uint64... Indices>
	struct MakeIndexSequenceHelper : MakeIndexSequenceHelper<N - 1, N - 1, Indices...>
	{
	};

	template<uint64... Indices>
	struct MakeIndexSequenceHelper<0, Indices...>
	{
		using Type = IndexSequence<Indices...>;
	};

	template<uint64 N>
	using MakeIndexSequence = typename MakeIndexSequenceHelper<N>::Type;
}
