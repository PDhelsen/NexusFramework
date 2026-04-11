#pragma once

#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/Types/Functions/Delegate.h"
#include "NexusFramework/Types/Containers/List.h"

namespace NxFr
{
	template<typename... Args>
	class Event
	{
	public:
		using F = Delegate<void(Args...)>;

		Event(Allocator* Allctr = AllocatorContext::Get())
			: Functions(4, Allctr)
		{
		}

		~Event()
		{
		}

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
			uint64 Index = 0;
			for (; Index < Functions.GetCount(); ++Index)
			{
				if (Functions[Index] == Func)
				{
					break;
				}
			}

			if (Index < Functions.GetCount())
			{
				Functions.Remove(Index);
			}
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
