#pragma once

#include "NexusFramework/Core/NexusFramework.h"

namespace NxTs
{
	enum class DummyEnum : uint8
	{
		Null,
		First,
		Second,
		Third,
		Fourth,
	};

	enum class DummyFlag : uint8
	{
		None = 0,

		First = 1 << 0,
		Second = 1 << 1,
		Third = 1 << 2,
		Fourth = 1 << 3,
	};

	struct Dummy
	{
		static NxFr::String GeneratePath(NxFr::StringView Path = "");

		static NxFr::StringId GetId();
		static NxFr::StringView GetText();
		static NxFr::Byte* GetData();
		static Dummy GetReference();

		static void DummyFunction1(uint64 A);
		static uint64 DummyFunction2(uint64 A);

		Dummy();
		Dummy(uint64 Key);
		Dummy(NxFr::StringView Value);
		Dummy(uint64 Key, NxFr::StringView Value);
		Dummy(const Dummy& Other);
		Dummy(Dummy&& Other) noexcept;
		~Dummy();

		Dummy& operator=(const Dummy& Other);
		Dummy& operator=(Dummy&& Other) noexcept;

		bool operator==(const Dummy& Other) const;
		bool operator!=(const Dummy& Other) const;
		bool operator>(const Dummy& Other) const;
		bool operator<(const Dummy& Other) const;
		bool operator>=(const Dummy& Other) const;
		bool operator<=(const Dummy& Other) const;

		uint64 operator()(uint64 Value);
		uint64 operator()(uint64 Value) const;
		void Function1(uint64 Value);
		void Function1(uint64 Value) const;
		uint64 Function2(uint64 Value);
		uint64 Function2(uint64 Value) const;

		uint64 Key;
		NxFr::String Value;
	};
}

NX_ENUM_STRING(NxTs::DummyEnum, 5, "Null", "First", "Second", "Third", "Fourth")
NX_FLAG(NxTs::DummyFlag, uint8)
NX_FLAG_STRING(NxTs::DummyFlag, 4, "First", "Second", "Third", "Fourth")

namespace NxFr
{
	namespace HashUtility
	{
		template<typename H>
		class Hasher<NxTs::Dummy, H>
		{
		public:
			static void Accumulate(Hash<H>& State, const NxTs::Dummy& Data)
			{
				State.Accumulate(Data.Key);
				State.Accumulate(Data.Value);
			}
		};
	}

	namespace StringUtility
	{
		template<>
		struct Converter<NxTs::Dummy>
		{
			static void ToString(const NxTs::Dummy& Data, String& Result, StringView Format = "")
			{
				StringUtility::Format(Result, "Dummy: %d %s", Data.Key, Data.Value);
			}
		};
	}

	namespace RBSUtility
	{
		template<>
		struct Converter<NxTs::Dummy>
		{
			static NxTs::Dummy Decode(const RBS& Rbs)
			{
				NxTs::Dummy Instance;
				Instance.Key = Rbs.ReadObject<uint64>();
				Instance.Value = Rbs.ReadObject<NxFr::String>();
				return Instance;
			}

			static void Encode(RBS& Rbs, const NxTs::Dummy& Object)
			{
				Rbs.WriteObject(Object.Key);
				Rbs.WriteObject(Object.Value);
			}
		};
	}
}

namespace YAML
{
	template<>
	struct convert<NxTs::Dummy>
	{
		static Node encode(const NxTs::Dummy& rhs);
		static bool decode(const Node& node, NxTs::Dummy& rhs);
	};

	YAML::Emitter& operator<<(YAML::Emitter& out, const NxTs::Dummy& rhs);
}
