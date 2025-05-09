#include "NexusFramework/Core/NexusFrameworkPch.h"
#include "NexusFramework/Debug/Profiler/Stats/Stats.h"

#include "NexusFramework/External/StandardLibrary.h"
#include "NexusFramework/Core/NexusFrameworkGlobals.h"

namespace NxFr
{
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

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------
	// StatValue
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------

	Stats::StatValue::StatValue()
		: UnsignedInteger(0)
	{
		Memory::MemSet(this, 0, sizeof(StatValue));
	}

	Stats::StatValue::~StatValue()
	{
		Memory::MemSet(this, 0, sizeof(StatValue));
	}

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Stat
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------

	Stats::Stat::Stat(StatType Type, StatMode Mode)
		: Type(Type), Mode(Mode), Tick(0)
	{
		switch (Type)
		{
		case NxFr::Stats::StatType::Label: new (&Value.Label) String(32); break;
		case NxFr::Stats::StatType::Check: Value.State = false; break;
		case NxFr::Stats::StatType::Integer: Value.Integer = 0; break;
		case NxFr::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = 0; break;
		case NxFr::Stats::StatType::Decimal: Value.Decimal = 0.0f; break;
		case NxFr::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = 0.0; break;
		}

		Reset();
	}

	Stats::Stat::Stat(const Stat& Other)
	{
		Type = Other.Type;
		Mode = Other.Mode;
		Tick = Other.Tick;

		if (Type == StatType::Label)
		{
			new (&Value.Label) String();
		}

		switch (Type)
		{
		case NxFr::Stats::StatType::Label: Value.Label = Other.Value.Label; break;
		case NxFr::Stats::StatType::Check: Value.State = Other.Value.State; break;
		case NxFr::Stats::StatType::Integer: Value.Integer = Other.Value.Integer; break;
		case NxFr::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Other.Value.UnsignedInteger; break;
		case NxFr::Stats::StatType::Decimal: Value.Decimal = Other.Value.Decimal; break;
		case NxFr::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = Other.Value.DecimalPrecise; break;
		}
	}

	Stats::Stat::Stat(Stat&& Other) noexcept
	{
		Type = Other.Type;
		Mode = Other.Mode;
		Tick = Other.Tick;

		if (Type == StatType::Label)
		{
			new (&Value.Label) String();
		}

		switch (Type)
		{
		case NxFr::Stats::StatType::Label: Value.Label = Move(Other.Value.Label); break;
		case NxFr::Stats::StatType::Check: Value.State = Move(Other.Value.State); break;
		case NxFr::Stats::StatType::Integer: Value.Integer = Move(Other.Value.Integer); break;
		case NxFr::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Move(Other.Value.UnsignedInteger); break;
		case NxFr::Stats::StatType::Decimal: Value.Decimal = Move(Other.Value.Decimal); break;
		case NxFr::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = Move(Other.Value.DecimalPrecise); break;
		}
	}

	Stats::Stat::~Stat()
	{
		if (Type == StatType::Label)
		{
			Value.Label.~String();
		}
	}

	Stats::Stat& Stats::Stat::operator=(const Stat& Other)
	{
		Type = Other.Type;
		Mode = Other.Mode;
		Tick = Other.Tick;

		switch (Type)
		{
		case NxFr::Stats::StatType::Label: Value.Label = Other.Value.Label; break;
		case NxFr::Stats::StatType::Check: Value.State = Other.Value.State; break;
		case NxFr::Stats::StatType::Integer: Value.Integer = Other.Value.Integer; break;
		case NxFr::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Other.Value.UnsignedInteger; break;
		case NxFr::Stats::StatType::Decimal: Value.Decimal = Other.Value.Decimal; break;
		case NxFr::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = Other.Value.DecimalPrecise; break;
		}

		return *this;
	}

	Stats::Stat& Stats::Stat::operator=(Stat&& Other) noexcept
	{
		Type = Other.Type;
		Mode = Other.Mode;
		Tick = Other.Tick;

		switch (Type)
		{
		case NxFr::Stats::StatType::Label: Value.Label = Move(Other.Value.Label); break;
		case NxFr::Stats::StatType::Check: Value.State = Move(Other.Value.State); break;
		case NxFr::Stats::StatType::Integer: Value.Integer = Move(Other.Value.Integer); break;
		case NxFr::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Move(Other.Value.UnsignedInteger); break;
		case NxFr::Stats::StatType::Decimal: Value.Decimal = Move(Other.Value.Decimal); break;
		case NxFr::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = Move(Other.Value.DecimalPrecise); break;
		}

		return *this;
	}

	void Stats::Stat::Reset()
	{
		Tick = 0;

		switch (Type)
		{
		case NxFr::Stats::StatType::Label: Value.Label.Clear(); break;
		case NxFr::Stats::StatType::Check: Value.State = false; break;
		case NxFr::Stats::StatType::Integer: Value.Integer = Mode == StatMode::Min ? Integer::MaxI64 : Mode == StatMode::Max ? Integer::MinI64 : 0; break;
		case NxFr::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Mode == StatMode::Min ? Integer::MaxUI64 : Mode == StatMode::Max ? Integer::MinUI64 : 0u; break;
		case NxFr::Stats::StatType::Decimal: Value.Decimal = Mode == StatMode::Min ? Decimal::MaxF : Mode == StatMode::Max ? Decimal::MinF : 0.0f; break;
		case NxFr::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = Mode == StatMode::Min ? Decimal::MaxD : Mode == StatMode::Max ? Decimal::MinD : 0.0; break;
		}
	}

	void Stats::Stat::RecordLabel(StringView Statistique)
	{
		Tick++;
		Value.Label.Clear();
		Value.Label += Statistique;
	}

	void Stats::Stat::RecordCheck(bool Statistique)
	{
		Tick++;
		Value.State = Statistique;
	}

	void Stats::Stat::RecordInteger(int64 Statistique)
	{
		Value.Integer = Compute(Value.Integer, Statistique);
		Tick++;
	}

	void Stats::Stat::RecordUnsignedInteger(uint64 Statistique)
	{
		Tick++;
		Value.UnsignedInteger = Compute(Value.UnsignedInteger, Statistique);
	}

	void Stats::Stat::RecordDecimal(float Statistique)
	{
		Tick++;
		Value.Decimal = Compute(Value.Decimal, Statistique);
	}

	void Stats::Stat::RecordDecimalPrecision(double Statistique)
	{
		Tick++;
		Value.DecimalPrecise = Compute(Value.DecimalPrecise, Statistique);
	}

	const String& Stats::Stat::ToString(String& PreAllocated) const
	{
		switch (Type)
		{
		case NxFr::Stats::StatType::Label: return GetValue<const String&>();
		case NxFr::Stats::StatType::Check: StringUtility::Format(PreAllocated, StringView("%s"), GetValue<bool>() ? "X" : ""); return PreAllocated;
		case NxFr::Stats::StatType::Integer: StringUtility::Format(PreAllocated, StringView("%d"), GetValue<int64>()); return PreAllocated;
		case NxFr::Stats::StatType::UnsignedInteger: StringUtility::Format(PreAllocated, StringView("%d"), GetValue<uint64>()); return PreAllocated;
		case NxFr::Stats::StatType::Decimal: StringUtility::Format(PreAllocated, StringView("%.2f"), GetValue<float>()); return PreAllocated;
		case NxFr::Stats::StatType::DecimalPrecision: StringUtility::Format(PreAllocated, StringView("%.2f"), GetValue<double>()); return PreAllocated;
		}

		return PreAllocated;
	}

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Stats
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------

	Stats::Stats(StringView Path)
		: Headers(), Data(), Handle(Path), BufferLine(1024), BufferCell(), Initialized(false), Recording(false), Locked(false)
	{
		Handle.Delete();
		Handle.Create();
		Handle.Open(File::Mode::Append);

		RecordHeader(StatsHeader::TickId, StatType::UnsignedInteger, StatMode::Cnt);
	}

	Stats::~Stats()
	{
		Handle.Close();
	}

	void Stats::Initialize()
	{
		if (Initialized)
		{
			NEXUS_LOG(Warning, Default, "Stats is already initialized");
			return;
		}

		RecordHeader(StatsHeader::CommentId, StatType::Label, StatMode::Set);

		WriteLine();

		GetStat(StatsHeader::CommentId).RecordLabel("");
		GetStat(StatsHeader::TickId).RecordUnsignedInteger(0);

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

		for (auto& Statistique : Data)
		{
			const String& Text = Statistique.ToString(BufferCell);

			BufferLine += Text;
			BufferLine += Separator;

			BufferCell.Clear();
		}

		WriteLine();

		GetStat(StatsHeader::CommentId).RecordLabel("");
		GetStat(StatsHeader::TickId).RecordUnsignedInteger(0);
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

		for (auto& Statistique : Data)
		{
			Statistique.Reset();
		}

		GetStat(StatsHeader::TickId).RecordUnsignedInteger(0);
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
		NEXUS_ASSERT(!(Type == StatType::DecimalPrecision && Mode == StatMode::Cnt), Default, "Combination not supported");

		Headers.Append(Name, Data.GetCount());
		Data.Append(Stat(Type, Mode));

		BufferLine += Name.ToString();
		BufferLine += Separator;
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

		NEXUS_ASSERT(Headers.ContainsKey(Id), Default, "Failed to find Id (%s)", Id.C());
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

		NEXUS_ASSERT(Headers.ContainsKey(Id), Default, "Failed to find Id (%s)", Id.C());
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

		NEXUS_ASSERT(Headers.ContainsKey(Id), Default, "Failed to find Id (%s)", Id.C());
		auto& Statistique = GetStat(Id);
		NEXUS_ASSERT(Statistique.Type == StatType::Integer, Default, "Invalid record call");
		Statistique.RecordInteger(Value);
	}

	void Stats::RecordStatUnsignedInteger(StringId Id, uint64 Value)
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

		NEXUS_ASSERT(Headers.ContainsKey(Id), Default, "Failed to find Id (%s)", Id.C());
		auto& Statistique = GetStat(Id);
		NEXUS_ASSERT(Statistique.Type == StatType::UnsignedInteger, Default, "Invalid record call");
		Statistique.RecordUnsignedInteger(Value);
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

		NEXUS_ASSERT(Headers.ContainsKey(Id), Default, "Failed to find Id (%s)", Id.C());
		auto& Statistique = GetStat(Id);
		NEXUS_ASSERT(Statistique.Type == StatType::Decimal, Default, "Invalid record call");
		Statistique.RecordDecimal(Value);
	}

	void Stats::RecordStatDecimalPrecision(StringId Id, double Value)
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

		NEXUS_ASSERT(Headers.ContainsKey(Id), Default, "Failed to find Id (%s)", Id.C());
		auto& Statistique = GetStat(Id);
		NEXUS_ASSERT(Statistique.Type == StatType::DecimalPrecision, Default, "Invalid record call");
		Statistique.RecordDecimalPrecision(Value);
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

	const Stats::Stat* Stats::GetCurrentStat(StringId Id) const
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return nullptr;
		}

		return &GetStat(Id);
	}

	Dictionary<StringId, const Stats::Stat*> Stats::GetAllCurrentStats() const
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return Dictionary<StringId, const Stats::Stat*>();
		}

		if (Locked)
		{
			return Dictionary<StringId, const Stats::Stat*>();
		}

		Dictionary<StringId, const Stats::Stat*> Result(Data.GetCount());
		for (auto& [Header, Index] : Headers)
		{
			Result.Append(Header, &Data[Index]);
		}

		return Result;
	}

	void Stats::WriteLine()
	{
		BufferLine += StringUtility::NewLine;
		Handle.WriteText(BufferLine);
		BufferLine.Clear();
	}

	Stats::Stat& Stats::GetStat(StringId Id)
	{
		return Data[Headers[Id]];
	}

	const Stats::Stat& Stats::GetStat(StringId Id) const
	{
		return Data[Headers[Id]];
	}
}
