#pragma once

// Macro to define common function related to enum
// The Enum to String macro needs to be used in .h & .cpp. The .h macro needs to be used inside a class

#define NEXUS_ENUM_TO_STRING_DEFINITION(EnumType)\
const char* EnumType##ToString(EnumType Value) const;\
const char* EnumType##ToString(uint64 Value) const;
#define NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(EnumType, Class, Count, ...)\
static const char* EnumType##Strings[Count] = { __VA_ARGS__ };\
const char* Class##EnumType##ToString(EnumType Value) const { return EnumType##Strings[(uint64)Value]; }\
const char* Class##EnumType##ToString(uint64 Value) const { return EnumType##Strings[Value]; }
#define NEXUS_ENUM_TO_STRING_IMPLEMENTATION(EnumType, Class, ...) NEXUS_ENUM_TO_STRING_IMPLEMENTATION_COUNT(EnumType, Class, (uint64)EnumType::COUNT, __VA_ARGS__)

#define NEXUS_FLAG(EnumType, IntegerType)\
inline constexpr EnumType operator ~ (EnumType A) {  return (EnumType)(~(IntegerType)A); } \
inline constexpr EnumType operator | (EnumType A, EnumType B) {  return (EnumType)((IntegerType)A | (IntegerType)B); } \
inline constexpr EnumType operator & (EnumType A, EnumType B) {  return (EnumType)((IntegerType)A & (IntegerType)B); } \
inline constexpr EnumType operator ^ (EnumType A, EnumType B) {  return (EnumType)((IntegerType)A ^ (IntegerType)B); } \
inline EnumType& operator |= (EnumType& A, EnumType B) {  return (EnumType&)((IntegerType&)A |= (IntegerType)B); } \
inline EnumType& operator &= (EnumType& A, EnumType B) {  return (EnumType&)((IntegerType&)A &= (IntegerType)B); } \
inline EnumType& operator ^= (EnumType& A, EnumType B) {  return (EnumType&)((IntegerType&)A ^= (IntegerType)B); } \
inline bool CheckFlag(EnumType A, EnumType F) { return (IntegerType)(A & F) != 0; }\
inline EnumType SetFlagTrue(EnumType A, EnumType F) { return A |= F; }\
inline EnumType SetFlagFalse(EnumType A, EnumType F) { return A &= ~F; }\
inline EnumType SetFlag(EnumType A, EnumType F, bool State) { return State ? SetFlagTrue(A, F) : SetFlagFalse(A, F); }