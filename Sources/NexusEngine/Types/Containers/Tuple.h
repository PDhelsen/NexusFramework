#pragma once

#include "Misc/References.h"

namespace NxEn
{
	template<typename T1, typename T2>
	class Tuple
	{
	public:
		Tuple(const T1& F, const T2& S)
			: First(F), Second(S)
		{
		}

		Tuple(T1&& F, T2&& S)
			: First(Move(F)), Second(Move(S))
		{
		}

		Tuple(const Tuple<T1, T2>& Other)
			: First(Other.F), Second(Other.S)
		{
		}

		Tuple(Tuple<T1, T2>&& Other)
			: First(Other.F), Second(Other.S)
		{
		}

		~Tuple()
		{
			
		}

		Tuple<T1, T2> Copy()
		{
			return Tuple<T1, T2>(First, Second);
		}

		bool operator==(const Tuple<T1, T2>& Other)
		{
			return First == Other.First && Second == Other.Second;
		}

		bool operator!=(const Tuple<T1, T2>& Other)
		{
			return First != Other.First || Second != Other.Second;
		}

		void Assign(const T1& F, const T2& S)
		{
			First = F;
			Second = S;
		}

		void Assign(T1&& F, T2&& S)
		{
			First = Move(F);
			Second = Move(S);
		}

		T1& GetFirst()
		{
			return First;
		}

		const T1& GetFirst() const
		{
			return First;
		}

		T2& GetSecond()
		{
			return Second;
		}

		const T2& GetSecond() const
		{
			return Second;
		}

		void SetFirst(const T1& Value)
		{
			First = Value;
		}

		void SetFirst(T1&& Value)
		{
			First = Move(Value);
		}

		void SetSecond(const T2& Value)
		{
			Second = Value;
		}

		void SetSecond(T2&& Value)
		{
			Second = Move(Value);
		}

		void Swap()
		{
			constexpr bool SameType = IsSameType<T1, T2>;
			NEXUS_ASSERT_STATIC(SameType, "Cannot swap if First and Second are not of the same type");

			T1 Temp = Second;
			Second = First;
			First = Move(Temp);
		}

	private:
		T1 First;
		T2 Second;
	};
}