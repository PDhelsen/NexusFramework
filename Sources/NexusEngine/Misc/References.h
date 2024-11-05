#pragma once

namespace NxEn
{
	template <typename T> 
	struct RemoveReference 
	{ 
		using Type = T;
	};

	template <typename T> 
	struct RemoveReference<T&> 
	{ 
		using Type = T;
	};

	template <typename T> 
	struct RemoveReference<T&&> 
	{ 
		using Type = T;
	};

	template <typename T>
	typename RemoveReference<T>::Type&& Move(T&& Obj)
	{
		using RawType = typename RemoveReference<T>::Type;
		return (RawType&&)Obj;
	}

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

	template <class, class>
	constexpr bool IsSameType = false;
	template <class T>
	constexpr bool IsSameType<T, T> = true;
}
