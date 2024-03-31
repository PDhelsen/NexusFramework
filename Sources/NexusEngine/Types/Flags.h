#pragma once

// TODO: Implementation - Enum & Flags - Improve macro 
// Add support for count, to string
// See if there is something else

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