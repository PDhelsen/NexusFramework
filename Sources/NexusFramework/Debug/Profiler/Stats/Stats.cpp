#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Debug/Profiler/Stats/Stats.h"

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxFr
{
#pragma region Static

	static const String Separator = ";";

	namespace StatsHeader
	{
		const StringId TickId = "Tick"_Sid;
		const StringId CommentId = "Comments"_Sid;
	}

	Stats* Stats::GetInstance()
	{
		return Globals::Statistiques;
	}

#pragma endregion

#pragma region Stat

	Stats::StatValue::StatValue()
		 : Integer(0)
	{
		Memory::MemSet(this, 0, sizeof(StatValue));
	}
	
	Stats::StatValue::~StatValue()
	{
		Memory::MemSet(this, 0, sizeof(StatValue));
	}

	Stats::Stat::Stat(StatType Type, StatMode Mode)
		: Value(), Type(Type), Mode(Mode), Tick(0)
	{
		switch (Type)
		{
		case NxFr::Stats::StatType::Label: new (&Value.Label) String(32); break;
		case NxFr::Stats::StatType::Check: Value.State = false; break;
		case NxFr::Stats::StatType::Integer: Value.Integer = 0; break;
		case NxFr::Stats::StatType::Decimal: Value.Decimal = 0.0f; break;
		}
	}

	Stats::Stat::~Stat()
	{
		if (Type == StatType::Label)
		{
			Value.Label.~String();
		}
	}

	void Stats::Stat::Reset()
	{
		Tick = 0;
		switch (Type)
		{
		case NxFr::Stats::StatType::Label: Value.Label.Clear(); break;
		case NxFr::Stats::StatType::Check: Value.State = false; break;
		case NxFr::Stats::StatType::Integer: Value.Integer = Mode == StatMode::Min ? Integer::MaxI64 : Mode == StatMode::Max ? Integer::MinI64 : 0; break;
		case NxFr::Stats::StatType::Decimal: Value.Decimal = Mode == StatMode::Min ? Decimal::MaxF : Mode == StatMode::Max ? Decimal::MinF : 0.0f; break;
		}
	}

	void Stats::Stat::RecordLabel(StringView Statistique)
	{
		Value.Label.Clear();
		Value.Label += Statistique;
	}

	void Stats::Stat::RecordCheck(bool Statistique)
	{
		Value.State = Statistique;
	}

	void Stats::Stat::RecordInteger(int64 Statistique)
	{
		Tick++;
		Value.Integer = Compute(Value.Integer, Statistique);
	}

	void Stats::Stat::RecordDecimal(float Statistique)
	{
		Tick++;
		Value.Decimal = Compute(Value.Decimal, Statistique);
	}

	template<>
	struct StringConverter<Stats::Stat>
	{
		static void ToString(const Stats::Stat& Data, String& Result, StringView Format = "")
		{
			switch (Data.GetType())
			{
			case NxFr::Stats::StatType::Label: StringConverter<String>::ToString(Data.GetValue<StringView>(), Result); break;
			case NxFr::Stats::StatType::Check: StringConverter<bool>::ToString(Data.GetValue<bool>(), Result); break;
			case NxFr::Stats::StatType::Integer: StringConverter<int64>::ToString(Data.GetValue<int64>(), Result); break;
			case NxFr::Stats::StatType::Decimal: StringConverter<float>::ToString(Data.GetValue<float>(), Result); break;
			}
		}
	};

#pragma endregion

#pragma region Stats

	Stats::Stats(StringView Path)
		: Headers(), Data(), Stream(Path), Buffer(1024), Initialized(false), Recording(false), Locked(false), Guard()
	{
		Stream.Open(File::Mode::Write);

		RecordHeader(StatsHeader::TickId, StatType::Integer, StatMode::Cnt);
	}

	Stats::~Stats()
	{
		Stream.Close();
	}

	void Stats::Initialize()
	{
		if (Initialized)
		{
			NEXUS_LOG(Warning, Default, "Stats is already initialized");
			return;
		}

		RecordHeader(StatsHeader::CommentId, StatType::Label, StatMode::Set);

		Stream.WriteLine("");
		Stream.Flush();

		Initialized = true;
	}

	void Stats::Flush()
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (Locked)
		{
			NEXUS_LOG(Error, Default, "Stats is locked");
			return;
		}

		if (!Recording)
		{
			return;
		}

		for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
		{
			StringConverter<Stats::Stat>::ToString(Data[Index], Buffer);

			Stream.WriteBlock(Buffer);
			Stream.WriteBlock(Separator);
		}

		Stream.WriteLine("");
		Stream.Flush();

		GetStat(StatsHeader::CommentId).RecordLabel("");
		GetStat(StatsHeader::TickId).RecordInteger(0);
	}

	void Stats::Reset()
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (Locked)
		{
			NEXUS_LOG(Error, Default, "Stats is locked");
			return;
		}

		for (uint64 Index = 0; Index < Data.GetCount(); ++Index)
		{
			Data[Index].Reset();
		}
	}

	void Stats::RecordHeader(StringId Name, StatType Type, StatMode Mode)
	{
		if (Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is already initialized");
			return;
		}

		::NxFr::Lock LockGuard(Guard);

		NEXUS_ASSERT(!(Type == StatType::Label && Mode != StatMode::Set), Default, "Combination not supported");
		NEXUS_ASSERT(!(Type == StatType::Check && Mode != StatMode::Set), Default, "Combination not supported");
		NEXUS_ASSERT(!(Type == StatType::Decimal && Mode == StatMode::Cnt), Default, "Combination not supported");

		Headers.Append(Name, Data.GetCount());
		Data.AppendConstruct(Type, Mode);

		Stream.WriteBlock(Name);
		Stream.WriteBlock(Separator);
	}

	void Stats::RecordStatLabel(StringId Id, StringView Value)
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (!Recording)
		{
			return;
		}

		::NxFr::Lock LockGuard(Guard);

		NEXUS_ASSERT(ContainersUtils::ContainsKey(Headers, Id), Default, "Failed to find Id (%s)", Id.C());
		auto& Statistique = GetStat(Id);
		NEXUS_ASSERT(Statistique.Type == StatType::Label, Default, "Invalid record call");
		Statistique.RecordLabel(Value);
	}

	void Stats::RecordStatCheck(StringId Id, bool Value)
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (!Recording)
		{
			return;
		}

		::NxFr::Lock LockGuard(Guard);

		NEXUS_ASSERT(ContainersUtils::ContainsKey(Headers, Id), Default, "Failed to find Id (%s)", Id.C());
		auto& Statistique = GetStat(Id);
		NEXUS_ASSERT(Statistique.Type == StatType::Check, Default, "Invalid record call");
		Statistique.RecordCheck(Value);
	}

	void Stats::RecordStatInteger(StringId Id, int64 Value)
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (!Recording)
		{
			return;
		}

		::NxFr::Lock LockGuard(Guard);

		NEXUS_ASSERT(ContainersUtils::ContainsKey(Headers, Id), Default, "Failed to find Id (%s)", Id.C());
		auto& Statistique = GetStat(Id);
		NEXUS_ASSERT(Statistique.Type == StatType::Integer, Default, "Invalid record call");
		Statistique.RecordInteger(Value);
	}

	void Stats::RecordStatDecimal(StringId Id, float Value)
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (!Recording)
		{
			return;
		}

		::NxFr::Lock LockGuard(Guard);

		NEXUS_ASSERT(ContainersUtils::ContainsKey(Headers, Id), Default, "Failed to find Id (%s)", Id.C());
		auto& Statistique = GetStat(Id);
		NEXUS_ASSERT(Statistique.Type == StatType::Decimal, Default, "Invalid record call");
		Statistique.RecordDecimal(Value);
	}

	void Stats::RecordComment(StringView Comment)
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (!Recording)
		{
			return;
		}

		::NxFr::Lock LockGuard(Guard);

		String& Comments = GetStat(StatsHeader::CommentId).Value.Label;
		Comments += Comment;
		Comments += Separator;
	}

	void Stats::Lock()
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (Locked)
		{
			NEXUS_LOG(Warning, Default, "Stats is already locked");
			return;
		}

		Locked = true;
	}

	void Stats::Unlock()
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (!Locked)
		{
			NEXUS_LOG(Warning, Default, "Stats is already unlocked");
			return;
		}

		Locked = false;
	}

	void Stats::StartRecording()
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (Recording)
		{
			NEXUS_LOG(Warning, Default, "Stats is already recording");
			return;
		}

		Recording = true;
	}

	void Stats::StopRecording()
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return;
		}

		if (!Recording)
		{
			NEXUS_LOG(Warning, Default, "Stats is not recording");
			return;
		}

		Recording = false;
	}

	Stats::Stat& Stats::GetStat(StringId Id)
	{
		return Data[Headers[Id]];
	}

	const Stats::Stat& Stats::GetStat(StringId Id) const
	{
		return Data[Headers[Id]];
	}

#pragma endregion

}
