#pragma once

#define NX_NOCOPY(T) \
	T(const T&) = delete;\
	T& operator=(const T&) = delete;
#define NX_NOMOVE(T) \
	T(T&&) noexcept = delete;\
	T& operator=(T&&) noexcept = delete;
#define NX_NOCOPY_NOMOVE(T)\
	NX_NOCOPY(T)\
	NX_NOMOVE(T)

namespace NxFr
{
#pragma region Traits

	template <typename...>
	using HasTrait = void;

	template<bool B, typename T = void>
	struct EnableIf
	{
	};

	template<typename T>
	struct EnableIf<true, T>
	{
		using Type = T;
	};

#pragma endregion

#pragma region Type & Inheritance

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
		static uint8   Check(Base*);
		static uint32  Check(...);

	public:
		static constexpr bool Value = sizeof(Check(static_cast<Derived*>(nullptr))) == sizeof(char);
	};

	template<typename T, typename = void>
	struct HasSimilar
	{
		const static bool Value = false;
	};

	template<typename T>
	struct HasSimilar<T, decltype(void(typename T::Similar{}))>
	{
		const static bool Value = true;
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

#pragma endregion

#pragma region Pointer & Reference

	using NullPtr = decltype(nullptr);

	template<typename T>
	struct IsPointer
	{
		static const bool Value = false;
	};

	template<typename T>
	struct IsPointer<T*>
	{
		static const bool Value = true;
	};

	template<typename T>
	struct Decay
	{
		using Type = T;
	};

	template<typename T>
	struct Decay<const T>
	{
		using Type = T;
	};

	template<typename T>
	struct Decay<T*>
	{
		using Type = T;
	};

	template<typename T>
	struct Decay<const T*>
	{
		using Type = T;
	};

	template<typename T>
	struct Decay<T&>
	{
		using Type = T;
	};

	template<typename T>
	struct Decay<const T&>
	{
		using Type = T;
	};

	template<typename T>
	struct Decay<T&&>
	{
		using Type = T;
	};

	template<typename T>
	struct Decay<const T&&>
	{
		using Type = T;
	};

	template<typename T>
	struct DecayPointer
	{
		using Type = T;
	};

	template<typename T>
	struct DecayPointer<T*>
	{
		using Type = T;
	};

	template<typename T> 
	struct DecayReference 
	{ 
		using Type = T;
	};

	template<typename T> 
	struct DecayReference<T&> 
	{ 
		using Type = T;
	};

	template<typename T> 
	struct DecayReference<T&&> 
	{ 
		using Type = T;
	};

	template<typename T>
	struct DecayConst
	{
		using Type = T;
	};

	template<typename T>
	struct DecayConst<const T>
	{
		using Type = T;
	};

	template<typename T>
	constexpr typename DecayReference<T>::Type&& Move(T&& Arg)
	{
		return static_cast<typename DecayReference<T>::Type&&>(Arg);
	}

	template<typename T>
	constexpr T&& Forward(typename DecayReference<T>::Type& Arg)
	{
		return static_cast<T&&>(Arg);
	}

	template<typename T>
	constexpr T&& Forward(typename DecayReference<T>::Type&& Arg)
	{
		return static_cast<T&&>(Arg);
	}

#pragma endregion

#pragma region Sequence

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

#pragma endregion

#pragma region Misc

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

#pragma endregion

}
