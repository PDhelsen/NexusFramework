#include "Core/NexusEnginePch.h"
#include "Stats.h"

#include "Core/NexusEngineGlobals.h"

namespace NxEn
{
	static const String Separator = ";";
	static const StringId TickId = "Tick"_Sid;
	static const StringId CommentId = "Comments"_Sid;

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
		: Type(Type), Mode(Mode)
	{
		switch (Type)
		{
		case NxEn::Stats::StatType::Label: new (&Value.Label) String(32); break;
		case NxEn::Stats::StatType::Check: Value.State = false; break;
		case NxEn::Stats::StatType::Integer: Value.Integer = 0; break;
		case NxEn::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = 0; break;
		case NxEn::Stats::StatType::Decimal: Value.Decimal = 0.0f; break;
		case NxEn::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = 0.0; break;
		}

		Reset();
	}

	Stats::Stat::Stat(const Stat& Other)
	{
		Type = Other.Type;
		Mode = Other.Mode;

		if (Type == StatType::Label)
		{
			new (&Value.Label) String();
		}

		switch (Type)
		{
		case NxEn::Stats::StatType::Label: Value.Label = Other.Value.Label; break;
		case NxEn::Stats::StatType::Check: Value.State = Other.Value.State; break;
		case NxEn::Stats::StatType::Integer: Value.Integer = Other.Value.Integer; break;
		case NxEn::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Other.Value.UnsignedInteger; break;
		case NxEn::Stats::StatType::Decimal: Value.Decimal = Other.Value.Decimal; break;
		case NxEn::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = Other.Value.DecimalPrecise; break;
		}
	}

	Stats::Stat::Stat(Stat&& Other) noexcept
	{
		Type = Other.Type;
		Mode = Other.Mode;

		if (Type == StatType::Label)
		{
			new (&Value.Label) String();
		}

		switch (Type)
		{
		case NxEn::Stats::StatType::Label: Value.Label = Move(Other.Value.Label); break;
		case NxEn::Stats::StatType::Check: Value.State = Move(Other.Value.State); break;
		case NxEn::Stats::StatType::Integer: Value.Integer = Move(Other.Value.Integer); break;
		case NxEn::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Move(Other.Value.UnsignedInteger); break;
		case NxEn::Stats::StatType::Decimal: Value.Decimal = Move(Other.Value.Decimal); break;
		case NxEn::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = Move(Other.Value.DecimalPrecise); break;
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

		switch (Type)
		{
		case NxEn::Stats::StatType::Label: Value.Label = Other.Value.Label; break;
		case NxEn::Stats::StatType::Check: Value.State = Other.Value.State; break;
		case NxEn::Stats::StatType::Integer: Value.Integer = Other.Value.Integer; break;
		case NxEn::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Other.Value.UnsignedInteger; break;
		case NxEn::Stats::StatType::Decimal: Value.Decimal = Other.Value.Decimal; break;
		case NxEn::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = Other.Value.DecimalPrecise; break;
		}

		return *this;
	}

	Stats::Stat& Stats::Stat::operator=(Stat&& Other) noexcept
	{
		Type = Other.Type;
		Mode = Other.Mode;

		switch (Type)
		{
		case NxEn::Stats::StatType::Label: Value.Label = Move(Other.Value.Label); break;
		case NxEn::Stats::StatType::Check: Value.State = Move(Other.Value.State); break;
		case NxEn::Stats::StatType::Integer: Value.Integer = Move(Other.Value.Integer); break;
		case NxEn::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Move(Other.Value.UnsignedInteger); break;
		case NxEn::Stats::StatType::Decimal: Value.Decimal = Move(Other.Value.Decimal); break;
		case NxEn::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = Move(Other.Value.DecimalPrecise); break;
		}

		return *this;
	}

	void Stats::Stat::Reset()
	{
		switch (Type)
		{
		case NxEn::Stats::StatType::Label: Value.Label.Clear(); break;
		case NxEn::Stats::StatType::Check: Value.State = false; break;
		case NxEn::Stats::StatType::Integer: Value.Integer = Mode == StatMode::Min ? Integer::MaxI64() : Mode == StatMode::Max ? Integer::MinI64() : 0; break;
		case NxEn::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Mode == StatMode::Min ? Integer::MaxUI64() : Mode == StatMode::Max ? Integer::MinUI64() : 0u; break;
		case NxEn::Stats::StatType::Decimal: Value.Decimal = Mode == StatMode::Min ? Decimal::MaxF() : Mode == StatMode::Max ? Decimal::MinF() : 0.0f; break;
		case NxEn::Stats::StatType::DecimalPrecision: Value.DecimalPrecise = Mode == StatMode::Min ? Decimal::MaxD() : Mode == StatMode::Max ? Decimal::MinD() : 0.0; break;
		}
	}

	void Stats::Stat::RecordLabel(StringView Statistique)
	{
		Value.Label = Statistique.ToString();
	}

	void Stats::Stat::RecordCheck(bool Statistique)
	{
		Value.State = Statistique;
	}

	void Stats::Stat::RecordInteger(int64 Statistique)
	{
		Value.Integer = Compute(Value.Integer, Statistique);
	}

	void Stats::Stat::RecordUnsignedInteger(uint64 Statistique)
	{
		Value.UnsignedInteger = Compute(Value.UnsignedInteger, Statistique);
	}

	void Stats::Stat::RecordDecimal(float Statistique)
	{
		Value.Decimal = Compute(Value.Decimal, Statistique);
	}

	void Stats::Stat::RecordDecimalPrecision(double Statistique)
	{
		Value.DecimalPrecise = Compute(Value.DecimalPrecise, Statistique);
	}

	void Stats::Stat::RecordCount()
	{
		switch (Type)
		{
		case NxEn::Stats::StatType::Integer: Value.Integer = Compute(Value.Integer, int64(0)); break;
		case NxEn::Stats::StatType::UnsignedInteger: Value.UnsignedInteger = Compute(Value.UnsignedInteger, uint64(0)); break;
		}
	}

	const String& Stats::Stat::ToString(String& PreAllocated, double Span) const
	{
		switch (Type)
		{
		case NxEn::Stats::StatType::Label: return Value.Label;
		case NxEn::Stats::StatType::Check: StringUtility::Format(PreAllocated, StringView("%s"), Value.State ? "X" : ""); return PreAllocated;
		case NxEn::Stats::StatType::Integer: StringUtility::Format(PreAllocated, StringView("%d"), Finalize(Value.Integer, Span)); return PreAllocated;
		case NxEn::Stats::StatType::UnsignedInteger: StringUtility::Format(PreAllocated, StringView("%d"), Finalize(Value.UnsignedInteger, Span)); return PreAllocated;
		case NxEn::Stats::StatType::Decimal: StringUtility::Format(PreAllocated, StringView("%.2f"), Finalize(Value.Decimal, Span)); return PreAllocated;
		case NxEn::Stats::StatType::DecimalPrecision: StringUtility::Format(PreAllocated, StringView("%.2f"), Finalize(Value.DecimalPrecise, Span)); return PreAllocated;
		}

		return PreAllocated;
	}

	// -------------------------------------------------------------------------------------------------------------------------------------------------------------
	// Stats
	// -------------------------------------------------------------------------------------------------------------------------------------------------------------

	Stats::Stats(StringView Path)
		: Headers(), Data(), Handle(Path), Line(1024), Cell(), Span(0.0), Initialized(false), Recording(false), Locked(false)
	{
		Handle.Delete();
		Handle.Create();
		Handle.Open(File::Mode::Append);

		RecordHeader(TickId, StatType::UnsignedInteger, StatMode::Cnt);
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

		RecordHeader(CommentId, StatType::Label, StatMode::Set);

		WriteLine();

		GetStat(CommentId).Reset();
		GetStat(TickId).RecordCount();
		Span = 1.0;

		Initialized = true;
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
			const String& Text = Statistique.ToString(Cell, Span);

			Line += Text;
			Line += Separator;

			Cell.Clear();
		}

		WriteLine();

		GetStat(CommentId).Reset();
		GetStat(TickId).RecordCount();
		Span++;
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

		// Skip Tick and Comments
		for (uint64 Index = 1; Index < Data.GetCount() - 1; ++Index)
		{
			Data[Index].Reset();
		}

		Span = 1.0;
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
		NEXUS_ASSERT(!(Mode == StatMode::Cnt && Type != StatType::Integer && Type != StatType::UnsignedInteger), Default, "Combination not supported");

		Headers.Append(Name, Data.GetCount());
		Data.Append(Stat(Type, Mode));

		Line += Name.ToString();
		Line += Separator;
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
		NEXUS_ASSERT(Statistique.Type == StatType::Integer && Statistique.Mode != StatMode::Cnt, Default, "Invalid record call");
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
		NEXUS_ASSERT(Statistique.Type == StatType::UnsignedInteger && Statistique.Mode != StatMode::Cnt, Default, "Invalid record call");
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

	void Stats::RecordStatCount(StringId Id)
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
		NEXUS_ASSERT((Statistique.Type == StatType::Integer || Statistique.Type == StatType::UnsignedInteger) && Statistique.Mode == StatMode::Cnt, Default, "Invalid record call");
		Statistique.RecordCount();
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

		String& Comments = GetStat(CommentId).Value.Label;
		Comments += Comment;
		Comments += Separator;
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

	const Stats::Stat* Stats::GetCurrentStat(StringId Id) const
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return nullptr;
		}

		return &GetStat(Id);
	}

	uint64 Stats::GetCurrentTick() const
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return 0;
		}

		return GetStat(TickId).Value.UnsignedInteger;
	}

	StringView Stats::GetCurrentComment() const
	{
		if (!Initialized)
		{
			NEXUS_LOG(Error, Default, "Stats is not initialized");
			return StringUtility::Empty;
		}

		return GetStat(CommentId).Value.Label;
	}

	void Stats::WriteLine()
	{
		Line += StringUtility::NewLine;
		Handle.WriteText(Line);
		Line.Clear();
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
