#pragma once

#include "Core/NexusEngineBase.h"
#include "Misc/IO/Path.h"
#include "Misc/IO/File.h"

namespace NxEn
{
	class Stats
	{
	public:
		enum class StatType
		{
			Label, Check, Integer, UnsignedInteger, Decimal, DecimalPrecision
		};

		enum class StatMode
		{
			Set, Avg, Min, Max, Cnt, Add
		};

		union StatValue
		{
			String Label;
			int64 Integer;
			uint64 UnsignedInteger;
			float Decimal;
			double DecimalPrecise;
			bool State;

			NEXUS_ENGINE_API StatValue();
			NEXUS_ENGINE_API ~StatValue();
		};

		struct Stat
		{
			friend class Stats;

		public:
			NEXUS_ENGINE_API Stat(StatType Type, StatMode Mode);
			NEXUS_ENGINE_API Stat(const Stat& Other);
			NEXUS_ENGINE_API Stat(Stat&& Other) noexcept;
			NEXUS_ENGINE_API ~Stat();

			NEXUS_ENGINE_API Stat& operator=(const Stat& Other);
			NEXUS_ENGINE_API Stat& operator=(Stat&& Other) noexcept;

			StatType GetType() const { return Type; }
			StatMode GetMode() const { return Mode; }
			const StatValue& GetValue() const { return Value; }

		private:
			NEXUS_ENGINE_API void Reset();

			NEXUS_ENGINE_API void RecordLabel(StringView Statistique);
			NEXUS_ENGINE_API void RecordCheck(bool Statistique);
			NEXUS_ENGINE_API void RecordInteger(int64 Statistique);
			NEXUS_ENGINE_API void RecordUnsignedInteger(uint64 Statistique);
			NEXUS_ENGINE_API void RecordDecimal(float Statistique);
			NEXUS_ENGINE_API void RecordDecimalPrecision(double Statistique);
			NEXUS_ENGINE_API void RecordCount();

			template<typename T>
			T Compute(T Current, T New) const;
			template<typename T>
			T Finalize(T Current, double Span) const;

			NEXUS_ENGINE_API const String& ToString(String& PreAllocated, double Span) const;

			StatValue Value;
			StatType Type;
			StatMode Mode;
		};

	public:
		NEXUS_ENGINE_API Stats(StringView Path);
		NEXUS_ENGINE_API ~Stats();

		NEXUS_ENGINE_API void Initialize();
		NEXUS_ENGINE_API void Lock();
		NEXUS_ENGINE_API void Unlock();
		NEXUS_ENGINE_API void StartRecording();
		NEXUS_ENGINE_API void StopRecording();

		NEXUS_ENGINE_API void Flush();
		NEXUS_ENGINE_API void Reset();

		NEXUS_ENGINE_API void RecordHeader(StringId Name, StatType Type, StatMode Mode);
		NEXUS_ENGINE_API void RecordStatLabel(StringId Id, StringView Value);
		NEXUS_ENGINE_API void RecordStatCheck(StringId Id, bool Value);
		NEXUS_ENGINE_API void RecordStatInteger(StringId Id, int64 Value);
		NEXUS_ENGINE_API void RecordStatUnsignedInteger(StringId Id, uint64 Value);
		NEXUS_ENGINE_API void RecordStatDecimal(StringId Id, float Value);
		NEXUS_ENGINE_API void RecordStatDecimalPrecision(StringId Id, double Value);
		NEXUS_ENGINE_API void RecordStatCount(StringId Id);
		NEXUS_ENGINE_API void RecordComment(StringView Comment);

		NEXUS_ENGINE_API Dictionary<StringId, const Stat*> GetAllCurrentStats() const;
		NEXUS_ENGINE_API const Stat* GetCurrentStat(StringId Id) const;
		NEXUS_ENGINE_API uint64 GetCurrentTick() const;
		NEXUS_ENGINE_API StringView GetCurrentComment() const;

		StringView GetPath() const { return Handle.GetPath(); }
		uint64 GetCount() const { return Data.GetCount(); }
		double GetSpan() const { return Span; }
		bool IsInitialized() const { return Initialized; }
		bool IsRecording() const { return Recording; }
		bool IsLocked() const { return Locked; }

		static Stats* GetInstance() { static Stats* Instance = new Stats(Path::GetWorkingDirectory() + "Stats.csv"); return Instance; }

	private:
		NEXUS_ENGINE_API void WriteLine();
		NEXUS_ENGINE_API Stat& GetStat(StringId Id);
		NEXUS_ENGINE_API const Stat& GetStat(StringId Id) const;

		Dictionary<StringId, uint64> Headers;
		List<Stat> Data;

		File Handle;
		String Line;
		String Cell;

		double Span;
		bool Initialized;
		bool Recording;
		bool Locked;
	};

	template<typename T>
	inline T Stats::Stat::Compute(T Current, T New) const
	{
		switch (Mode)
		{
		case NxEn::Stats::StatMode::Set: return New;
		case NxEn::Stats::StatMode::Avg: return Current + New;
		case NxEn::Stats::StatMode::Min: return Math::Min(Current, New);
		case NxEn::Stats::StatMode::Max: return Math::Max(Current, New);
		case NxEn::Stats::StatMode::Cnt: return ++Current;
		case NxEn::Stats::StatMode::Add: return Current + New;
		}

		return New;
	}

	template<typename T>
	inline T Stats::Stat::Finalize(T Current, double Span) const
	{
		if (Mode == StatMode::Avg)
		{
			return (T)(Current / Span);
		}

		return Current;
	}
}

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_STAT_HEADER_INSTANCE(Instance, Id, Type, Mode) Instance->RecordHeader(Id, Type, Mode)
	#define NEXUS_STAT_LABEL_INSTANCE(Instance, Id, Value) Instance->RecordStatLabel(Id, Value)
	#define NEXUS_STAT_CHECK_INSTANCE(Instance, Id, Value) Instance->RecordStatCheck(Id, Value);
	#define NEXUS_STAT_INTEGER_INSTANCE(Instance, Id, Value) Instance->RecordStatInteger(Id, Value)
	#define NEXUS_STAT_UNSIGNEDINTEGER_INSTANCE(Instance, Id, Value) Instance->RecordStatUnsignedInteger(Id, Value)
	#define NEXUS_STAT_DECIMAL_INSTANCE(Instance, Id, Value) Instance->RecordStatDecimal(Id, Value)
	#define NEXUS_STAT_DECIMALPRECISION_INSTANCE(Instance, Id, Value) Instance->RecordStatDecimalPrecision(Id, Value)
	#define NEXUS_STAT_COUNT_INSTANCE(Instance, Id, Value) Instance->RecordStatCount(Id, Value)
	#define NEXUS_STAT_COMMENT_INSTANCE(Instance, Id, Value) Instance->RecordComment(Id, Value)

	#define NEXUS_STAT_HEADER(Id, Type, Mode) NEXUS_STAT_HEADER_INSTANCE(::NxEn::Stats::GetInstance(), Id, Type, Mode)
	#define NEXUS_STAT_LABEL(Id, Value) NEXUS_STAT_LABEL_INSTANCE(::NxEn::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_CHECK(Id, Value) NEXUS_STAT_CHECK_INSTANCE(::NxEn::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_INTEGER(Id, Value) NEXUS_STAT_INTEGER_INSTANCE(::NxEn::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_UNSIGNEDINTEGER(Id, Value) NEXUS_STAT_UNSIGNEDINTEGER_INSTANCE(::NxEn::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_DECIMAL(Id, Value) NEXUS_STAT_DECIMAL_INSTANCE(::NxEn::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_DECIMALPRECISION(Id, Value) NEXUS_STAT_DECIMALPRECISION_INSTANCE(::NxEn::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_COUNT(Id, Value) NEXUS_STAT_COUNT_INSTANCE(::NxEn::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_COMMENT(Id, Value) NEXUS_STAT_COMMENT_INSTANCE(::NxEn::Stats::GetInstance(), Id, Value)
#elif NEXUS_DISTRIB
	#define NEXUS_STAT_HEADER_INSTANCE(Instance, Id, Type, Mode)
	#define NEXUS_STAT_LABEL_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_CHECK_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_INTEGER_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_UNSIGNEDINTEGER_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_DECIMAL_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_DECIMALPRECISION_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_COUNT_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_COMMENT_INSTANCE(Instance, Id, Value)

	#define NEXUS_STAT_HEADER(Id, Type, Mode)
	#define NEXUS_STAT_LABEL(Id, Value)
	#define NEXUS_STAT_CHECK(Id, Value) 
	#define NEXUS_STAT_INTEGER(Id, Value)
	#define NEXUS_STAT_UNSIGNEDINTEGER(Id, Value)
	#define NEXUS_STAT_DECIMAL(Id, Value)
	#define NEXUS_STAT_DECIMALPRECISION(Id, Value)
	#define NEXUS_STAT_COUNT(Id, Value)
	#define NEXUS_STAT_COMMENT(Id, Value)
#endif
