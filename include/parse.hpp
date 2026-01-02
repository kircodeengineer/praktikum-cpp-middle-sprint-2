#pragma once

#include <charconv>
#include <concepts>
#include <cstring>
#include <iterator>
#include <optional>
#include <system_error>

#include "format_string.hpp"
#include "types.hpp"

namespace stdx::details {

// Шаблонная функция, возвращающая пару позиций в строке с исходными данными, соотвествующих I-ому плейсхолдеру
template <int I, format_string fmt, fixed_string source>
consteval auto get_current_source_for_parsing() {
    static_assert(I >= 0 && I < fmt.number_placeholders, "Invalid placeholder index");

    constexpr auto to_sv = [](const auto &fs) { return std::string_view(fs.data, fs.size() - 1); };

    constexpr auto fmt_sv = to_sv(fmt.fmt);
    constexpr auto src_sv = to_sv(source);
    constexpr auto &positions = fmt.placeholder_positions;

    // Получаем границы текущего плейсхолдера в формате
    constexpr auto pos_i = positions[I];
    constexpr size_t fmt_start = pos_i.first, fmt_end = pos_i.second;

    // Находим начало в исходной строке
    constexpr auto src_start = [&] {
        if constexpr (I == 0) {
            return fmt_start;
        } else {
            // Находим конец предыдущего плейсхолдера в исходной строке
            constexpr auto prev_bounds = get_current_source_for_parsing<I - 1, fmt, source>();
            const auto prev_end = prev_bounds.second;

            // Получаем разделитель между текущим и предыдущим плейсхолдерами
            constexpr auto prev_fmt_end = positions[I - 1].second;
            constexpr auto sep = fmt_sv.substr(prev_fmt_end + 1, fmt_start - (prev_fmt_end + 1));

            // Ищем разделитель после предыдущего значения
            auto pos = src_sv.find(sep, prev_end);
            return pos != std::string_view::npos ? pos + sep.size() : src_sv.size();
        }
    }();

    // Находим конец в исходной строке
    constexpr auto src_end = [&] {
        // Получаем разделитель после текущего плейсхолдера
        if constexpr (fmt_end == (fmt_sv.size() - 1)) {
            return src_sv.size();
        }
        constexpr auto sep =
            fmt_sv.substr(fmt_end + 1, (I < fmt.number_placeholders - 1) ? positions[I + 1].first - (fmt_end + 1)
                                                                         : fmt_sv.size() - (fmt_end + 1));
        // Ищем разделитель после текущего значения
        constexpr auto pos = src_sv.find(sep, src_start);
        return pos != std::string_view::npos ? pos : src_sv.size();
    }();
    return std::pair{src_start, src_end};
}

template <typename T>
consteval std::pair<std::from_chars_result, T> parse_number(const char *begin, const char *end) {
    T result{};
    auto parse_result = std::from_chars(begin, end, result);
    return {parse_result, result};
}

// Реализуйте семейство функция parse_value
template <typename Result, fixed_string fmt_fs, fixed_string source_fs>
consteval auto parse_value()
    requires(!std::strcmp(fmt_fs.data, "%d") || !std::strcmp(fmt_fs.data, "%u"))
{

    constexpr auto begin{source_fs.data};
    constexpr auto end{source_fs.data + source_fs.size()};
    constexpr auto result{parse_number<Result>(begin, end)};
    static_assert(result.first.ec != std::errc::invalid_argument, "Invalid data to parse in number");
    return result.second;
}
template <typename Result, fixed_string fmt_fs, fixed_string source_fs>
consteval auto parse_value()
    requires(!std::strcmp(fmt_fs.data, "%s") || !std::strcmp(fmt_fs.data, ""))
{
    return source_fs.data;
}

// Шаблонная функция, выполняющая преобразования исходных данных в конкретный тип на основе I-го плейсхолдера
template <typename result, std::size_t I, format_string fmt, fixed_string source>
consteval auto parse_input()
    requires(!std::is_const_v<result>)
{
    constexpr auto source_pos{get_current_source_for_parsing<I, fmt, source>()};
    constexpr auto source_begin{source.data + source_pos.first};
    constexpr auto source_end{source.data + source_pos.second};
    constexpr auto source_fs{fixed_string<std::distance(source_begin, source_end) + 1>(source_begin, source_end)};

    constexpr auto fmt_pos{fmt.placeholder_positions.at(I)};
    constexpr auto fmt_begin{fmt.fmt.data + fmt_pos.first + 1};
    constexpr auto fmt_end{fmt.fmt.data + fmt_pos.second};
    constexpr auto fmt_fs{fixed_string<std::distance(fmt_begin, fmt_end) + 1>(fmt_begin, fmt_end)};

    return parse_value<result, fmt_fs, source_fs>();
}

}  // namespace stdx::details
