#pragma once

namespace NxEn
{
	template <typename T> 
	struct RemoveReference 
	{ 
		typedef T Type; 
	};

	template <typename T> 
	struct RemoveReference<T&> 
	{ 
		typedef T Type; 
	};

	template <typename T> 
	struct RemoveReference<T&&> 
	{ 
		typedef T Type; 
	};

	template <typename T>
	typename RemoveReference<T>::Type&& Move(T&& Obj)
	{
		typedef typename RemoveReference<T>::Type RawType;
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