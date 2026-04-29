#pragma once

#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	template<typename T1, typename T2>
	class Tuple
	{
		friend class ContainersUtils;

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
			: First(Other.First), Second(Other.Second)
		{
		}

		Tuple(Tuple<T1, T2>&& Other) noexcept
			: First(Move(Other.First)), Second(Move(Other.Second))
		{
		}

		~Tuple()
		{
			
		}

		Tuple<T1, T2>& operator=(const Tuple<T1, T2>& Other)
		{
			if (this == &Other)
			{
				return *this;
			}

			First = Other.First;
			Second = Other.Second;

			return *this;
		}

		Tuple<T1, T2>& operator=(Tuple<T1, T2>&& Other) noexcept
		{
			if (this == &Other)
			{
				return *this;
			}

			First = Move(Other.First);
			Second = Move(Other.Second);

			return *this;
		}

		bool operator==(const Tuple<T1, T2>& Other) const
		{
			return First == Other.First && Second == Other.Second;
		}

		bool operator!=(const Tuple<T1, T2>& Other) const
		{
			return !(*this == Other);
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

		template<typename... Args>
		void SetFirstConstruct(Args&&... args)
		{
			Memory::Destruct(&First);
			Memory::Construct(&First, args...);
		}

		void SetSecond(const T2& Value)
		{
			Second = Value;
		}

		void SetSecond(T2&& Value)
		{
			Second = Move(Value);
		}

		template<typename... Args>
		void SetSecondConstruct(Args&&... args)
		{
			Memory::Destruct(&Second);
			Memory::Construct(&Second, args...);
		}

	private:
		T1 First;
		T2 Second;
	};
}
