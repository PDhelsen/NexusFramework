#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Containers/Stack.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	template<typename T>
	struct Context
	{
	public:
		struct Reference
		{
		public:
			NX_NOCOPY_NOMOVE(Reference)
			Reference(Context* Instances, T* Instance)
				: Instances(Instances)
			{
				Instances->Push(Instance);
			}
			~Reference()
			{
				Instances->Pop();
			}

		private:
			Context* Instances;
		};

		struct Value
		{
		public:
			NX_NOCOPY_NOMOVE(Value)
			template<typename... Args>
			Value(Context* Instances, Args&&... args)
				: Instances(Instances), Data(Forward<Args>(args)...)
			{
				Instances->Push(&Data);
			}
			~Value()
			{
				Instances->Pop();
			}

			T* operator->()
			{
				return &Data;
			}
			const T* operator->() const
			{
				return &Data;
			}

		private:
			Context* Instances;
			T Data;
		};

		Context()
			: Contexts()
		{
		}

		Reference PushReference(T* Instance)
		{
			return Reference(this, Instance);
		}

		template<typename... Args>
		Value PushValue(Args&&... args)
		{
			return Value(this, Forward<Args>(args)...);
		}

		void Push(T* Instance)
		{
			Contexts.Append(Instance);
		}
		void Pop()
		{
			if (Contexts.IsEmpty())
			{
				return;
			}

			Contexts.Remove();
		}
		void Clear()
		{
			Contexts.Clear();
		}

		T* TryGet()
		{
			if (Contexts.IsEmpty())
			{
				return nullptr;
			}

			return Contexts.Get();
		}
		const T* TryGet() const
		{
			if (Contexts.IsEmpty())
			{
				return nullptr;
			}

			return Contexts.Get();
		}

		bool IsEmpty() const
		{
			return Contexts.IsEmpty();
		}
		uint64 GetCount() const
		{
			return Contexts.GetCount();
		}

	private:
		Stack<T*> Contexts;
	};
}
