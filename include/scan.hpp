#pragma once

#include <tuple>
#include <utility>

#include "format_string.hpp"
#include "parse.hpp"
#include "types.hpp"

namespace stdx {
template <typename T>
concept valid_type = std::disjunction_v<
    std::is_same<std::remove_cvref_t<T>, std::int8_t>, std::is_same<std::remove_cvref_t<T>, std::int16_t>,
    std::is_same<std::remove_cvref_t<T>, std::int32_t>, std::is_same<std::remove_cvref_t<T>, std::int64_t>,
    std::is_same<std::remove_cvref_t<T>, std::uint8_t>, std::is_same<std::remove_cvref_t<T>, std::uint16_t>,
    std::is_same<std::remove_cvref_t<T>, std::uint32_t>, std::is_same<std::remove_cvref_t<T>, std::uint64_t>,
    std::is_same<std::remove_cvref_t<T>, std::string_view>>;

// Главная функция
template <details::format_string fmt, details::fixed_string source, valid_type... Ts>
[[nodiscard]] consteval auto scan() noexcept {
    constexpr auto typesCount{sizeof...(Ts)};
    static_assert(fmt.number_placeholders == typesCount, "Invalid types count");
    constexpr auto make_tuple = []<std::size_t... Is>(std::index_sequence<Is...>) {
        return std::tuple{details::parse_input<std::remove_cvref_t<Ts>, Is, fmt, source>()...};
    };
    constexpr auto indices{std::make_index_sequence<typesCount>()};
    return details::scan_result<Ts...>(make_tuple(indices));
}

}  // namespace stdx