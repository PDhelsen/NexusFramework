#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/IO/Stream.h"
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
			Label, Check, Integer, Decimal
		};

		enum class StatMode
		{
			Set, Cnt, Add, Avg, Min, Max
		};

		union StatValue
		{
			NEXUS_FRAMEWORK_API StatValue();
			NEXUS_FRAMEWORK_API ~StatValue();

			String Label;
			int64 Integer;
			float Decimal;
			bool State;
		};

		struct Stat
		{
			friend class Stats;

		public:
			NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, Stat)
			NEXUS_FRAMEWORK_API Stat(StatType Type, StatMode Mode);
			NEXUS_FRAMEWORK_API ~Stat();

			template<typename T> T GetValue() const { return (T)0; }
			StatType GetType() const { return Type; }
			StatMode GetMode() const { return Mode; }

		private:
			NEXUS_FRAMEWORK_API void Reset();
			NEXUS_FRAMEWORK_API double Compute(double Current, double New) const;
			NEXUS_FRAMEWORK_API double Finalize(double Current) const;

			NEXUS_FRAMEWORK_API void RecordLabel(StringView Statistique);
			NEXUS_FRAMEWORK_API void RecordCheck(bool Statistique);
			NEXUS_FRAMEWORK_API void RecordInteger(int64 Statistique);
			NEXUS_FRAMEWORK_API void RecordDecimal(float Statistique);

			StatValue Value;
			StatType Type;
			StatMode Mode;
			uint64 Tick;
		};

	public:
		inline static const String Separator = ";";

		NEXUS_NOCOPY_NOMOVE(NEXUS_FRAMEWORK_API, Stats)
		NEXUS_FRAMEWORK_API Stats(StringView Path);
		NEXUS_FRAMEWORK_API ~Stats();

		NEXUS_FRAMEWORK_API void Initialize();
		NEXUS_FRAMEWORK_API void Flush();
		NEXUS_FRAMEWORK_API void Reset();

		NEXUS_FRAMEWORK_API void RecordHeader(StringId Name, StatType Type, StatMode Mode);
		NEXUS_FRAMEWORK_API void RecordStatLabel(StringId Id, StringView Value);
		NEXUS_FRAMEWORK_API void RecordStatCheck(StringId Id, bool Value);
		NEXUS_FRAMEWORK_API void RecordStatInteger(StringId Id, int64 Value);
		NEXUS_FRAMEWORK_API void RecordStatDecimal(StringId Id, float Value);
		NEXUS_FRAMEWORK_API void RecordComment(StringView Comment);

		NEXUS_FRAMEWORK_API void StartRecording();
		NEXUS_FRAMEWORK_API void StopRecording();

		NEXUS_FRAMEWORK_API Array<StringId> GetHeaders() const;
		NEXUS_FRAMEWORK_API Dictionary<StringId, const Stat*> GetStats() const;
		NEXUS_FRAMEWORK_API const Stat& GetStat(StringId Id) const;

		uint64 GetCount() const { return Data.GetCount(); }
		bool IsInitialized() const { return Initialized; }
		bool IsRecording() const { return Recording; }

	private:
		Dictionary<StringId, uint64> Headers;
		List<Stat> Data;

		TextStream Stream;
		String Buffer;

		bool Initialized;
		bool Recording;

		Mutex Guard;
	};

	template<>
	inline StringView Stats::Stat::GetValue() const
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
	inline float Stats::Stat::GetValue() const
	{
		return Finalize(Value.Decimal);
	}
}

#if NEXUS_DEBUG || NEXUS_RELEASE
	#define NEXUS_STAT_HEADER_INSTANCE(Instance, Id, Type, Mode) if (Instance) { Instance->RecordHeader(Id, ::NxFr::Stats::StatType::Type, ::NxFr::Stats::StatMode::Mode); }
	#define NEXUS_STAT_LABEL_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatLabel(Id, Value); }
	#define NEXUS_STAT_CHECK_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatCheck(Id, Value); }
	#define NEXUS_STAT_INTEGER_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatInteger(Id, Value); }
	#define NEXUS_STAT_DECIMAL_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatDecimal(Id, Value); }
	#define NEXUS_STAT_COMMENT_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordComment(Id, Value); }

	#define NEXUS_STAT_HEADER(Id, Type, Mode) NEXUS_STAT_HEADER_INSTANCE(::NxFr::Globals::Statistiques, Id, Type, Mode)
	#define NEXUS_STAT_LABEL(Id, Value) NEXUS_STAT_LABEL_INSTANCE(::NxFr::Globals::Statistiques, Id, Value)
	#define NEXUS_STAT_CHECK(Id, Value) NEXUS_STAT_CHECK_INSTANCE(::NxFr::Globals::Statistiques, Id, Value)
	#define NEXUS_STAT_INTEGER(Id, Value) NEXUS_STAT_INTEGER_INSTANCE(::NxFr::Globals::Statistiques, Id, Value)
	#define NEXUS_STAT_DECIMAL(Id, Value) NEXUS_STAT_DECIMAL_INSTANCE(::NxFr::Globals::Statistiques, Id, Value)
	#define NEXUS_STAT_COMMENT(Id, Value) NEXUS_STAT_COMMENT_INSTANCE(::NxFr::Globals::Statistiques, Id, Value)
#elif NEXUS_DISTRIB
	#define NEXUS_STAT_HEADER_INSTANCE(Instance, Id, Type, Mode)
	#define NEXUS_STAT_LABEL_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_CHECK_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_INTEGER_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_DECIMAL_INSTANCE(Instance, Id, Value)
	#define NEXUS_STAT_COMMENT_INSTANCE(Instance, Id, Value)

	#define NEXUS_STAT_HEADER(Id, Type, Mode)
	#define NEXUS_STAT_LABEL(Id, Value)
	#define NEXUS_STAT_CHECK(Id, Value)
	#define NEXUS_STAT_INTEGER(Id, Value)
	#define NEXUS_STAT_DECIMAL(Id, Value)
	#define NEXUS_STAT_COMMENT(Id, Value)
#endif
