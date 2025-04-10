#pragma once

#define NEXUS_ENUM_TO_STRING_DEFINITION_NAME(Dll, EnumType, EnumName)\
namespace Enum\
{\
Dll const char* EnumName##ToString(EnumType Value);\
Dll const char* EnumName##ToString(uint64 Value);\
}
#define NEXUS_ENUM_TO_STRING_DEFINITION(Dll, EnumType) NEXUS_ENUM_TO_STRING_DEFINITION_NAME(Dll, EnumType, EnumType)

#define NEXUS_ENUM_TO_STRING_IMPLEMENTATION_NAME_COUNT(EnumType, EnumName, Count, ...)\
namespace Enum\
{\
static const char* EnumName##Strings[Count] = { __VA_ARGS__ };\
const char* EnumName##ToString(EnumType Value) { return EnumName##Strings[(uint64)Value]; }\
const char* EnumName##ToString(uint64 Value) { return EnumName##Strings[Value]; } \
}
#define NEXUS_ENUM_TO_STRING_IMPLEMENTATION_NAME(EnumType, EnumName, ...) NEXUS_ENUM_TO_STRING_IMPLEMENTATION_NAME_COUNT(EnumType, EnumName, (uint64)EnumType::COUNT, __VA_ARGS__)
#define NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(EnumType, Count, ...) NEXUS_ENUM_TO_STRING_IMPLEMENTATION_NAME_COUNT(EnumType, EnumType, Count, __VA_ARGS__)
#define NEXUS_ENUM_TO_STRING_IMPLEMENTATION(EnumType, ...) NEXUS_ENUM_TO_STRING_IMPLEMENTATION_NAME_COUNT(EnumType, EnumType, (uint64)EnumType::COUNT, __VA_ARGS__)

#define NEXUS_ENUM_TO_FLAG_TYPE(EnumType, IntegerType)\
inline constexpr EnumType operator ~ (EnumType A) {  return (EnumType)(~(IntegerType)A); } \
inline constexpr EnumType operator | (EnumType A, EnumType B) {  return (EnumType)((IntegerType)A | (IntegerType)B); } \
inline constexpr EnumType operator & (EnumType A, EnumType B) {  return (EnumType)((IntegerType)A & (IntegerType)B); } \
inline constexpr EnumType operator ^ (EnumType A, EnumType B) {  return (EnumType)((IntegerType)A ^ (IntegerType)B); } \
inline EnumType& operator |= (EnumType& A, EnumType B) {  return (EnumType&)((IntegerType&)A |= (IntegerType)B); } \
inline EnumType& operator &= (EnumType& A, EnumType B) {  return (EnumType&)((IntegerType&)A &= (IntegerType)B); } \
inline EnumType& operator ^= (EnumType& A, EnumType B) {  return (EnumType&)((IntegerType&)A ^= (IntegerType)B); } \
namespace Enum\
{\
inline bool CheckFlag(EnumType A, EnumType F) { return (IntegerType)(A & F) != 0; }\
inline EnumType SetFlagTrue(EnumType A, EnumType F) { return A |= F; }\
inline EnumType SetFlagFalse(EnumType A, EnumType F) { return A &= ~F; }\
inline EnumType SetFlag(EnumType A, EnumType F, bool State) { return State ? SetFlagTrue(A, F) : SetFlagFalse(A, F); }\
}
#define NEXUS_ENUM_TO_FLAG(EnumType) NEXUS_ENUM_TO_FLAG_TYPE(EnumType, uint8)
