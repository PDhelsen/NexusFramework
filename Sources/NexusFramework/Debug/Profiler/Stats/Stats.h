#pragma once

#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/Types/Numeric/Integer.h"
#include "NexusFramework/Types/Containers/List.h"
#include "NexusFramework/Types/Containers/Dictionary.h"
#include "NexusFramework/Types/Strings/String.h"
#include "NexusFramework/Types/Strings/StringView.h"
#include "NexusFramework/Types/Strings/StringId.h"
#include "NexusFramework/Misc/Templates.h"
#include "NexusFramework/IO/Stream.h"
#include "NexusFramework/Threading/Mutex.h"

namespace NxFr
{
	namespace StatsHeader
	{
		inline const StringId TickId = "Tick"_Sid;
		inline const StringId CommentId = "Comments"_Sid;
	}

	class NX_FRAMEWORK_API Stats
	{
	public:
		enum class Type
		{
			Label, Check, Integer, Decimal
		};

		enum class Mode
		{
			Set, Cnt, Add, Avg, Min, Max
		};

		union NX_FRAMEWORK_API Value
		{
			Value();
			~Value();

			String Label;
			int64 Integer;
			float Decimal;
			bool State;
		};

		struct NX_FRAMEWORK_API Stat
		{
			friend class Stats;

		public:
			NX_NOCOPY_NOMOVE(Stat)
			Stat(Type Type, Mode Mode);
			~Stat();

			StringView GetValueLabel() const { return StatValue.Label; }
			bool GetValueCheck() const { return StatValue.State; }
			int64 GetValueInteger() const { return Finalize(StatValue.Integer); }
			float GetValueDecimal() const { return Finalize(StatValue.Decimal); }
			Type GetType() const { return StatType; }
			Mode GetMode() const { return StatMode; }

		private:
			void Reset();
			double Compute(double Current, double New) const;
			double Finalize(double Current) const;

			void RecordLabel(StringView Statistique);
			void RecordCheck(bool Statistique);
			void RecordInteger(int64 Statistique);
			void RecordDecimal(float Statistique);

			Value StatValue;
			Type StatType;
			Mode StatMode;
			uint64 StatTick;
		};

		inline static const String Separator = ";";

		NX_NOCOPY_NOMOVE(Stats)
		Stats(StringView Path);
		~Stats();

		void Initialize();
		void Flush();
		void Reset();

		void RecordHeader(StringId Name, Type Type, Mode Mode);
		void RecordStatLabel(StringId Id, StringView Value);
		void RecordStatCheck(StringId Id, bool Value);
		void RecordStatInteger(StringId Id, int64 Value);
		void RecordStatDecimal(StringId Id, float Value);
		void RecordComment(StringView Comment);

		void StartRecording();
		void StopRecording();

		Array<StringId> GetHeaders() const;
		Dictionary<StringId, const Stat*> GetStats() const;
		const Stat& GetStat(StringId Id) const;

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
}

#if NX_DEBUG || NX_RELEASE
	#define NX_STAT_HEADER_INSTANCE(Instance, Id, StatType, StatMode) if (Instance) { Instance->RecordHeader(Id, ::NxFr::Stats::Type::StatType, ::NxFr::Stats::Mode::StatMode); }
	#define NX_STAT_LABEL_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatLabel(Id, Value); }
	#define NX_STAT_CHECK_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatCheck(Id, Value); }
	#define NX_STAT_INTEGER_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatInteger(Id, Value); }
	#define NX_STAT_DECIMAL_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordStatDecimal(Id, Value); }
	#define NX_STAT_COMMENT_INSTANCE(Instance, Id, Value) if (Instance) { Instance->RecordComment(Id, Value); }

	#define NX_STAT_HEADER(Id, StatType, StatMode) NX_STAT_HEADER_INSTANCE(::NxFr::Globals::Debug::Statistiques, Id, StatType, StatMode)
	#define NX_STAT_LABEL(Id, Value) NX_STAT_LABEL_INSTANCE(::NxFr::Globals::Debug::Statistiques, Id, Value)
	#define NX_STAT_CHECK(Id, Value) NX_STAT_CHECK_INSTANCE(::NxFr::Globals::Debug::Statistiques, Id, Value)
	#define NX_STAT_INTEGER(Id, Value) NX_STAT_INTEGER_INSTANCE(::NxFr::Globals::Debug::Statistiques, Id, Value)
	#define NX_STAT_DECIMAL(Id, Value) NX_STAT_DECIMAL_INSTANCE(::NxFr::Globals::Debug::Statistiques, Id, Value)
	#define NX_STAT_COMMENT(Id, Value) NX_STAT_COMMENT_INSTANCE(::NxFr::Globals::Debug::Statistiques, Id, Value)
#elif NX_DISTRIB
	#define NX_STAT_HEADER_INSTANCE(Instance, Id, StatType, StatMode)
	#define NX_STAT_LABEL_INSTANCE(Instance, Id, Value)
	#define NX_STAT_CHECK_INSTANCE(Instance, Id, Value)
	#define NX_STAT_INTEGER_INSTANCE(Instance, Id, Value)
	#define NX_STAT_DECIMAL_INSTANCE(Instance, Id, Value)
	#define NX_STAT_COMMENT_INSTANCE(Instance, Id, Value)

	#define NX_STAT_HEADER(Id, StatType, StatMode)
	#define NX_STAT_LABEL(Id, Value)
	#define NX_STAT_CHECK(Id, Value)
	#define NX_STAT_INTEGER(Id, Value)
	#define NX_STAT_DECIMAL(Id, Value)
	#define NX_STAT_COMMENT(Id, Value)
#endif
