#pragma once

#include <tuple>
#include <utility>

#include "format_string.hpp"
#include "parse.hpp"
#include "types.hpp"

namespace stdx {

// Главная функция
template <details::format_string fmt, details::fixed_string source, typename... Ts>
consteval auto scan() {
    constexpr auto make_tuple = []<std::size_t... Is>(std::index_sequence<Is...>) {
        return std::tuple{details::parse_input<Ts, Is, fmt, source>()...};
    };
    constexpr auto indices{std::make_index_sequence<sizeof...(Ts)>()};
    return details::scan_result<Ts...>(make_tuple(indices));
}

}  // namespace stdx