#include <xtd/XtdUnicode.h>

#include "../Platform/Windows/WinAPI.h"

BEGIN_NAMESPACE_XTD

namespace text
{
namespace detail
{
void AnsiToWide(const Char* str, size_t len, WStr& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
#ifdef XTD_PLATFORM_WINDOWS
    const int lenNeed = windows::MultiByteToWideChar(windows::GetAcpCodePage(), 0, str, static_cast<int>(len), nullptr, 0);
    output.resize(lenNeed);
    windows::MultiByteToWideChar(windows::GetAcpCodePage(), 0, str, static_cast<int>(len), const_cast<WChar*>(output.data()), lenNeed);
#else
    conv<utf8, utfw>(str, str + len, std::back_inserter(output));
#endif
}

void AnsiToUtf8(const Char* str, size_t len, U8Str& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
#ifdef XTD_PLATFORM_WINDOWS
    WStr strW;
    AnsiToWide(str, len, strW);
    conv<utfw, utf8>(strW.c_str(), strW.c_str() + strW.size(), std::back_inserter(output));
#else
    output.assign((const Char8*)str, len);
#endif
}

void AnsiToUtf16(const Char* str, size_t len, U16Str& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
#ifdef XTD_PLATFORM_WINDOWS
    WStr strW;
    AnsiToWide(str, len, strW);
    conv<utfw, utf16>(reinterpret_cast<const utfw::char_type*>(strW.data()),
                      reinterpret_cast<const utfw::char_type*>(strW.data()) + strW.size(),
                      std::back_inserter(output));
#else
    conv<utf8, utf16>(reinterpret_cast<const utf8::char_type*>(str),
                      reinterpret_cast<const utf8::char_type*>(str) + len,
                      std::back_inserter(output));
#endif
}

void AnsiToUtf32(const Char* str, size_t len, U32Str& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
#ifdef XTD_PLATFORM_WINDOWS
    WStr strW;
    AnsiToWide(str, len, strW);
    conv<utfw, utf32>(reinterpret_cast<const utfw::char_type*>(strW.data()),
                      reinterpret_cast<const utfw::char_type*>(strW.data()) + strW.size(),
                      std::back_inserter(output));
#else
    conv<utf8, utf32>(reinterpret_cast<const utf8::char_type*>(str),
                      reinterpret_cast<const utf8::char_type*>(str) + len,
                      std::back_inserter(output));
#endif
}

void WideToAnsi(const WChar* str, size_t len, Str& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
#if XTD_PLATFORM_WINDOWS
    const int lenNeed = windows::WideCharToMultiByte(windows::GetAcpCodePage(), 0, str, static_cast<int>(len), nullptr, 0, nullptr, nullptr);
    output.resize(lenNeed);
    windows::WideCharToMultiByte(windows::GetAcpCodePage(), 0, str, static_cast<int>(len), const_cast<Char*>(output.data()), lenNeed, nullptr, nullptr);
#else
    conv<utfw, utf8>(reinterpret_cast<const utfw::char_type*>(str),
                     reinterpret_cast<const utfw::char_type*>(str) + len,
                     std::back_inserter(output));
#endif
}

void WideToUtf8(const WChar* str, size_t len, U8Str& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
    conv<utfw, utf8>(reinterpret_cast<const utfw::char_type*>(str),
                     reinterpret_cast<const utfw::char_type*>(str) + len,
                     std::back_inserter(output));
}

void WideToUtf16(const WChar* str, size_t len, U16Str& output)
{
    output.clear();
    len = static_cast<int>(StrLen(str, len));
    if(len == 0)
        return;
    conv<utfw, utf16>(reinterpret_cast<const utfw::char_type*>(str),
                      reinterpret_cast<const utfw::char_type*>(str) + len,
                      std::back_inserter(output));
}

void WideToUtf32(const WChar* str, size_t len, U32Str& output)
{
    output.clear();
    len = static_cast<int>(StrLen(str, len));
    if(len == 0)
        return;
    conv<utfw, utf32>(reinterpret_cast<const utfw::char_type*>(str),
                      reinterpret_cast<const utfw::char_type*>(str) + len,
                      std::back_inserter(output));
}

void Utf8ToAnsi(const Char8* str, size_t len, Str& output)
{
    output.clear();
    len = static_cast<int>(StrLen(str, len));
    if(len == 0)
        return;
#ifdef XTD_PLATFORM_WINDOWS
    WStr strW;
    conv<utf8, utfw>(reinterpret_cast<const utf8::char_type*>(str),
                     reinterpret_cast<const utf8::char_type*>(str) + len,
                     std::back_inserter(strW));
    WideToAnsi(strW.data(), static_cast<int>(strW.size()), output);
#else
    output.assign((const Char*)str, len);
#endif
}

void Utf8ToWide(const Char8* str, size_t len, WStr& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
#ifdef XTD_PLATFORM_WINDOWS
    const int lenNeed = windows::MultiByteToWideChar(windows::GetUtf8CodePage(), 0, str, static_cast<int>(len), nullptr, 0);
    output.resize(lenNeed);
    windows::MultiByteToWideChar(windows::GetUtf8CodePage(), 0, str, static_cast<int>(len), const_cast<WChar*>(output.data()), lenNeed);
#else
    conv<utf8, utfw>(reinterpret_cast<const utf8::char_type*>(str),
                     reinterpret_cast<const utf8::char_type*>(str) + len,
                     std::back_inserter(output));
#endif
}

void Utf16ToAnsi(const Char16* str, size_t len, Str& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
#ifdef XTD_PLATFORM_WINDOWS
    WStr strW;
    conv<utf16, utfw>(reinterpret_cast<const utf16::char_type*>(str),
                      reinterpret_cast<const utf16::char_type*>(str) + len,
                      std::back_inserter(strW));
    WideToAnsi(strW.data(), static_cast<int>(strW.size()), output);
#else
    conv<utf16, utf8>(reinterpret_cast<const utf16::char_type*>(str),
                      reinterpret_cast<const utf16::char_type*>(str) + len,
                      std::back_inserter(output));
#endif
}

void Utf16ToWide(const Char16* str, size_t len, WStr& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
    conv<utf16, utfw>(reinterpret_cast<const utf16::char_type*>(str),
                      reinterpret_cast<const utf16::char_type*>(str) + len,
                      std::back_inserter(output));
}

void Utf32ToAnsi(const Char32* str, size_t len, Str& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
#ifdef XTD_PLATFORM_WINDOWS
    WStr strW;
    conv<utf32, utfw>(reinterpret_cast<const utf32::char_type*>(str),
                      reinterpret_cast<const utf32::char_type*>(str) + len,
                      std::back_inserter(strW));
    WideToAnsi(strW.data(), static_cast<int>(strW.size()), output);
#else
    conv<utf32, utf8>(reinterpret_cast<const utf32::char_type*>(str),
                      reinterpret_cast<const utf32::char_type*>(str) + len,
                      std::back_inserter(output));
#endif
}

void Utf32ToWide(const Char32* str, size_t len, WStr& output)
{
    output.clear();
    len = StrLen(str, len);
    if(len == 0)
        return;
    conv<utf32, utfw>(reinterpret_cast<const utf32::char_type*>(str),
                      reinterpret_cast<const utf32::char_type*>(str) + len,
                      std::back_inserter(output));
}
}// namespace text
}// namespace detail

END_NAMESPACE_XTD
