#pragma once
#include "XtdDefine.h"

BEGIN_NAMESPACE_XTD

namespace detail
{
template<typename T, size_t N>
class StringImpl final
{
public:
    StringImpl() = default;

private:
    size_t m_size;
    T      m_data[N / sizeof(T)];
};
}

//template<size_t N> using StringA   = detail::StringImpl<char    , N>;
//template<size_t N> using StringW   = detail::StringImpl<wchar_t , N>;
//template<size_t N> using StringU8  = detail::StringImpl<char8_t , N>;
//template<size_t N> using StringU16 = detail::StringImpl<char16_t, N>;
//template<size_t N> using StringU32 = detail::StringImpl<char32_t, N>;

END_NAMESPACE_XTD
