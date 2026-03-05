#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numbers/Integer.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/IO/File.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	namespace StatsHeader
	{
		NEXUS_FRAMEWORK_API extern const StringId TickId;
		NEXUS_FRAMEWORK_API extern const StringId CommentId;
	}

	class Stats
	{
	public:
		enum class StatType
		{
			Label, Check, Integer, UnsignedInteger, Decimal, DecimalPrecision
		};

		enum class StatMode
		{
			Set, Cnt, Add, Avg, Min, Max
		};

		union StatValue
		{
			String Label;
			int64 Integer;
			uint64 UnsignedInteger;
			float Decimal;
			double DecimalPrecise;
			bool State;

			NEXUS_FRAMEWORK_API StatValue();
			NEXUS_FRAMEWORK_API ~StatValue();
		};

		struct Stat
		{
			friend class Stats;

		public:
			NEXUS_FRAMEWORK_API Stat(StatType Type, StatMode Mode);
			NEXUS_FRAMEWORK_API Stat(const Stat& Other);
			NEXUS_FRAMEWORK_API Stat(Stat&& Other) noexcept;
			NEXUS_FRAMEWORK_API ~Stat();

			NEXUS_FRAMEWORK_API Stat& operator=(const Stat& Other);
			NEXUS_FRAMEWORK_API Stat& operator=(Stat&& Other) noexcept;

			template<typename T>
			T GetValue() const;
			StatType GetType() const { return Type; }
			StatMode GetMode() const { return Mode; }

		private:
			NEXUS_FRAMEWORK_API void Reset();

			NEXUS_FRAMEWORK_API void RecordLabel(StringView Statistique);
			NEXUS_FRAMEWORK_API void RecordCheck(bool Statistique);
			NEXUS_FRAMEWORK_API void RecordInteger(int64 Statistique);
			NEXUS_FRAMEWORK_API void RecordUnsignedInteger(uint64 Statistique);
			NEXUS_FRAMEWORK_API void RecordDecimal(float Statistique);
			NEXUS_FRAMEWORK_API void RecordDecimalPrecision(double Statistique);

			template<typename T>
			T Compute(T Current, T New) const;
			template<typename T>
			T Finalize(T Current) const;

			StatValue Value;
			StatType Type;
			StatMode Mode;
			uint64 Tick;
		};

	public:
		NEXUS_FRAMEWORK_API static Stats* GetInstance();

		NEXUS_FRAMEWORK_API Stats(StringView Path);
		NEXUS_FRAMEWORK_API ~Stats();

		NEXUS_FRAMEWORK_API void Initialize();
		NEXUS_FRAMEWORK_API void Flush();
		NEXUS_FRAMEWORK_API void Reset();

		NEXUS_FRAMEWORK_API void RecordHeader(StringId Name, StatType Type, StatMode Mode);
		NEXUS_FRAMEWORK_API void RecordStatLabel(StringId Id, StringView Value);
		NEXUS_FRAMEWORK_API void RecordStatCheck(StringId Id, bool Value);
		NEXUS_FRAMEWORK_API void RecordStatInteger(StringId Id, int64 Value);
		NEXUS_FRAMEWORK_API void RecordStatUnsignedInteger(StringId Id, uint64 Value);
		NEXUS_FRAMEWORK_API void RecordStatDecimal(StringId Id, float Value);
		NEXUS_FRAMEWORK_API void RecordStatDecimalPrecision(StringId Id, double Value);
		NEXUS_FRAMEWORK_API void RecordComment(StringView Comment);

		NEXUS_FRAMEWORK_API void Lock();
		NEXUS_FRAMEWORK_API void Unlock();
		NEXUS_FRAMEWORK_API void StartRecording();
		NEXUS_FRAMEWORK_API void StopRecording();

		template<typename T>
		T GetCurrentStatValue(StringId Id) const;
		NEXUS_FRAMEWORK_API const Stat* GetCurrentStat(StringId Id) const;
		NEXUS_FRAMEWORK_API Dictionary<StringId, const Stat*> GetAllCurrentStats() const;
		NEXUS_FRAMEWORK_API void GetAllCurrentStats(Dictionary<StringId, const Stat*>& Result) const;

		StringView GetPath() const { return Handle.GetPath(); }
		uint64 GetCount() const { return Data.GetCount(); }
		bool IsInitialized() const { return Initialized; }
		bool IsRecording() const { return Recording; }
		bool IsLocked() const { return Locked; }

	private:
		NEXUS_FRAMEWORK_API void WriteLine();
		NEXUS_FRAMEWORK_API Stat& GetStat(StringId Id);
		NEXUS_FRAMEWORK_API const Stat& GetStat(StringId Id) const;

		Dictionary<StringId, uint64> Headers;
		List<Stat> Data;

		String BufferLine;
		String BufferCell;

		File Handle;

		bool Initialized;
		bool Recording;
		bool Locked;

		Mutex Guard;
	};

	template<typename T>
	inline T Stats::Stat::GetValue() const
	{
		return 0;
	}

	template<>
	inline const String& Stats::Stat::GetValue() const
	{
		return Value.Label;
	}

	template<>
	inline bool Stats::Stat::GetValue() const
	{
		return Value.State;
	}

	template<>
	inline int64 Stats::Stat::GetValue() const
	{
		return Finalize(Value.Integer);
	}

	template<>
	inline uint64 Stats::Stat::GetValue() const
	{
		return Finalize(Value.UnsignedInteger);
	}

	template<>
	inline float Stats::Stat::GetValue() const
	{
		return Finalize(Value.Decimal);
	}

	template<>
	inline double Stats::Stat::GetValue() const
	{
		return Finalize(Value.DecimalPrecise);
	}

	template<typename T>
	inline T Stats::Stat::Compute(T Current, T New) const
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

	template<typename T>
	inline T Stats::Stat::Finalize(T Current) const
	{
		if (Mode == StatMode::Avg)
		{
			return (T)(Current / (double)Tick);
		}

		return Current;
	}

	template<typename T>
	inline T Stats::GetCurrentStatValue(StringId Id) const
	{
		return GetCurrentStat(Id)->GetValue<T>();
	}
}

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_STAT_HEADER_INSTANCE(Instance, Id, Type, Mode) if (Instance) { Instance->RecordHeader(Id, ::NxFr::Stats::StatType::Type, ::NxFr::Stats::StatMode::Mode); }
	#define NEXUS_STAT_LABEL_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatLabel(Id, Value); }
	#define NEXUS_STAT_CHECK_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatCheck(Id, Value); }
	#define NEXUS_STAT_INTEGER_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatInteger(Id, Value); }
	#define NEXUS_STAT_UNSIGNEDINTEGER_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatUnsignedInteger(Id, Value); }
	#define NEXUS_STAT_DECIMAL_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatDecimal(Id, Value); }
	#define NEXUS_STAT_DECIMALPRECISION_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatDecimalPrecision(Id, Value); }
	#define NEXUS_STAT_COUNT_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatCount(Id, Value); }
	#define NEXUS_STAT_COMMENT_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordComment(Id, Value); }

	#define NEXUS_STAT_HEADER(Id, Type, Mode) NEXUS_STAT_HEADER_INSTANCE(::NxFr::Stats::GetInstance(), Id, Type, Mode)
	#define NEXUS_STAT_LABEL(Id, Value) NEXUS_STAT_LABEL_INSTANCE(::NxFr::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_CHECK(Id, Value) NEXUS_STAT_CHECK_INSTANCE(::NxFr::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_INTEGER(Id, Value) NEXUS_STAT_INTEGER_INSTANCE(::NxFr::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_UNSIGNEDINTEGER(Id, Value) NEXUS_STAT_UNSIGNEDINTEGER_INSTANCE(::NxFr::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_DECIMAL(Id, Value) NEXUS_STAT_DECIMAL_INSTANCE(::NxFr::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_DECIMALPRECISION(Id, Value) NEXUS_STAT_DECIMALPRECISION_INSTANCE(::NxFr::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_COUNT(Id, Value) NEXUS_STAT_COUNT_INSTANCE(::NxFr::Stats::GetInstance(), Id, Value)
	#define NEXUS_STAT_COMMENT(Id, Value) NEXUS_STAT_COMMENT_INSTANCE(::NxFr::Stats::GetInstance(), Id, Value)
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
