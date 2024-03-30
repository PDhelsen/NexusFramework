#pragma once

// TODO: Implementation - Enum & Flags - Improve macro 
// Add support for count, to string
// See if there is something else

#define NEXUS_FLAG(EnumType, IntegerType)\
constexpr EnumType operator ~ (EnumType A) {  return (EnumType)(~(IntegerType)A); } \
constexpr EnumType operator | (EnumType A, EnumType B) {  return (EnumType)((IntegerType)A | (IntegerType)B); } \
constexpr EnumType operator & (EnumType A, EnumType B) {  return (EnumType)((IntegerType)A & (IntegerType)B); } \
constexpr EnumType operator ^ (EnumType A, EnumType B) {  return (EnumType)((IntegerType)A ^ (IntegerType)B); } \
EnumType& operator |= (EnumType& A, EnumType B) {  return (EnumType&)((IntegerType&)A |= (IntegerType)B); } \
EnumType& operator &= (EnumType& A, EnumType B) {  return (EnumType&)((IntegerType&)A &= (IntegerType)B); } \
EnumType& operator ^= (EnumType& A, EnumType B) {  return (EnumType&)((IntegerType&)A ^= (IntegerType)B); } \
