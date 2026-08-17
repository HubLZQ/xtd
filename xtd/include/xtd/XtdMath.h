#pragma once
#include "XtdDefine.h"
#include <cmath>
#ifdef XTD_COMPILER_MSVC
    #include <corecrt_math_defines.h>
#endif

// ReSharper disable CppCStyleCast

BEGIN_NAMESPACE_XTD

static constexpr double X_E        = 2.71828182845904523536; // e
static constexpr double X_LOG2E    = 1.44269504088896340736; // log2(e)
static constexpr double X_LOG10E   = 0.434294481903251827651;// log10(e)
static constexpr double X_LN2      = 0.693147180559945309417;// ln(2)
static constexpr double X_LN10     = 2.30258509299404568402; // ln(10)
static constexpr double X_PI       = 3.14159265358979323846; // pi
static constexpr double X_PI_2     = 1.57079632679489661923; // pi/2
static constexpr double X_PI_4     = 0.785398163397448309616;// pi/4
static constexpr double X_1_PI     = 0.318309886183790671538;// 1/pi
static constexpr double X_2_PI     = 0.636619772367581343076;// 2/pi
static constexpr double X_2_SQRTPI = 1.12837916709551257390; // 2/sqrt(pi)
static constexpr double X_SQRT2    = 1.41421356237309504880; // sqrt(2)
static constexpr double X_SQRT1_2  = 0.707106781186547524401;// 1/sqrt(2)

template<typename T>
constexpr auto Max(const T& a, const T& b)
{
    return a > b ? a : b;
}

template<typename T>
constexpr auto Min(const T& a, const T& b)
{
    return a < b ? a : b;
}

template<typename T>
constexpr auto Clamp(const T& value, const T& min, const T& max)
{
    return value < min ? min : (value > max ? max : value);
}

// clang-format off
inline I32 ToI32(const Char* str, Char** strEnd = Null, const int base = 0) { return ::strtol  (str, strEnd, base); }
inline I64 ToI64(const Char* str, Char** strEnd = Null, const int base = 0) { return ::strtoll (str, strEnd, base); }
inline U32 ToU32(const Char* str, Char** strEnd = Null, const int base = 0) { return ::strtoul (str, strEnd, base); }
inline U64 ToU64(const Char* str, Char** strEnd = Null, const int base = 0) { return ::strtoull(str, strEnd, base); }
inline F32 ToF32(const Char* str, Char** strEnd = Null                    ) { return ::strtof  (str, strEnd      ); }
inline F64 ToF64(const Char* str, Char** strEnd = Null                    ) { return ::strtod  (str, strEnd      ); }
inline I32 ToI32(const Str& str, size_t* idx = Null, const int base = 0   ) { Char* strEnd = Null; const auto v = ::strtol  (str.c_str(), idx ? &strEnd : Null, base); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }
inline I64 ToI64(const Str& str, size_t* idx = Null, const int base = 0   ) { Char* strEnd = Null; const auto v = ::strtoll (str.c_str(), idx ? &strEnd : Null, base); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }
inline U32 ToU32(const Str& str, size_t* idx = Null, const int base = 0   ) { Char* strEnd = Null; const auto v = ::strtoul (str.c_str(), idx ? &strEnd : Null, base); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }
inline U64 ToU64(const Str& str, size_t* idx = Null, const int base = 0   ) { Char* strEnd = Null; const auto v = ::strtoull(str.c_str(), idx ? &strEnd : Null, base); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }
inline F32 ToF32(const Str& str, size_t* idx = Null                       ) { Char* strEnd = Null; const auto v = ::strtof  (str.c_str(), idx ? &strEnd : Null      ); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }
inline F64 ToF64(const Str& str, size_t* idx = Null                       ) { Char* strEnd = Null; const auto v = ::strtod  (str.c_str(), idx ? &strEnd : Null      ); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }

inline I32 ToI32(const WChar* str, WChar** strEnd = Null, const int base = 0) { return ::wcstol  (str, strEnd, base); }
inline I64 ToI64(const WChar* str, WChar** strEnd = Null, const int base = 0) { return ::wcstoll (str, strEnd, base); }
inline U32 ToU32(const WChar* str, WChar** strEnd = Null, const int base = 0) { return ::wcstoul (str, strEnd, base); }
inline U64 ToU64(const WChar* str, WChar** strEnd = Null, const int base = 0) { return ::wcstoull(str, strEnd, base); }
inline F32 ToF32(const WChar* str, WChar** strEnd = Null                    ) { return ::wcstof  (str, strEnd      ); }
inline F64 ToF64(const WChar* str, WChar** strEnd = Null                    ) { return ::wcstod  (str, strEnd      ); }
inline I32 ToI32(const WStr& str, size_t* idx = Null, const int base = 0    ) { WChar* strEnd = Null; const auto v = ::wcstol  (str.c_str(), idx ? &strEnd : Null, base); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }
inline I64 ToI64(const WStr& str, size_t* idx = Null, const int base = 0    ) { WChar* strEnd = Null; const auto v = ::wcstoll (str.c_str(), idx ? &strEnd : Null, base); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }
inline U32 ToU32(const WStr& str, size_t* idx = Null, const int base = 0    ) { WChar* strEnd = Null; const auto v = ::wcstoul (str.c_str(), idx ? &strEnd : Null, base); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }
inline U64 ToU64(const WStr& str, size_t* idx = Null, const int base = 0    ) { WChar* strEnd = Null; const auto v = ::wcstoull(str.c_str(), idx ? &strEnd : Null, base); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }
inline F32 ToF32(const WStr& str, size_t* idx = Null                        ) { WChar* strEnd = Null; const auto v = ::wcstof  (str.c_str(), idx ? &strEnd : Null      ); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }
inline F64 ToF64(const WStr& str, size_t* idx = Null                        ) { WChar* strEnd = Null; const auto v = ::wcstod  (str.c_str(), idx ? &strEnd : Null      ); if(idx) *idx = (size_t)(strEnd - str.c_str()); return v; }

#ifdef XTD_PLATFORM_WINDOWS
inline  I32 ToI32(const Char16* str, Char16** strEnd = Null, const int base = 0) { return ::wcstol  ((const WChar*)str, (WChar**)strEnd, base); }
inline  I64 ToI64(const Char16* str, Char16** strEnd = Null, const int base = 0) { return ::wcstoll ((const WChar*)str, (WChar**)strEnd, base); }
inline  U32 ToU32(const Char16* str, Char16** strEnd = Null, const int base = 0) { return ::wcstoul ((const WChar*)str, (WChar**)strEnd, base); }
inline  U64 ToU64(const Char16* str, Char16** strEnd = Null, const int base = 0) { return ::wcstoull((const WChar*)str, (WChar**)strEnd, base); }
inline  F32 ToF32(const Char16* str, Char16** strEnd = Null                    ) { return ::wcstof  ((const WChar*)str, (WChar**)strEnd      ); }
inline  F64 ToF64(const Char16* str, Char16** strEnd = Null                    ) { return ::wcstod  ((const WChar*)str, (WChar**)strEnd      ); }
XTD_API I32 ToI32(const Char32* str, Char32** strEnd = Null, int base = 0);
XTD_API I64 ToI64(const Char32* str, Char32** strEnd = Null, int base = 0);
XTD_API U32 ToU32(const Char32* str, Char32** strEnd = Null, int base = 0);
XTD_API U64 ToU64(const Char32* str, Char32** strEnd = Null, int base = 0);
XTD_API F32 ToF32(const Char32* str, Char32** strEnd = Null              );
XTD_API F64 ToF64(const Char32* str, Char32** strEnd = Null              );
#else
XTD_API I32 ToI32(const Char16* str, Char16** strEnd = Null, int base = 0);
XTD_API I64 ToI64(const Char16* str, Char16** strEnd = Null, int base = 0);
XTD_API U32 ToU32(const Char16* str, Char16** strEnd = Null, int base = 0);
XTD_API U64 ToU64(const Char16* str, Char16** strEnd = Null, int base = 0);
XTD_API F32 ToF32(const Char16* str, Char16** strEnd = Null              );
XTD_API F64 ToF64(const Char16* str, Char16** strEnd = Null              );
inline  I32 ToI32(const Char32* str, Char32** strEnd = Null, int base = 0) { return ::wcstol  ((const WChar*)str, (WChar**)strEnd, base); }
inline  I64 ToI64(const Char32* str, Char32** strEnd = Null, int base = 0) { return ::wcstoll ((const WChar*)str, (WChar**)strEnd, base); }
inline  U32 ToU32(const Char32* str, Char32** strEnd = Null, int base = 0) { return ::wcstoul ((const WChar*)str, (WChar**)strEnd, base); }
inline  U64 ToU64(const Char32* str, Char32** strEnd = Null, int base = 0) { return ::wcstoull((const WChar*)str, (WChar**)strEnd, base); }
inline  F32 ToF32(const Char32* str, Char32** strEnd = Null              ) { return ::wcstof  ((const WChar*)str, (WChar**)strEnd      ); }
inline  F64 ToF64(const Char32* str, Char32** strEnd = Null              ) { return ::wcstod  ((const WChar*)str, (WChar**)strEnd      ); }
#endif
// clang-format on

END_NAMESPACE_XTD
