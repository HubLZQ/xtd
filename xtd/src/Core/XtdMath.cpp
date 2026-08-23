#include <iomanip>
#include <sstream>
#include "xtd/XtdMath.h"
#include "xtd/XtdUnicode.h"

BEGIN_NAMESPACE_XTD

namespace
{
template<typename TRet, typename TStr>
struct STo final
{
};
// clang-format off
template<> struct STo<I32, Char > final { static I32 Convert(const Char*  str, Char**  strEnd, const int base) { return ::strtol  (str, strEnd, base); } };
template<> struct STo<I64, Char > final { static I64 Convert(const Char*  str, Char**  strEnd, const int base) { return ::strtoll (str, strEnd, base); } };
template<> struct STo<U32, Char > final { static U32 Convert(const Char*  str, Char**  strEnd, const int base) { return ::strtoul (str, strEnd, base); } };
template<> struct STo<U64, Char > final { static U64 Convert(const Char*  str, Char**  strEnd, const int base) { return ::strtoull(str, strEnd, base); } };
template<> struct STo<F32, Char > final { static F32 Convert(const Char*  str, Char**  strEnd, const int     ) { return ::strtof  (str, strEnd); } };
template<> struct STo<F64, Char > final { static F64 Convert(const Char*  str, Char**  strEnd, const int     ) { return ::strtod  (str, strEnd); } };
template<> struct STo<I32, WChar> final { static I32 Convert(const WChar* str, WChar** strEnd, const int base) { return ::wcstol  (str, strEnd, base); } };
template<> struct STo<I64, WChar> final { static I64 Convert(const WChar* str, WChar** strEnd, const int base) { return ::wcstoll (str, strEnd, base); } };
template<> struct STo<U32, WChar> final { static U32 Convert(const WChar* str, WChar** strEnd, const int base) { return ::wcstoul (str, strEnd, base); } };
template<> struct STo<U64, WChar> final { static U64 Convert(const WChar* str, WChar** strEnd, const int base) { return ::wcstoull(str, strEnd, base); } };
template<> struct STo<F32, WChar> final { static F32 Convert(const WChar* str, WChar** strEnd, const int     ) { return ::wcstof  (str, strEnd); } };
template<> struct STo<F64, WChar> final { static F64 Convert(const WChar* str, WChar** strEnd, const int     ) { return ::wcstod  (str, strEnd); } };
// clang-format on

template<typename TRet, typename T, typename TCharCvt>
TRet StrToT(const T* str, T** strEnd, int base, const std::basic_string<TCharCvt>& strCvt)
{
    TCharCvt*  strEndTmp = nullptr;
    const TRet result    = STo<TRet, TCharCvt>::Convert(strCvt.c_str(), &strEndTmp, base);
    if(strEnd)
        *strEnd = const_cast<T*>(str + std::distance(strCvt.c_str(), static_cast<const TCharCvt*>(strEndTmp)));
    return result;
}
}// namespace

#ifdef XTD_PLATFORM_WINDOWS

namespace
{
template<typename TRet>
TRet ToTImpl(const Char32* str, Char32** strEnd, const int base)
{
    WStr strTmp;
    text::Utf32ToWStr(str, -1, strTmp);
    return StrToT<TRet, Char32, WChar>(str, strEnd, base, strTmp);
}
}// namespace

I32 ToI32(const Char32* str, Char32** strEnd, const int base) { return ToTImpl<I32>(str, strEnd, base); }
I64 ToI64(const Char32* str, Char32** strEnd, const int base) { return ToTImpl<I64>(str, strEnd, base); }
U32 ToU32(const Char32* str, Char32** strEnd, const int base) { return ToTImpl<U32>(str, strEnd, base); }
U64 ToU64(const Char32* str, Char32** strEnd, const int base) { return ToTImpl<U64>(str, strEnd, base); }
F32 ToF32(const Char32* str, Char32** strEnd) { return ToTImpl<F32>(str, strEnd, 0); }
F64 ToF64(const Char32* str, Char32** strEnd) { return ToTImpl<F64>(str, strEnd, 0); }

#else// other platforms

namespace
{
template<typename TRet>
TRet ToTImpl(const Char16* str, Char16** strEnd, const int base)
{
    WStr strTmp;
    text::Utf16ToWStr(str, -1, strTmp);
    return StrToT<TRet, Char16, WChar>(str, strEnd, base, strTmp);
}
}// namespace

I32 ToI32(const Char16* str, Char16** strEnd, const int base) { return ToTImpl<I32>(str, strEnd, base); }
I64 ToI64(const Char16* str, Char16** strEnd, const int base) { return ToTImpl<I64>(str, strEnd, base); }
U32 ToU32(const Char16* str, Char16** strEnd, const int base) { return ToTImpl<U32>(str, strEnd, base); }
U64 ToU64(const Char16* str, Char16** strEnd, const int base) { return ToTImpl<U64>(str, strEnd, base); }
F32 ToF32(const Char16* str, Char16** strEnd) { return ToTImpl<F32>(str, strEnd, 0); }
F64 ToF64(const Char16* str, Char16** strEnd) { return ToTImpl<F64>(str, strEnd, 0); }

#endif// #ifdef XTD_PLATFORM_WINDOWS

std::string ToStr(float value, int precision, bool scientific, bool upperCase)
{
    char        buf[64]{};
    const char* fmt = scientific ? (upperCase ? "%.*E" : "%.*e") : "%.*f";
    (void)snprintf(buf, sizeof(buf), fmt, precision, value);// NOLINT(clang-diagnostic-format-nonliteral)
    return std::string{buf};
}

std::string ToStr(double value, int precision, bool scientific, bool upperCase)
{
    char        buf[64]{};
    const char* fmt = scientific ? (upperCase ? "%.*E" : "%.*e") : "%.*f";
    (void)snprintf(buf, sizeof(buf), fmt, precision, value);// NOLINT(clang-diagnostic-format-nonliteral)
    return std::string{buf};
}

std::string ToStr(long double value, int precision, bool scientific, bool upperCase)
{
    char        buf[64]{};
    const char* fmt = scientific ? (upperCase ? "%.*E" : "%.*e") : "%.*f";
    (void)snprintf(buf, sizeof(buf), fmt, precision, value);// NOLINT(clang-diagnostic-format-nonliteral)
    return std::string{buf};
}

std::wstring ToWStr(float value, int precision, bool scientific, bool upperCase)
{
    wchar_t        buf[64]{};
    const wchar_t* fmt = scientific ? (upperCase ? L"%.*E" : L"%.*e") : L"%.*f";// NOLINT(clang-diagnostic-format-nonliteral)
    (void)swprintf(buf, sizeof(buf), fmt, precision, value);
    return std::wstring{buf};
}

std::wstring ToWStr(double value, int precision, bool scientific, bool upperCase)
{
    wchar_t        buf[64]{};
    const wchar_t* fmt = scientific ? (upperCase ? L"%.*E" : L"%.*e") : L"%.*f";// NOLINT(clang-diagnostic-format-nonliteral)
    (void)swprintf(buf, sizeof(buf), fmt, precision, value);
    return std::wstring{buf};
}

std::wstring ToWStr(long double value, int precision, bool scientific, bool upperCase)
{
    wchar_t        buf[64]{};
    const wchar_t* fmt = scientific ? (upperCase ? L"%.*E" : L"%.*e") : L"%.*f";// NOLINT(clang-diagnostic-format-nonliteral)
    (void)swprintf(buf, sizeof(buf), fmt, precision, value);
    return std::wstring{buf};
}

END_NAMESPACE_XTD
