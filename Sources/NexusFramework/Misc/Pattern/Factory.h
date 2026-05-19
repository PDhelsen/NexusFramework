#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Misc/Pattern/Registry.h"
#include "NexusFramework/Misc/Templates.h"

namespace NxFr
{
	template<typename T>
	struct Factory
	{
	public:
		struct Creator
		{
			NX_NOCOPY_NOMOVE(Creator);
			Creator() = default;
			virtual ~Creator() = default;

			virtual T* Create() const = 0;
		};

		template<typename F>
		Creator* Register(StringId Id, bool Replace = false)
		{
			Creator* Instance = Registry.TryGet(Id);
			if (Instance && Replace)
			{
				delete Instance;
			}

			Instance = new Wrapper<F>();
			Registry.Register(Id, Instance, Replace);
			return Instance;
		}
		void Unregister(StringId Id)
		{
			Creator* Instance = Registry.TryGet(Id);
			Registry.Unregister(Id);
			delete Instance;
		}
		void Clear()
		{
			for (auto It = Registry.Begin(); It != Registry.End(); ++It)
			{
				delete It->Value;
			}

			Registry.Clear();
		}

		T* Create(StringId Id)
		{
			const Creator* Instance = TryGet(Id);
			return Instance ? Instance->Create() : nullptr;
		}
		const Creator* TryGet(StringId Id) const
		{
			return Registry.TryGet(Id);
		}

		bool IsEmpty()
		{
			return Registry.IsEmpty();
		}
		uint64 GetCount()
		{
			return Registry.GetCount();
		}

	private:
		template<typename F>
		struct Wrapper : public Creator
		{
			F* Create() const override { return new F(); };
		};

		Registry<Creator*> Registry;
	};
}
