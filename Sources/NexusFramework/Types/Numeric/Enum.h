#pragma once

#include "NexusFramework/Math/Math.h"

#define NX_ENUM_STRING(EnumType, Count, ...)\
	namespace NxFr\
	{\
		namespace StringUtility\
		{\
			template<>\
			struct Converter<EnumType>\
			{\
				inline static const StringView Names[Count] = { __VA_ARGS__ }; \
				static void ToString(const EnumType& Data, String& Result, StringView Format = "")\
				{\
					Result = Names[(uint64)Data];\
				}\
				static void FromString(StringView Data, EnumType& Result, StringView Format = "")\
				{\
					for (int Index = 0; Index < Count; ++Index)\
					{\
						if (Data == Names[Index])\
						{\
							Result = (EnumType)Index;\
							return;\
						}\
					}\
					Result = (EnumType)Count;\
				}\
			};\
		}\
	}

#define NX_FLAG_STRING(EnumType, Count, ...)\
	namespace NxFr\
	{\
		namespace StringUtility\
		{\
			template<>\
			struct Converter<EnumType>\
			{\
				inline static const StringView Names[Count] = { __VA_ARGS__ }; \
				static void ToString(const EnumType& Data, String& Result, StringView Format = "")\
				{\
					uint64 Index = Enum::FlagIndex(Data);\
					Result = Index < Count ? Names[Index] : "None";\
				}\
				static void FromString(StringView Data, EnumType& Result, StringView Format = "")\
				{\
					for (int Index = 0; Index < Count; ++Index)\
					{\
						if (Data == Names[Index])\
						{\
							Result = (EnumType)(1 << Index);\
							return;\
						}\
					}\
					Result = EnumType::None;\
				}\
			};\
		}\
	}

#define NX_FLAG(EnumType, IntegerType)\
	inline EnumType operator ~ (EnumType Value) { return (EnumType)(~(IntegerType)Value); }\
	inline EnumType operator | (EnumType Value, EnumType Flag) { return (EnumType)((IntegerType)Value | (IntegerType)Flag); }\
	inline EnumType operator & (EnumType Value, EnumType Flag) { return (EnumType)((IntegerType)Value & (IntegerType)Flag); }\
	inline EnumType operator ^ (EnumType Value, EnumType Flag) { return (EnumType)((IntegerType)Value ^ (IntegerType)Flag); }\
	inline EnumType& operator |= (EnumType& Value, EnumType Flag) { return (EnumType&)((IntegerType&)Value |= (IntegerType)Flag); }\
	inline EnumType& operator &= (EnumType& Value, EnumType Flag) { return (EnumType&)((IntegerType&)Value &= (IntegerType)Flag); }\
	inline EnumType& operator ^= (EnumType& Value, EnumType Flag) { return (EnumType&)((IntegerType&)Value ^= (IntegerType)Flag); }\
	namespace NxFr\
	{\
		namespace Enum\
		{\
			inline bool CheckFlag(EnumType Value, EnumType Flag) { return (IntegerType)(Value & Flag) != 0; }\
			inline EnumType SetFlagTrue(EnumType Value, EnumType Flag) { return Value | Flag; }\
			inline EnumType SetFlagFalse(EnumType Value, EnumType Flag) { return Value & ~Flag; }\
			inline EnumType SetFlag(EnumType Value, EnumType Flag, bool State) { return State ? SetFlagTrue(Value, Flag) : SetFlagFalse(Value, Flag); }\
			inline IntegerType FlagIndex(EnumType Value) { return Math::LogTwoPowerOfTwo((IntegerType)Value); }\
		}\
	}
