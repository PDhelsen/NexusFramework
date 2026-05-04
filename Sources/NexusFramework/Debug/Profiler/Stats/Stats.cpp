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

	double Stats::Stat::Compute(double Current, double New) const
	{
		switch (Mode)
		{
		case NxFr::Stats::StatMode::Set: return New;
		case NxFr::Stats::StatMode::Cnt: return ++Current;
		case NxFr::Stats::StatMode::Add: return Current + New;
		case NxFr::Stats::StatMode::Avg: return Current + New;
		case NxFr::Stats::StatMode::Min: return Math::Min(Current, New);
		case NxFr::Stats::StatMode::Max: return Math::Max(Current, New);
		}

		return New;
	}

	double Stats::Stat::Finalize(double Current) const
	{
		if (Mode == StatMode::Avg)
		{
			return Current / Tick;
		}

		return Current;
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

#pragma endregion

#pragma region Stats

	Stats::Stats(StringView Path)
		: Headers(), Data(), Stream(Path), Buffer(1024), Initialized(false), Recording(false), Guard()
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

		Data.First().RecordInteger(0);
		Data.Last().RecordLabel("");
	}

	void Stats::Reset()
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

		NEXUS_ASSERT(!(Type == StatType::Label && Mode != StatMode::Set), Default, "Combination not supported");
		NEXUS_ASSERT(!(Type == StatType::Check && Mode != StatMode::Set), Default, "Combination not supported");
		NEXUS_ASSERT(!(Type == StatType::Decimal && Mode == StatMode::Cnt), Default, "Combination not supported");

		Lock LockGuard(Guard);
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

		Lock LockGuard(Guard);

		NEXUS_ASSERT(Headers.TryGet(Id) != nullptr, Default, "Failed to find Id (%s)", Id.C());
		Stat& Statistique = Data[Headers[Id]];
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

		Lock LockGuard(Guard);

		NEXUS_ASSERT(Headers.TryGet(Id) != nullptr, Default, "Failed to find Id (%s)", Id.C());
		Stat& Statistique = Data[Headers[Id]];
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

		Lock LockGuard(Guard);

		NEXUS_ASSERT(Headers.TryGet(Id) != nullptr, Default, "Failed to find Id (%s)", Id.C());
		Stat& Statistique = Data[Headers[Id]];
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

		Lock LockGuard(Guard);

		NEXUS_ASSERT(Headers.TryGet(Id) != nullptr, Default, "Failed to find Id (%s)", Id.C());
		Stat& Statistique = Data[Headers[Id]];
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

		Lock LockGuard(Guard);

		String& Comments = Data.Last().Value.Label;
		Comments += Comment;
		Comments += Separator;
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

	Array<StringId> Stats::GetHeaders() const
	{
		return ContainersUtils::ToArrayKeys(Headers);
	}

	Dictionary<StringId, const Stats::Stat*> Stats::GetStats() const
	{
		Dictionary<StringId, const Stat*> Result(Headers.GetCount());
		for (auto& Header : Headers)
		{
			Result.AppendConstruct(Header.Key, &Data[Header.Value]);
		}
		return Result;
	}

	const Stats::Stat& Stats::GetStat(StringId Id) const
	{
		return Data[Headers[Id]];
	}

#pragma endregion

}
