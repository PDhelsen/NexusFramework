#pragma once

#define NEXUS_ENUM_STRING(EnumType, Count, ...)									\
	namespace Enum																\
	{																			\
		namespace EnumType##Utils												\
		{																		\
			inline static NxFr::StringView Names[Count] = { __VA_ARGS__ };		\
			inline NxFr::StringView ToString(EnumType Value)					\
			{																	\
				return Names[(uint64)Value];									\
			}																	\
			inline EnumType FromString(NxFr::StringView Value)					\
			{																	\
				for (int Index = 0; Index < Count; ++Index)						\
					if (Value == Names[Index])									\
							return (EnumType)Index;								\
				return (EnumType)Count;											\
			}																	\
		}																		\
	}

#define NEXUS_FLAG_STRING(EnumType, Count, ...)									\
	namespace Enum																\
	{																			\
		namespace EnumType##Utils												\
		{																		\
			inline static NxFr::StringView Names[Count] = { __VA_ARGS__ };		\
			inline NxFr::StringView ToString(EnumType Value)					\
			{																	\
				uint64 Index = Enum::FlagIndex(Value);							\
				return Index < Count ? Names[Index] : "None";					\
			}																	\
			inline EnumType FromString(NxFr::StringView Value)					\
			{																	\
				for (int Index = 0; Index < Count; ++Index)						\
					if (Value == Names[Index])									\
							return (EnumType)(1 << Index);						\
				return EnumType::None;											\
			}																	\
		}																		\
	}

#define NEXUS_FLAG(EnumType, IntegerType)																											\
	inline EnumType operator ~ (EnumType Value) { return (EnumType)(~(IntegerType)Value); }															\
	inline EnumType operator | (EnumType Value, EnumType Flag) { return (EnumType)((IntegerType)Value | (IntegerType)Flag); }						\
	inline EnumType operator & (EnumType Value, EnumType Flag) { return (EnumType)((IntegerType)Value & (IntegerType)Flag); }						\
	inline EnumType operator ^ (EnumType Value, EnumType Flag) { return (EnumType)((IntegerType)Value ^ (IntegerType)Flag); }						\
	inline EnumType& operator |= (EnumType& Value, EnumType Flag) { return (EnumType&)((IntegerType&)Value |= (IntegerType)Flag); }					\
	inline EnumType& operator &= (EnumType& Value, EnumType Flag) { return (EnumType&)((IntegerType&)Value &= (IntegerType)Flag); }					\
	inline EnumType& operator ^= (EnumType& Value, EnumType Flag) { return (EnumType&)((IntegerType&)Value ^= (IntegerType)Flag); }					\
	namespace Enum																																	\
	{																																				\
		inline bool CheckFlag(EnumType Value, EnumType Flag) { return (IntegerType)(Value & Flag) != 0; }											\
		inline EnumType SetFlagTrue(EnumType Value, EnumType Flag) { return Value | Flag; }															\
		inline EnumType SetFlagFalse(EnumType Value, EnumType Flag) { return Value & ~Flag; }														\
		inline EnumType SetFlag(EnumType Value, EnumType Flag, bool State) { return State ? SetFlagTrue(Value, Flag) : SetFlagFalse(Value, Flag); }	\
		inline IntegerType FlagIndex(EnumType Value) { return ::NxFr::Math::LogTwoPowerOfTwo((IntegerType)Value); }									\
	}
