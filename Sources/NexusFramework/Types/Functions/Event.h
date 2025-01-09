#pragma once

#include "Misc/Templates.h"
#include "Types/Functions/Delegate.h"
#include "Types/Containers/List.h"

namespace NxFr
{
	template<typename Signature>
	class Event;

	template<typename... Args>
	class Event<Args...>
	{
	public:
		using F = Delegate<void(Args...)>;

		Event() = default;
		~Event() = default;

		void operator()(Args... args) const
		{
			return Invoke(args...);
		}

		explicit operator bool() const
		{
			return !IsNull();
		}

		void operator+=(const F& Func)
		{
			Register(Func);
		}

		void operator-=(const F& Func)
		{
			Unregister(Func);
		}

		void Register(const F& Func)
		{
			Functions.Append(Func);
		}

		void Unregister(const F& Func)
		{
			typename List<F>::I Iter = Functions.Find(Func);
			if (Iter == Functions.End())
			{
				return;
			}

			Functions.Remove(Iter.Id());
		}

		template<typename... Args>
		void Invoke(Args... args)
		{
			for (auto& F : Functions)
			{
				F.Invoke(args...);
			}
		}

		void Clear()
		{
			Functions.Clear();
		}

		bool IsNull() const { return Functions.IsEmpty(); }
		uint64 GetCount() const { return Functions.GetCount(); }

	private:
		List<F> Functions;
	};
}
