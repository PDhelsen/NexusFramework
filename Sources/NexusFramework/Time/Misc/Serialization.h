#pragma once

#include "NexusFramework/Serialization/Rbs.h"
#include "NexusFramework/Serialization/Yaml.h"
#include "NexusFramework/Debug/Logger/Log.h"

#include "NexusFramework/Time/Timespan.h"
#include "NexusFramework/Time/Timestamp.h"
#include "NexusFramework/Time/Stopwatch.h"

namespace NxFr
{
	namespace RBSUtility
	{
		template<>
		struct Converter<Timespan>
		{
			static Timespan Decode(const RBS& Rbs)
			{
				Timespan Result;
				Result.Seconds = Rbs.ReadObject<int32>();
				Result.Minutes = Rbs.ReadObject<int32>();
				Result.Hours = Rbs.ReadObject<int32>();
				Result.Days = Rbs.ReadObject<int32>();
				Result.Months = Rbs.ReadObject<int32>();
				Result.Years = Rbs.ReadObject<int32>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Timespan& Object)
			{
				Rbs.WriteObject(Object.Seconds);
				Rbs.WriteObject(Object.Minutes);
				Rbs.WriteObject(Object.Hours);
				Rbs.WriteObject(Object.Days);
				Rbs.WriteObject(Object.Months);
				Rbs.WriteObject(Object.Years);
			}
		};

		template<>
		struct Converter<Timestamp>
		{
			static Timestamp Decode(const RBS& Rbs)
			{
				Timestamp Result;
				Result.Seconds = Rbs.ReadObject<int32>();
				Result.Minutes = Rbs.ReadObject<int32>();
				Result.Hours = Rbs.ReadObject<int32>();
				Result.Days = Rbs.ReadObject<int32>();
				Result.Months = Rbs.ReadObject<int32>();
				Result.Years = Rbs.ReadObject<int32>();
				Result.WeekDay = Rbs.ReadObject<int32>();
				Result.YearDay = Rbs.ReadObject<int32>();
				Result.DayLightSaving = Rbs.ReadObject<int32>();
				return Result;
			}

			static void Encode(RBS& Rbs, const Timestamp& Object)
			{
				Rbs.WriteObject(Object.Seconds);
				Rbs.WriteObject(Object.Minutes);
				Rbs.WriteObject(Object.Hours);
				Rbs.WriteObject(Object.Days);
				Rbs.WriteObject(Object.Months);
				Rbs.WriteObject(Object.Years);
				Rbs.WriteObject(Object.WeekDay);
				Rbs.WriteObject(Object.YearDay);
				Rbs.WriteObject(Object.DayLightSaving);
			}
		};

		template<>
		struct Converter<Stopwatch>
		{
			static Stopwatch Decode(const RBS& Rbs)
			{
				NEXUS_ASSERT(false, Default, "Unsupported FromString with Stopwatch");
				return Stopwatch();

			}

			static void Encode(RBS& Rbs, const Stopwatch& Object)
			{
				Rbs.WriteObject(Object.Peek());
			}
		};
	}
}

namespace YAML
{
	template<>
	struct convert<NxFr::Timespan>
	{
		static Node encode(const NxFr::Timespan& rhs)
		{
			Node node;

			node["seconds"] = rhs.Seconds;
			node["minutes"] = rhs.Minutes;
			node["hours"] = rhs.Hours;
			node["days"] = rhs.Days;
			node["months"] = rhs.Months;
			node["years"] = rhs.Years;

			return node;
		}

		static bool decode(const Node& node, NxFr::Timespan& rhs)
		{
			rhs.Seconds = node["seconds"].as<int32>();
			rhs.Minutes = node["minutes"].as<int32>();
			rhs.Hours = node["hours"].as<int32>();
			rhs.Days = node["days"].as<int32>();
			rhs.Months = node["months"].as<int32>();
			rhs.Years = node["years"].as<int32>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Timespan& rhs)
	{
		out << YAML::Key << "seconds" << YAML::Value << rhs.Seconds;
		out << YAML::Key << "minutes" << YAML::Value << rhs.Minutes;
		out << YAML::Key << "hours" << YAML::Value << rhs.Hours;
		out << YAML::Key << "days" << YAML::Value << rhs.Days;
		out << YAML::Key << "months" << YAML::Value << rhs.Months;
		out << YAML::Key << "years" << YAML::Value << rhs.Years;

		return out;
	}

	template<>
	struct convert<NxFr::Timestamp>
	{
		static Node encode(const NxFr::Timestamp& rhs)
		{
			Node node;

			node["seconds"] = rhs.Seconds;
			node["minutes"] = rhs.Minutes;
			node["hours"] = rhs.Hours;
			node["days"] = rhs.Days;
			node["months"] = rhs.Months;
			node["years"] = rhs.Years;
			node["weekday"] = rhs.WeekDay;
			node["yearday"] = rhs.YearDay;
			node["daylightsaving"] = rhs.DayLightSaving;

			return node;
		}

		static bool decode(const Node& node, NxFr::Timestamp& rhs)
		{
			rhs.Seconds = node["seconds"].as<int32>();
			rhs.Minutes = node["minutes"].as<int32>();
			rhs.Hours = node["hours"].as<int32>();
			rhs.Days = node["days"].as<int32>();
			rhs.Months = node["months"].as<int32>();
			rhs.Years = node["years"].as<int32>();
			rhs.WeekDay = node["weekday"].as<int32>();
			rhs.YearDay = node["yearday"].as<int32>();
			rhs.DayLightSaving = node["daylightsaving"].as<int32>();

			return true;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Timestamp& rhs)
	{
		out << YAML::Key << "seconds" << YAML::Value << rhs.Seconds;
		out << YAML::Key << "minutes" << YAML::Value << rhs.Minutes;
		out << YAML::Key << "hours" << YAML::Value << rhs.Hours;
		out << YAML::Key << "days" << YAML::Value << rhs.Days;
		out << YAML::Key << "months" << YAML::Value << rhs.Months;
		out << YAML::Key << "years" << YAML::Value << rhs.Years;
		out << YAML::Key << "weekday" << YAML::Value << rhs.WeekDay;
		out << YAML::Key << "yearday" << YAML::Value << rhs.YearDay;
		out << YAML::Key << "daylightsaving" << YAML::Value << rhs.DayLightSaving;

		return out;
	}

	template<>
	struct convert<NxFr::Stopwatch>
	{
		static Node encode(const NxFr::Stopwatch& rhs)
		{
			Node node;

			node["elapsed"] = rhs.Peek();

			return node;
		}

		static bool decode(const Node& node, NxFr::Stopwatch& rhs)
		{
			return false;
		}
	};

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const NxFr::Stopwatch& rhs)
	{
		out << YAML::Key << "elapsed" << YAML::Value << rhs.Peek();

		return out;
	}
}
