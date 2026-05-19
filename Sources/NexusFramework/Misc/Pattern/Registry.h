#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	template<typename T>
	struct Registry
	{
	public:
		using P = typename DecayPointer<T>::Type*;
		using I = typename Dictionary<StringId, T>::I;
		
		template<typename U>
		P Register(StringId Id, U&& Value, bool Replace = false)
		{
			T& Instance = !Replace ?
				Instances.Append(Id, Forward<U>(Value)) :
				Instances.AppendOrAssign(Id, Forward<U>(Value));

			if constexpr (IsPointer<T>::Value)
			{
				return Instance;
			}
			else
			{
				return &Instance;
			}
		}
		void Unregister(StringId Id)
		{
			Instances.TryRemove(Id);
		}
		void Clear()
		{
			Instances.Clear();
		}

		P TryGet(StringId Id)
		{
			T* Result = Instances.TryGet(Id);
			if (!Result)
			{
				return nullptr;
			}

			if constexpr (IsPointer<T>::Value)
			{
				return *Result;
			}
			else
			{
				return Result;
			}
		}
		const P TryGet(StringId Id) const
		{
			const T* Result = Instances.TryGet(Id);
			if (!Result)
			{
				return nullptr;
			}

			if constexpr (IsPointer<T>::Value)
			{
				return *Result;
			}
			else
			{
				return Result;
			}
		}

		I Begin()
		{
			return Instances.Begin();
		}
		const I Begin() const
		{
			return Instances.Begin();
		}
		I End()
		{
			return Instances.End();
		}
		const I End() const
		{
			return Instances.End();
		}

		bool IsEmpty() const
		{
			return Instances.IsEmpty();
		}
		uint64 GetCount() const
		{
			return Instances.GetCount();
		}

	private:
		Dictionary<StringId, T> Instances;
	};
}
