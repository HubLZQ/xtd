#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <type_traits>

#ifdef _WIN32
    #define XTD_PLATFORM_WINDOWS 1
    #ifdef _WIN64
        #define XTD_PLATFORM_WIN64 1
        #define XTD_64BIT          1
    #else
        #define XTD_PLATFORM_WIN32 1
        #define XTD_32BIT          1
    #endif
#elif defined(__linux__)
    #define XTD_PLATFORM_LINUX 1
    #ifdef __x86_64__
        #define XTD_64BIT 1
    #else
        #define XTD_32BIT 1
    #endif
#elif defined(__unix__)
    #define XTD_PLATFORM_UNIX 1
#elif defined(__APPLE__)
    #define XTD_PLATFORM_APPLE 1
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
        #define XTD_PLATFORM_IOS_SIM// "iOS Simulator"
    #elif TARGET_OS_IPHONE
        #define XTD_PLATFORM_IOS_DEV// "iOS Device"
    #elif TARGET_OS_MAC
        #define XTD_PLATFORM_MACOS// "macOS"
    #endif
    #ifdef __arm64__
        #define XTD_64BIT 1
    #else
        #define XTD_32BIT 1
    #endif
#elif defined(__ANDROID__)
    #define XTD_PLATFORM_ANDROID 1
    #define XTD_64BIT            1
#else
    #error "Unkown platform!"
#endif

#ifdef _MSC_VER
    #define XTD_COMPILER_MSVC 1
#endif

#ifdef _MSC_VER
    #define XTD_CPP_VER _MSVC_LANG
#else
    #define XTD_CPP_VER __cplusplus
#endif

#ifdef XTD_EXPORTS
    #ifdef XTD_COMPILER_MSVC
        #define XTD_API __declspec(dllexport)
    #else
        #define XTD_API __attribute__((visibility("default")))
    #endif
#else
    #ifdef XTD_COMPILER_MSVC
        #define XTD_API __declspec(dllimport)
    #else
        #define XTD_API
    #endif
#endif

#if XTD_CPP_VER >= 201103L
    #define XTD_CPP_11 1
#endif
#if XTD_CPP_VER >= 201402L
    #define XTD_CPP_14 1
#endif
#if XTD_CPP_VER >= 201703L
    #define XTD_CPP_17 1
#endif
#if XTD_CPP_VER >= 202002L
    #define XTD_CPP_20 1
#endif
#if XTD_CPP_VER >= 202302L
    #define XTD_CPP_23 1
#endif
#if XTD_CPP_VER >= 202400L
    #define XTD_CPP_26 1
#endif

#ifndef XTD_CPP_14
    #error "xtd requires at least C++14 standard!"
#endif

#define BEGIN_NAMESPACE_XTD \
    namespace xtd           \
    {
#define END_NAMESPACE_XTD \
    }

BEGIN_NAMESPACE_XTD

using I8    = int8_t;
using I16   = int16_t;
using I32   = int32_t;
using I64   = int64_t;
using U8    = uint8_t;
using U16   = uint16_t;
using U32   = uint32_t;
using U64   = uint64_t;
using F32   = float;
using F64   = double;
using Bool  = bool;
using Byte  = uint8_t;
using SByte = int8_t;

constexpr Bool True  = true;
constexpr Bool False = false;
constexpr auto Null  = nullptr;

#ifdef XTD_CPP_20
using U8Str = std::u8string;
#else
using U8Str = std::string;
#endif
using U16Str = std::u16string;
using U32Str = std::u32string;
using Str    = std::string;
using WStr   = std::wstring;

#ifdef XTD_CPP_20
using U8View = std::u8string_view;
#endif
#ifdef XTD_CPP_17
using U16View  = std::u16string_view;
using U32View  = std::u32string_view;
using StrView  = std::string_view;
using WStrView = std::wstring_view;
#endif

using Char8  = U8Str::value_type;
using Char16 = U16Str::value_type;
using Char32 = U32Str::value_type;
using Char   = Str::value_type;
using WChar  = WStr::value_type;

template<typename T, typename TDeleter = std::default_delete<T>>
using UPtr = std::unique_ptr<T, TDeleter>;

template<typename T>
using SPtr = std::shared_ptr<T>;

template<typename T>
using WPtr = std::weak_ptr<T>;

END_NAMESPACE_XTD
