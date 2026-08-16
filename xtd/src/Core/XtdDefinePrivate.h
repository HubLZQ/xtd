#pragma once
#include "xtd/XtdDefine.h"

using ssize_t = std::make_signed_t<std::size_t>;

#if 0

namespace std
{
#ifndef XTD_CPP_17
template<class C>
constexpr auto size(const C& c) noexcept(noexcept(c.size()))
    -> decltype(c.size())
{
    return c.size();
}

template<class T, std::size_t N>
constexpr std::size_t size(const T (&array)[N]) noexcept
{
    return N;
}
#endif

#ifndef XTD_CPP_20
template<class C>
constexpr auto ssize(const C& c) noexcept(noexcept(c.size()))
    -> std::common_type_t<std::ptrdiff_t,
                          std::make_signed_t<decltype(c.size())>>
{
    using R = std::common_type_t<std::ptrdiff_t,
                                 std::make_signed_t<decltype(c.size())>>;
    return static_cast<R>(c.size());
}

template<class T, std::ptrdiff_t N>
constexpr std::ptrdiff_t ssize(const T (&array)[N]) noexcept
{
    return N;
}
#endif
}

#ifdef XTD_CPP_17
using byte = ::std::byte;
#else
using byte = unsigned char;
#endif

#ifdef XTD_CPP_20
using u8str = ::std::u8string;
#else
using char8_t = char;
using u8str   = ::std::string;
#endif
using u16str = ::std::u16string;
using u32str = ::std::u32string;

#ifdef XTD_CPP_20
using u8sv = ::std::u8string_view;
#else
using u8sv = ::std::string_view;
#endif
#ifndef XTD_CPP_17
using u16sv = ::std::u16string_view;
using u32sv = ::std::u32string_view;
#endif

template<typename T, typename TDeleter = ::std::default_delete<T>>
using UPtr = ::std::unique_ptr<T, TDeleter>;

template<typename T>
using SPtr = ::std::shared_ptr<T>;

template<typename T>
using WPtr = ::std::weak_ptr<T>;

#endif