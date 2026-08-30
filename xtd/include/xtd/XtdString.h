#pragma once
#include "XtdDefine.h"

BEGIN_NAMESPACE_XTD
namespace detail
{
constexpr size_t NPos = static_cast<size_t>(-1);

template<typename T>
struct StrTraitBase
{
    using CharType = T;
    using SizeType = size_t;
    using PStr     = CharType*;
    using PCStr    = const CharType*;
    using StdStr   = std::basic_string<CharType>;
};

template<typename T>
struct StrTrait : StrTraitBase<T>
{
};

template<>
struct StrTrait<char> : StrTraitBase<char>
{
    static constexpr PCStr Empty() { return ""; }
};

template<>
struct StrTrait<wchar_t> : StrTraitBase<wchar_t>
{
    static constexpr PCStr Empty() { return L""; }
};

}// namespace detail

template<typename T>
class StrT
{
public:
    using XChar     = T;
    using XTrait    = detail::StrTrait<XChar>;
    using XPStr     = typename XTrait::PStr;
    using XPCStr    = typename XTrait::PCStr;
    using XStdStr   = typename XTrait::StdStrType;
    using XSizeType = typename XTrait::SizeType;

public:
    StrT() = default;
    explicit StrT(XPCStr str = nullptr) : m_str(str ? str : XTrait::Empty()) {}
    explicit StrT(const XStdStr& str) : m_str(str) {}
    explicit StrT(XStdStr&& str) : m_str(std::move(str)) {}

    StrT& assign(std::nullptr_t)
    {
        m_str.clear();
        return *this;
    }

    StrT& assign(const StrT& value)
    {
        m_str = value.m_str;
        return *this;
    }

    StrT& assign(StrT&& value) noexcept
    {
        m_str.assign(std::move(value.m_str));
        return *this;
    }

    StrT& assign(const XStdStr& str, XSizeType pos, XSizeType count)
    {
        m_str.assign(str, pos, count);
        return *this;
    }

    StrT& assign(const StrT& str, XSizeType pos, XSizeType count)
    {
        m_str.assign(str.m_str, pos, count);
        return *this;
    }

    template<typename U>
    StrT& assign(U&& value)
    {
        m_str = std::forward<U>(value);
        return *this;
    }

    StrT& assign(XSizeType count, XChar ch)
    {
        m_str.assign(count, ch);
        return *this;
    }

    StrT& assign(XPCStr str, XSizeType count)
    {
        m_str.assign(str, count);
        return *this;
    }

    template<typename TInput>
    StrT& assign(TInput first, TInput last)
    {
        m_str.assign(first, last);
        return *this;
    }

    StrT& assign(std::initializer_list<XChar> ilist)
    {
        m_str.assign(ilist);
        return *this;
    }

#ifdef XTD_CPP_17
    template<typename SV>
    StrT& assign(const SV& t, XSizeType pos, XSizeType count = detail::NPos)
    {
        m_str.assign(t, pos, count);
        return *this;
    }
#endif

    StrT& operator=(std::nullptr_t)
    {
        m_str.clear();
        return *this;
    }

    StrT& operator=(XPCStr value)
    {
        m_str = value ? value : XTrait::Empty();
        return *this;
    }

    StrT& operator=(const StrT& value)
    {
        m_str = value.m_str;
        return *this;
    }

    StrT& operator=(StrT&& value) noexcept
    {
        m_str = std::move(value.m_str);
        return *this;
    }

    template<typename U>
    StrT& operator=(U&& value)
    {
        m_str = std::forward<U>(value);
        return *this;
    }

    XChar& operator[](XSizeType pos)
    {
        XTD_ASSERT(pos < m_str.length());
        return m_str[pos];
    }

    const XChar& operator[](XSizeType pos) const
    {
        XTD_ASSERT(pos < m_str.length());
        return m_str[pos];
    }

    // clang-format off

    bool operator==(const StrT& lhs, const StrT& rhs) const { return lhs.m_str == rhs.m_str; }
    bool operator!=(const StrT& lhs, const StrT& rhs) const { return lhs.m_str != rhs.m_str; }
    bool operator> (const StrT& lhs, const StrT& rhs) const { return lhs.m_str >  rhs.m_str; }
    bool operator< (const StrT& lhs, const StrT& rhs) const { return lhs.m_str <  rhs.m_str; }
    bool operator<=(const StrT& lhs, const StrT& rhs) const { return lhs.m_str <= rhs.m_str; }
    bool operator>=(const StrT& lhs, const StrT& rhs) const { return lhs.m_str >= rhs.m_str; }

    bool operator==(XPCStr lhs, const StrT& rhs) const { return lhs == rhs.m_str; }
    bool operator!=(XPCStr lhs, const StrT& rhs) const { return lhs != rhs.m_str; }
    bool operator> (XPCStr lhs, const StrT& rhs) const { return lhs >  rhs.m_str; }
    bool operator< (XPCStr lhs, const StrT& rhs) const { return lhs <  rhs.m_str; }
    bool operator<=(XPCStr lhs, const StrT& rhs) const { return lhs <= rhs.m_str; }
    bool operator>=(XPCStr lhs, const StrT& rhs) const { return lhs >= rhs.m_str; }

    bool operator==(const StrT& lhs, XPCStr rhs) const { return lhs.m_str == rhs; }
    bool operator!=(const StrT& lhs, XPCStr rhs) const { return lhs.m_str != rhs; }
    bool operator> (const StrT& lhs, XPCStr rhs) const { return lhs.m_str >  rhs; }
    bool operator< (const StrT& lhs, XPCStr rhs) const { return lhs.m_str <  rhs; }
    bool operator<=(const StrT& lhs, XPCStr rhs) const { return lhs.m_str <= rhs; }
    bool operator>=(const StrT& lhs, XPCStr rhs) const { return lhs.m_str >= rhs; }

    bool operator==(const XStdStr& lhs, const StrT& rhs) const { return lhs == rhs.m_str; }
    bool operator!=(const XStdStr& lhs, const StrT& rhs) const { return lhs != rhs.m_str; }
    bool operator> (const XStdStr& lhs, const StrT& rhs) const { return lhs >  rhs.m_str; }
    bool operator< (const XStdStr& lhs, const StrT& rhs) const { return lhs <  rhs.m_str; }
    bool operator<=(const XStdStr& lhs, const StrT& rhs) const { return lhs <= rhs.m_str; }
    bool operator>=(const XStdStr& lhs, const StrT& rhs) const { return lhs >= rhs.m_str; }

    bool operator==(const StrT& lhs, const XStdStr& rhs) const { return lhs.m_str == rhs; }
    bool operator!=(const StrT& lhs, const XStdStr& rhs) const { return lhs.m_str != rhs; }
    bool operator> (const StrT& lhs, const XStdStr& rhs) const { return lhs.m_str >  rhs; }
    bool operator< (const StrT& lhs, const XStdStr& rhs) const { return lhs.m_str <  rhs; }
    bool operator<=(const StrT& lhs, const XStdStr& rhs) const { return lhs.m_str <= rhs; }
    bool operator>=(const StrT& lhs, const XStdStr& rhs) const { return lhs.m_str >= rhs; }

    // clang-format on

#ifdef XTD_CPP_20
    // Three-way comparison operator
    // clang-format off
    operator<=>(const StrT&    lhs, const StrT&    rhs) const { return lhs.m_str <=> rhs.m_str; }
    operator<=>(XPCStr         lhs, const StrT&    rhs) const { return lhs       <=> rhs.m_str; }
    operator<=>(const StrT&    lhs, XPCStr         rhs) const { return lhs.m_str <=> rhs;       }
    operator<=>(const XStdStr& lhs, const StrT&    rhs) const { return lhs       <=> rhs.m_str; }
    operator<=>(const StrT&    lhs, const XStdStr& rhs) const { return lhs.m_str <=> rhs;       }
    // clang-format on
#endif

    // implicit conversion operators
    operator XPCStr() const { return m_str.c_str(); }
    operator XPStr() { return static_cast<XPStr>(m_str.c_str()); }
    operator XStdStr&() { return m_str; }
    operator const XStdStr&() const { return m_str; }

#ifdef XTD_CPP_17
    template<typename CharT, typename Traits = std::char_traits<CharT>>
    operator std::basic_string_view<CharT, Traits>() const noexcept
    {
        return std::basic_string_view<CharT, Traits>(m_str.data(), m_str.size());
    }
#endif

    XPCStr front() const { return m_str.c_str(); }
    XPCStr back() const
    {
        XTD_ASSERT(!m_str.empty());
        return m_str[m_str.length() - 1];
    }

    // clang-format off
    XPCStr    c_str   () const { return m_str.c_str(); }
    XPCStr    data    () const { return m_str.c_str(); }
    XPStr     data    ()       { return static_cast<XPStr>(m_str.data()); }
    XSizeType length  () const { return m_str.length(); }
    XSizeType size    () const { return m_str.size(); }
    XSizeType max_size() const { return m_str.max_size(); }
    XSizeType capacity() const { return m_str.capacity(); }
    bool      empty   () const { return m_str.empty(); }
    void      clear   ()       { m_str.clear(); }
    void      reserve(XSizeType new_cap = 0) { m_str.reserve(new_cap); }
    // clang-format on

    // clang-format off
    auto begin  ()       { return m_str.begin(); }
    auto cbegin () const { return m_str.begin(); }
    auto end    ()       { return m_str.end(); }
    auto cend   () const { return m_str.end(); }
    auto rbegin ()       { return m_str.rbegin(); }
    auto crbegin() const { return m_str.rbegin(); }
    auto rend   ()       { return m_str.rend(); }
    auto crend  () const { return m_str.rend(); }
    // clang-format on

    // clang-format off
    const XStdStr& str() const { return m_str; }
    XStdStr&       str()       { return m_str; }
    // clang-format on

private:
    XStdStr m_str;
};

END_NAMESPACE_XTD
