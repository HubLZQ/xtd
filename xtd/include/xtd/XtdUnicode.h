#pragma once
#include <iterator>
#include <stdexcept>

#include "XtdDefine.h"

BEGIN_NAMESPACE_XTD

namespace text
{
namespace detail
{
static constexpr uint32_t max_unicode_code_point = 0x10FFFF;

// Supported combinations:
//   0xxx_xxxx
//   110x_xxxx 10xx_xxxx
//   1110_xxxx 10xx_xxxx 10xx_xxxx
//   1111_0xxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
//   1111_10xx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
//   1111_110x 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
struct utf8 final
{
    static constexpr size_t   max_unicode_symbol_size   = 4;
    static constexpr size_t   max_supported_symbol_size = 6;
    static constexpr uint32_t max_supported_code_point  = 0x7FFFFFFF;

    using char_type = uint8_t;

    template<typename PeekFn>
    static size_t char_size(PeekFn&& peek_fn)
    {
        char_type const ch0 = std::forward<PeekFn>(peek_fn)();
        if(ch0 < 0x80)// 0xxx_xxxx
            return 1;
        if(ch0 < 0xC0)
            throw std::runtime_error("The utf8 first char in sequence is incorrect");
        if(ch0 < 0xE0)// 110x_xxxx 10xx_xxxx
            return 2;
        if(ch0 < 0xF0)// 1110_xxxx 10xx_xxxx 10xx_xxxx
            return 3;
        if(ch0 < 0xF8)// 1111_0xxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
            return 4;
        if(ch0 < 0xFC)// 1111_10xx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
            return 5;
        if(ch0 < 0xFE)// 1111_110x 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
            return 6;
        throw std::runtime_error("The utf8 first char in sequence is incorrect");
    }

    template<typename ReadFn>
    static uint32_t read(ReadFn&& read_fn)
    {
        char_type const ch0 = read_fn();
        if(ch0 < 0x80)// 0xxx_xxxx
            return ch0;
        if(ch0 < 0xC0)
            throw std::runtime_error("The utf8 first char in sequence is incorrect");
        if(ch0 < 0xE0)// 110x_xxxx 10xx_xxxx
        {
            // clang-format off
            char_type const ch1 = read_fn(); if(ch1 >> 6 != 2) goto _err;
            // clang-format on
            return (ch0 << 6) + ch1 - 0x3080;
        }
        if(ch0 < 0xF0)// 1110_xxxx 10xx_xxxx 10xx_xxxx
        {
            // clang-format off
            char_type const ch1 = read_fn(); if(ch1 >> 6 != 2) goto _err;
            char_type const ch2 = read_fn(); if(ch2 >> 6 != 2) goto _err;
            // clang-format on
            return (ch0 << 12) + (ch1 << 6) + ch2 - 0xE2080;
        }
        if(ch0 < 0xF8)// 1111_0xxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            // clang-format off
            char_type const ch1 = read_fn(); if(ch1 >> 6 != 2) goto _err;
            char_type const ch2 = read_fn(); if(ch2 >> 6 != 2) goto _err;
            char_type const ch3 = read_fn(); if(ch3 >> 6 != 2) goto _err;
            // clang-format on
            return (ch0 << 18) + (ch1 << 12) + (ch2 << 6) + ch3 - 0x3C82080;
        }
        if(ch0 < 0xFC)// 1111_10xx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            // clang-format off
            char_type const ch1 = read_fn(); if(ch1 >> 6 != 2) goto _err;
            char_type const ch2 = read_fn(); if(ch2 >> 6 != 2) goto _err;
            char_type const ch3 = read_fn(); if(ch3 >> 6 != 2) goto _err;
            char_type const ch4 = read_fn(); if(ch4 >> 6 != 2) goto _err;
            // clang-format on
            return (ch0 << 24) + (ch1 << 18) + (ch2 << 12) + (ch3 << 6) + ch4 - 0xFA082080;
        }
        if(ch0 < 0xFE)// 1111_110x 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            // clang-format off
            char_type const ch1 = read_fn(); if(ch1 >> 6 != 2) goto _err;
            char_type const ch2 = read_fn(); if(ch2 >> 6 != 2) goto _err;
            char_type const ch3 = read_fn(); if(ch3 >> 6 != 2) goto _err;
            char_type const ch4 = read_fn(); if(ch4 >> 6 != 2) goto _err;
            char_type const ch5 = read_fn(); if(ch5 >> 6 != 2) goto _err;
            // clang-format on
            return (ch0 << 30) + (ch1 << 24) + (ch2 << 18) + (ch3 << 12) + (ch4 << 6) + ch5 - 0x82082080;
        }
        throw std::runtime_error("The utf8 first char in sequence is incorrect");
_err:
        throw std::runtime_error("The utf8 slave char in sequence is incorrect");
    }

    template<typename WriteFn>
    static void write(uint32_t const cp, WriteFn&& write_fn)
    {
        if(cp < 0x80)// 0xxx_xxxx
            write_fn(static_cast<char_type>(cp));
        else if(cp < 0x800)// 110x_xxxx 10xx_xxxx
        {
            write_fn(static_cast<char_type>(0xC0 | cp >> 6));
            goto _1;
        }
        else if(cp < 0x10000)// 1110_xxxx 10xx_xxxx 10xx_xxxx
        {
            write_fn(static_cast<char_type>(0xE0 | cp >> 12));
            goto _2;
        }
        else if(cp < 0x200000)// 1111_0xxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            write_fn(static_cast<char_type>(0xF0 | cp >> 18));
            goto _3;
        }
        else if(cp < 0x4000000)// 1111_10xx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            write_fn(static_cast<char_type>(0xF8 | cp >> 24));
            goto _4;
        }
        else if(cp < 0x80000000)// 1111_110x 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx 10xx_xxxx
        {
            write_fn(static_cast<char_type>(0xFC | cp >> 30));
            goto _5;
        }
        else
            throw std::runtime_error("Tool large UTF8 code point");
        return;
        // clang-format off
        _5: write_fn(static_cast<char_type>(0x80 | (cp >> 24 & 0x3F)));
        _4: write_fn(static_cast<char_type>(0x80 | (cp >> 18 & 0x3F)));
        _3: write_fn(static_cast<char_type>(0x80 | (cp >> 12 & 0x3F)));
        _2: write_fn(static_cast<char_type>(0x80 | (cp >>  6 & 0x3F)));
        _1: write_fn(static_cast<char_type>(0x80 | (cp       & 0x3F)));
        // clang-format on
    }
};

//                1         0
//       98765432109876543210
//       ||||||||||||||||||||
// 110110xxxxxxxxxx|||||||||| high surrogate
//           110111xxxxxxxxxx low  surrogate
struct utf16 final
{
    static constexpr size_t   max_unicode_symbol_size   = 2;
    static constexpr size_t   max_supported_symbol_size = max_unicode_symbol_size;
    static constexpr uint32_t max_supported_code_point  = 0x10FFFF;

    using char_type = uint16_t;

    static constexpr char_type min_surrogate      = 0xD800;
    static constexpr char_type max_surrogate      = 0xDFFF;
    static constexpr char_type min_surrogate_high = 0xD800;
    static constexpr char_type max_surrogate_high = 0xDBFF;
    static constexpr char_type min_surrogate_low  = 0xDC00;
    static constexpr char_type max_surrogate_low  = 0xDFFF;

    template<typename PeekFn>
    static size_t char_size(PeekFn&& peek_fn)
    {
        char_type const ch0 = std::forward<PeekFn>(peek_fn)();
        if(ch0 < 0xD800)// [0x0000‥0xD7FF]
            return 1;
        if(ch0 < 0xDC00)// [0xD800‥0xDBFF] [0xDC00‥0xDFFF]
            return 2;
        if(ch0 < 0xE000)
            throw std::runtime_error("The high utf16 surrogate char is expected");
        // [0xE000‥0xFFFF]
        return 1;
    }

    template<typename ReadFn>
    static uint32_t read(ReadFn&& read_fn)
    {
        char_type const ch0 = read_fn();
        if(ch0 < 0xD800)// [0x0000‥0xD7FF]
            return ch0;
        if(ch0 < 0xDC00)// [0xD800‥0xDBFF] [0xDC00‥0xDFFF]
        {
            char_type const ch1 = read_fn();
            if(ch1 >> 10 != 0x37)
                throw std::runtime_error("The low utf16 surrogate char is expected");
            return static_cast<uint32_t>((ch0 << 10) + ch1 - 0x35FDC00);
        }
        if(ch0 < 0xE000)
            throw std::runtime_error("The high utf16 surrogate char is expected");
        // [0xE000‥0xFFFF]
        return ch0;
    }

    template<typename WriteFn>
    static void write(uint32_t const cp, WriteFn&& write_fn)
    {
        if(cp < 0xD800)// [0x0000‥0xD7FF]
            write_fn(static_cast<char_type>(cp));
        else if(cp < 0x10000)
        {
            if(cp < 0xE000)
                throw std::runtime_error("The utf16 code point can not be in surrogate range");
            // [0xE000‥0xFFFF]
            write_fn(static_cast<char_type>(cp));
        }
        else if(cp < 0x110000)// [0xD800‥0xDBFF] [0xDC00‥0xDFFF]
        {
            write_fn(static_cast<char_type>(0xD7C0 + (cp >> 10)));
            write_fn(static_cast<char_type>(0xDC00 + (cp & 0x3FF)));
        }
        else
            throw std::runtime_error("Too large the utf16 code point");
    }
};

struct utf32 final
{
    static constexpr size_t   max_unicode_symbol_size   = 1;
    static constexpr size_t   max_supported_symbol_size = 1;
    static constexpr uint32_t max_supported_code_point  = 0x7FFFFFFF;

    using char_type = uint32_t;

    template<typename PeekFn>
    static size_t char_size(PeekFn&&)
    {
        return 1;
    }

    template<typename ReadFn>
    static uint32_t read(ReadFn&& read_fn)
    {
        char_type const ch = std::forward<ReadFn>(read_fn)();
        if(ch < 0x80000000)
            return ch;
        throw std::runtime_error("Too large utf32 char");
    }

    template<typename WriteFn>
    static void write(uint32_t const cp, WriteFn&& write_fn)
    {
        if(cp < 0x80000000)
            std::forward<WriteFn>(write_fn)(static_cast<char_type>(cp));
        else
            throw std::runtime_error("Too large utf32 code point");
    }
};

#ifdef XTD_PLATFORM_WINDOWS
using utfw = utf16;
#else
using utfw = utf32;
#endif

namespace detail_utf
{
template<typename Ch>
struct utf_selector final
{
};
// clang-format off
template<> struct utf_selector<         char> final { using type = utf8 ; };
template<> struct utf_selector<unsigned char> final { using type = utf8 ; };
template<> struct utf_selector<signed   char> final { using type = utf8 ; };
template<> struct utf_selector<char16_t     > final { using type = utf16; };
template<> struct utf_selector<char32_t     > final { using type = utf32; };
template<> struct utf_selector<wchar_t      > final { using type = utfw ; };
// clang-format on
}// namespace detail_utf

template<typename Ch>
using utf_selector = detail_utf::utf_selector<std::decay_t<Ch>>;

template<typename Ch>
using utf_selector_t = typename utf_selector<Ch>::type;

#ifdef XTD_CPP_17
#include <string_view>
#endif

namespace detail_utf
{
enum struct convz_impl
{
    normal,
    binary_copy
};

template<
    typename Utf,
    typename Outf,
    typename It,
    typename Oit,
    convz_impl>
struct convz_strategy
{
    Oit operator()(It it, Oit oit) const
    {
        auto const read_fn = [&it]
        { return *it++; };
        auto const write_fn = [&oit](typename Outf::char_type const ch)
        { *oit++ = ch; };
        while(true)
        {
            auto const cp = Utf::read(read_fn);
            if(!cp)
                return oit;
            Outf::write(cp, write_fn);
        }
    }
};

template<
    typename Utf,
    typename Outf,
    typename It,
    typename Oit>
struct convz_strategy<Utf, Outf, It, Oit, convz_impl::binary_copy>
{
    Oit operator()(It it, Oit oit) const
    {
        while(true)
        {
            auto const ch = *it++;
            if(!ch)
                return oit;
            *oit++ = ch;
        }
    }
};
}// namespace detail_utf

template<
    typename Utf,
    typename Outf,
    typename It,
    typename Oit>
Oit convz(It&& it, Oit&& oit)
{
    return detail_utf::convz_strategy<Utf, Outf,
                                      std::decay_t<It>,
                                      std::decay_t<Oit>,
                                      std::is_same<Utf, Outf>::value
                                          ? detail_utf::convz_impl::binary_copy
                                          : detail_utf::convz_impl::normal>()(std::forward<It>(it), std::forward<Oit>(oit));
}

namespace detail_utf
{
enum struct conv_impl
{
    normal,
    random_interator,
    binary_copy
};

template<
    typename Utf,
    typename Outf,
    typename It,
    typename Oit,
    conv_impl>
struct conv_strategy final
{
    Oit operator()(It it, It const eit, Oit oit) const
    {
        auto const read_fn = [&it, &eit]
        {
            if(it == eit)
                throw std::runtime_error("Not enough input");
            return *it++;
        };
        auto const write_fn = [&oit](typename Outf::char_type const ch)
        { *oit++ = ch; };
        while(it != eit)
            Outf::write(Utf::read(read_fn), write_fn);
        return oit;
    }
};

template<
    typename Utf,
    typename Outf,
    typename It,
    typename Oit>
struct conv_strategy<Utf, Outf, It, Oit, conv_impl::random_interator> final
{
    Oit operator()(It it, It const eit, Oit oit) const
    {
        auto const write_fn = [&oit](typename Outf::char_type const ch)
        { *oit++ = ch; };
        if(eit - it >= static_cast<typename std::iterator_traits<It>::difference_type>(Utf::max_supported_symbol_size))
        {
            auto const fast_read_fn = [&it]
            { return *it++; };
            auto const fast_eit = eit - Utf::max_supported_symbol_size;
            while(it < fast_eit)
                Outf::write(Utf::read(fast_read_fn), write_fn);
        }
        auto const read_fn = [&it, &eit]
        {
            if(it == eit)
                throw std::runtime_error("Not enough input");
            return *it++;
        };
        while(it != eit)
            Outf::write(Utf::read(read_fn), write_fn);
        return oit;
    }
};

template<
    typename Utf,
    typename Outf,
    typename It,
    typename Oit>
struct conv_strategy<Utf, Outf, It, Oit, conv_impl::binary_copy> final
{
    Oit operator()(It it, It const eit, Oit oit) const
    {
        while(it != eit)
            *oit++ = *it++;
        return oit;
    }
};
}// namespace detail_utf

template<
    typename Utf,
    typename Outf,
    typename It,
    typename Eit,
    typename Oit>
Oit conv(It&& it, Eit&& eit, Oit&& oit)
{
    return detail_utf::conv_strategy<Utf, Outf,
                                     std::decay_t<It>,
                                     std::decay_t<Oit>,
                                     std::is_same<Utf, Outf>::value
                                         ? detail_utf::conv_impl::binary_copy
                                     : std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<std::decay_t<It>>::iterator_category>::value
                                         ? detail_utf::conv_impl::random_interator
                                         : detail_utf::conv_impl::normal>()(std::forward<It>(it),
                                                                            std::forward<Eit>(eit),
                                                                            std::forward<Oit>(oit));
}

template<
    typename Outf,
    typename Ch,
    typename Oit>
Oit convz(Ch const* const str, Oit&& oit)
{
    return convz<utf_selector_t<Ch>, Outf>(str, std::forward<Oit>(oit));
}

template<
    typename Och,
    typename Str>
std::basic_string<Och> convz(Str&& str)
{
    std::basic_string<Och> res;
    convz<utf_selector_t<Och>>(std::forward<Str>(str), std::back_inserter(res));
    return res;
}

template<
    typename Outf,
    typename Ch,
    typename Oit>
Oit conv(std::basic_string<Ch> const& str, Oit&& oit)
{
    return conv<utf_selector_t<Ch>, Outf>(str.cbegin(), str.cend(), std::forward<Oit>(oit));
}

#ifdef XTD_CPP_17
template<
    typename Outf,
    typename Ch,
    typename Oit>
Oit conv(std::basic_string_view<Ch> const& str, Oit&& oit)
{
    return conv<utf_selector_t<Ch>, Outf>(str.cbegin(), str.cend(), std::forward<Oit>(oit));
}
#endif

template<
    typename Och,
    typename Str,
    std::enable_if_t<!std::is_same<std::decay_t<Str>, std::basic_string<Och>>::value, void*> = nullptr>
std::basic_string<Och> conv(Str&& str)
{
    std::basic_string<Och> res;
    conv<utf_selector_t<Och>>(std::forward<Str>(str), std::back_inserter(res));
    return res;
}

template<
    typename Ch>
std::basic_string<Ch> conv(std::basic_string<Ch> str) throw()
{
    return str;
}


template<
    typename Utf,
    typename It>
size_t char_size(It it)
{
    return Utf::char_size([&it]
                          { return *it; });
}

template<
    typename Utf,
    typename It>
size_t size(It it)
{
    size_t total_cp = 0;
    while(*it)
    {
        size_t size = Utf::char_size([&it]
                                     { return *it; });
        while(++it, --size > 0)
            if(!*it)
                throw std::runtime_error("Not enough input for the null-terminated string");
        ++total_cp;
    }
    return total_cp;
}

namespace detail_utf
{
enum struct iterator_impl
{
    forward,
    random_access
};

template<
    typename It,
    iterator_impl>
struct next_strategy final
{
    void operator()(It& it, It const& eit, size_t size)
    {
        while(++it, --size > 0)
            if(it == eit)
                throw std::runtime_error("Not enough input for the forward iterator");
    }
};

template<typename It>
struct next_strategy<It, iterator_impl::random_access> final
{
    void operator()(It& it, It const& eit, typename std::iterator_traits<It>::difference_type const size)
    {
        if(eit - it < size)
            throw std::runtime_error("Not enough input for the random access iterator");
        it += size;
    }
};
}// namespace detail_utf

template<
    typename Utf,
    typename It,
    typename Eit>
size_t size(It it, Eit const eit)
{
    size_t total_cp = 0;
    while(it != eit)
    {
        size_t const size = Utf::char_size([&it]
                                           { return *it; });
        detail_utf::next_strategy<
            std::decay_t<It>,
            std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<std::decay_t<It>>::iterator_category>::value
                ? detail_utf::iterator_impl::random_access
                : detail_utf::iterator_impl::forward>()(it, eit, size);
        ++total_cp;
    }
    return total_cp;
}

template<typename Ch, typename Utf = utf_selector_t<Ch>>
size_t size(Ch const* str)
{
    return detail::size<Utf>(str);
}

template<typename Ch, typename Utf = utf_selector_t<Ch>>
size_t size(std::basic_string<Ch> str)
{
    return detail::size<Utf>(str.cbegin(), str.cend());
}

#ifdef XTD_CPP_17
template<typename Ch, typename Utf = utf_selector_t<Ch>>
size_t size(std::basic_string_view<Ch> str)
{
    return detail::size<Utf>(str.cbegin(), str.cend());
}
#endif

inline size_t StrLen(const Char* str)
{
    return !str || !*str
               ? 0
               : std::strlen(str);
}

inline size_t StrLen(const Char* str, size_t len)
{
    return !str || !*str || len == 0
               ? 0
               : (len > 0 && len != static_cast<size_t>(-1)
                      ? len
                      : std::strlen(str));
}

inline size_t StrLen(const WChar* str)
{
    return !str || !*str
               ? 0
               : std::wcslen(str);
}

inline size_t StrLen(const WChar* str, size_t len)
{
    return !str || !*str || len == 0
               ? 0
               : (len > 0 && len != static_cast<size_t>(-1)
                      ? len
                      : std::wcslen(str));
}

template<typename T>
size_t StrLen(const T* str)
{
    if(!str || !*str)
        return 0;
    size_t len = 0;
    while(*str++) ++len;
    return len;
}

template<typename T>
size_t StrLen(const T* str, size_t len)
{
    if(!str || !*str || len == 0)
        return 0;
    if(len > 0 && len != static_cast<size_t>(-1))
        return len;
    len = 0;
    while(*str++) ++len;
    return len;
}

template<typename TSrc, typename TDst, typename TInput, typename TOutput>
void UtfxToUtfy(const TInput* input, size_t len, TOutput& output)
{
    output.clear();
    len = detail::StrLen(input, len);
    detail::conv<TSrc, TDst>(reinterpret_cast<const typename TSrc::char_type*>(input),
                             reinterpret_cast<const typename TSrc::char_type*>(input) + len,
                             std::back_inserter(output));
}

// clang-format off
void AnsiToWide  (const Char*   str, size_t len, WStr&   output);
void AnsiToUtf8  (const Char*   str, size_t len, U8Str&  output);
void AnsiToUtf16 (const Char*   str, size_t len, U16Str& output);
void AnsiToUtf32 (const Char*   str, size_t len, U32Str& output);
void WideToAnsi  (const WChar*  str, size_t len, Str&    output);
void WideToUtf8  (const WChar*  str, size_t len, U8Str&  output);
void WideToUtf16 (const WChar*  str, size_t len, U16Str& output);
void WideToUtf32 (const WChar*  str, size_t len, U32Str& output);
void Utf8ToAnsi  (const Char8*  str, size_t len, Str&    output);
void Utf8ToWide  (const Char8*  str, size_t len, WStr&   output);
void Utf16ToAnsi (const Char16* str, size_t len, Str&    output);
void Utf16ToWide (const Char16* str, size_t len, WStr&   output);
void Utf32ToAnsi (const Char32* str, size_t len, Str&    output);
void Utf32ToWide (const Char32* str, size_t len, WStr&   output);
// clang-format on
}// namespace detail

// clang-format off
inline void   StrToUtf8 (const Char* str, const size_t len, U8Str&  output) { detail::AnsiToUtf8 (str, len, output); }
inline void   StrToUtf16(const Char* str, const size_t len, U16Str& output) { detail::AnsiToUtf16(str, len, output); }
inline void   StrToUtf32(const Char* str, const size_t len, U32Str& output) { detail::AnsiToUtf32(str, len, output); }
inline void   StrToWStr (const Char* str, const size_t len, WStr&   output) { detail::AnsiToWide (str, len, output); }
inline U8Str  StrToUtf8 (const Char* str, const size_t len) { U8Str  ret; detail::AnsiToUtf8 (str, len, ret); return ret; }
inline U16Str StrToUtf16(const Char* str, const size_t len) { U16Str ret; detail::AnsiToUtf16(str, len, ret); return ret; }
inline U32Str StrToUtf32(const Char* str, const size_t len) { U32Str ret; detail::AnsiToUtf32(str, len, ret); return ret; }
inline WStr   StrToWStr (const Char* str, const size_t len) { WStr   ret; detail::AnsiToWide (str, len, ret); return ret; }
// clang-format on

// clang-format off
inline void   WStrToUtf8 (const WChar* str, const size_t len, U8Str&  output) { detail::WideToUtf8 (str, len, output); }
inline void   WStrToUtf16(const WChar* str, const size_t len, U16Str& output) { detail::WideToUtf16(str, len, output); }
inline void   WStrToUtf32(const WChar* str, const size_t len, U32Str& output) { detail::WideToUtf32(str, len, output); }
inline void   WStrToStr  (const WChar* str, const size_t len, Str&    output) { detail::WideToAnsi (str, len, output); }
inline U8Str  WStrToUtf8 (const WChar* str, const size_t len) { U8Str  ret; detail::WideToUtf8 (str, len, ret); return ret; }
inline U16Str WStrToUtf16(const WChar* str, const size_t len) { U16Str ret; detail::WideToUtf16(str, len, ret); return ret; }
inline U32Str WStrToUtf32(const WChar* str, const size_t len) { U32Str ret; detail::WideToUtf32(str, len, ret); return ret; }
inline Str    WStrToStr  (const WChar* str, const size_t len) { Str    ret; detail::WideToAnsi (str, len, ret); return ret; }
// clang-format on

// clang-format off
inline void   Utf8ToUtf16(const Char8* str, const size_t len, U16Str& output) { detail::UtfxToUtfy<detail::utf8, detail::utf16>(str, len, output); }
inline void   Utf8ToUtf32(const Char8* str, const size_t len, U32Str& output) { detail::UtfxToUtfy<detail::utf8, detail::utf32>(str, len, output); }
inline void   Utf8ToStr  (const Char8* str, const size_t len, Str&    output) { detail::Utf8ToAnsi(str, len, output); }
inline void   Utf8ToWStr (const Char8* str, const size_t len, WStr&   output) { detail::Utf8ToWide(str, len, output); }
inline U16Str Utf8ToUtf16(const Char8* str, const size_t len) { U16Str ret; detail::UtfxToUtfy<detail::utf8, detail::utf16>(str, len, ret); return ret; }
inline U32Str Utf8ToUtf32(const Char8* str, const size_t len) { U32Str ret; detail::UtfxToUtfy<detail::utf8, detail::utf32>(str, len, ret); return ret; }
inline Str    Utf8ToStr  (const Char8* str, const size_t len) { Str    ret; detail::Utf8ToAnsi(str, len, ret); return ret; }
inline WStr   Utf8ToWStr (const Char8* str, const size_t len) { WStr   ret; detail::Utf8ToWide(str, len, ret); return ret; }
// clang-format on

// clang-format off
inline void   Utf16ToUtf8 (const Char16* str, const size_t len, U8Str&  output) { detail::UtfxToUtfy<detail::utf16, detail::utf8 >(str, len, output); }
inline void   Utf16ToUtf32(const Char16* str, const size_t len, U32Str& output) { detail::UtfxToUtfy<detail::utf16, detail::utf32>(str, len, output); }
inline void   Utf16ToStr  (const Char16* str, const size_t len, Str&    output) { detail::Utf16ToAnsi(str, len, output); }
inline void   Utf16ToWStr (const Char16* str, const size_t len, WStr&   output) { detail::Utf16ToWide(str, len, output); }
inline U8Str  Utf16ToUtf8 (const Char16* str, const size_t len) { U8Str  ret; detail::UtfxToUtfy<detail::utf16, detail::utf8 >(str, len, ret); return ret; }
inline U32Str Utf16ToUtf32(const Char16* str, const size_t len) { U32Str ret; detail::UtfxToUtfy<detail::utf16, detail::utf32>(str, len, ret); return ret; }
inline Str    Utf16ToStr  (const Char16* str, const size_t len) { Str    ret; detail::Utf16ToAnsi(str, len, ret); return ret; }
inline WStr   Utf16ToWStr (const Char16* str, const size_t len) { WStr   ret; detail::Utf16ToWide(str, len, ret); return ret; }
// clang-format on

// clang-format off
inline void   Utf32ToUtf8 (const Char32* str, const size_t len, U8Str&  output) { detail::UtfxToUtfy<detail::utf32, detail::utf8 >(str, len, output); }
inline void   Utf32ToUtf32(const Char32* str, const size_t len, U32Str& output) { detail::UtfxToUtfy<detail::utf32, detail::utf32>(str, len, output); }
inline void   Utf32ToStr  (const Char32* str, const size_t len, Str&    output) { detail::Utf32ToAnsi(str, len, output); }
inline void   Utf32ToWStr (const Char32* str, const size_t len, WStr&   output) { detail::Utf32ToWide(str, len, output); }
inline U8Str  Utf32ToUtf8 (const Char32* str, const size_t len) { U8Str  ret; detail::UtfxToUtfy<detail::utf32, detail::utf8 >(str, len, ret); return ret; }
inline U32Str Utf32ToUtf32(const Char32* str, const size_t len) { U32Str ret; detail::UtfxToUtfy<detail::utf32, detail::utf32>(str, len, ret); return ret; }
inline Str    Utf32ToStr  (const Char32* str, const size_t len) { Str    ret; detail::Utf32ToAnsi(str, len, ret); return ret; }
inline WStr   Utf32ToWStr (const Char32* str, const size_t len) { WStr   ret; detail::Utf32ToWide(str, len, ret); return ret; }
// clang-format on

}// namespace text

END_NAMESPACE_XTD
