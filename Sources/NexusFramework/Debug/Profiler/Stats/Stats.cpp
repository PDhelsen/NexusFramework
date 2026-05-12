#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Debug/Profiler/Stats/Stats.h"
#include "NexusFramework/Debug/Profiler/Stats/Misc/String.h"

#include "NexusFramework/External/StandardLibrary.h"

namespace NxFr
{
#pragma region Static

	namespace StatsHeader
	{
		const StringId TickId = "Tick"_Sid;
		const StringId CommentId = "Comments"_Sid;
	}

#pragma endregion

#pragma region Stat

	Stats::Value::Value()
		 : Integer(0)
	{
		Memory::MemSet(this, 0, sizeof(Value));
	}
	
	Stats::Value::~Value()
	{
		Memory::MemSet(this, 0, sizeof(Value));
	}

	Stats::Stat::Stat(Type StatType, Mode StatMode)
		: StatValue(), StatType(StatType), StatMode(StatMode), StatTick(0)
	{
		switch (StatType)
		{
		case NxFr::Stats::Type::Label: new (&StatValue.Label) String(32); break;
		case NxFr::Stats::Type::Check: StatValue.State = false; break;
		case NxFr::Stats::Type::Integer: StatValue.Integer = 0; break;
		case NxFr::Stats::Type::Decimal: StatValue.Decimal = 0.0f; break;
		}
	}

	Stats::Stat::~Stat()
	{
		if (StatType == Type::Label)
		{
			StatValue.Label.~String();
		}
	}

	void Stats::Stat::Reset()
	{
		StatTick = 0;
		switch (StatType)
		{
		case NxFr::Stats::Type::Label: StatValue.Label.Clear(); break;
		case NxFr::Stats::Type::Check: StatValue.State = false; break;
		case NxFr::Stats::Type::Integer: StatValue.Integer = StatMode == Mode::Min ? Integer::MaxI64 : StatMode == Mode::Max ? Integer::MinI64 : 0; break;
		case NxFr::Stats::Type::Decimal: StatValue.Decimal = StatMode == Mode::Min ? Decimal::MaxF : StatMode == Mode::Max ? Decimal::MinF : 0.0f; break;
		}
	}

	double Stats::Stat::Compute(double Current, double New) const
	{
		switch (StatMode)
		{
		case NxFr::Stats::Mode::Set: return New;
		case NxFr::Stats::Mode::Cnt: return ++Current;
		case NxFr::Stats::Mode::Add: return Current + New;
		case NxFr::Stats::Mode::Avg: return Current + New;
		case NxFr::Stats::Mode::Min: return Math::Min(Current, New);
		case NxFr::Stats::Mode::Max: return Math::Max(Current, New);
		}

		return New;
	}

	double Stats::Stat::Finalize(double Current) const
	{
		if (StatMode == Mode::Avg)
		{
			return Current / StatTick;
		}

		return Current;
	}

	void Stats::Stat::RecordLabel(StringView Statistique)
	{
		StatValue.Label.Clear();
		StatValue.Label += Statistique;
		StatTick++;
	}

	void Stats::Stat::RecordCheck(bool Statistique)
	{
		StatValue.State = Statistique;
		StatTick++;
	}

	void Stats::Stat::RecordInteger(int64 Statistique)
	{
		StatValue.Integer = Compute(StatValue.Integer, Statistique);
		StatTick++;
	}

	void Stats::Stat::RecordDecimal(float Statistique)
	{
		StatValue.Decimal = Compute(StatValue.Decimal, Statistique);
		StatTick++;
	}

#pragma endregion

#pragma region Stats

	Stats::Stats(StringView Path)
		: Headers(), Data(), Stream(Path), Buffer(1024), Initialized(false), Recording(false), Guard()
	{
		Stream.Open(File::Mode::Write);

		RecordHeader(StatsHeader::TickId, Type::Integer, Mode::Cnt);
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

		RecordHeader(StatsHeader::CommentId, Type::Label, Mode::Set);

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
			StringUtility::ToString(Data[Index], Buffer);

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

	void Stats::RecordHeader(StringId Name, Type StatType, Mode StatMode)
	{
		if (Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is already initialized");
			return;
		}

		NEXUS_ASSERT(!(StatType == Type::Label && StatMode != Mode::Set), Default, "Combination not supported");
		NEXUS_ASSERT(!(StatType == Type::Check && StatMode != Mode::Set), Default, "Combination not supported");
		NEXUS_ASSERT(!(StatType == Type::Decimal && StatMode == Mode::Cnt), Default, "Combination not supported");

		Lock LockGuard(Guard);
		Headers.Append(Name, Data.GetCount());
		Data.AppendConstruct(StatType, StatMode);

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
		NEXUS_ASSERT(Statistique.StatType == Type::Label, Default, "Invalid record call");
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
		NEXUS_ASSERT(Statistique.StatType == Type::Check, Default, "Invalid record call");
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
		NEXUS_ASSERT(Statistique.StatType == Type::Integer, Default, "Invalid record call");
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
		NEXUS_ASSERT(Statistique.StatType == Type::Decimal, Default, "Invalid record call");
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

		String& Comments = Data.Last().StatValue.Label;
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
		return ContainerUtility::ToArrayKeys(Headers);
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
