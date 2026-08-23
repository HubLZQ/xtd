#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <type_traits>

BEGIN_NAMESPACE_XTD

template<typename T, typename... Ts>
constexpr bool is_all_type_same = std::conjunction_v<std::is_same<T, Ts>...>;

template<typename T, typename... Ts>
constexpr bool is_any_type_same = std::disjunction_v<std::is_same<T, Ts>...>;

END_NAMESPACE_XTD
