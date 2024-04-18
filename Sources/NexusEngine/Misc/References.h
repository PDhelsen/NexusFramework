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
}