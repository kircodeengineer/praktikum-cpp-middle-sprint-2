#include <cstdint>
#include <cstdlib>
#include <scan.hpp>

int main() {
    constexpr const std::size_t temp_size{45};
    constexpr stdx::details::fixed_string<temp_size> str77("123 456 789 Awesome");
    constexpr stdx::details::fixed_string<temp_size> str7("{%u} {%d} {%s} {}");

    constexpr auto lol7 = stdx::details::format_string<str7>{};

    constexpr auto parse_result = stdx::details::parse_input<std::int32_t, 0, lol7, str77>();
    constexpr auto parse_result1 = stdx::details::parse_input<std::uint32_t, 1, lol7, str77>();
    constexpr auto parse_result2 = stdx::details::parse_input<std::string_view, 2, lol7, str77>();
    constexpr auto parse_result3 = stdx::details::parse_input<std::string_view, 3, lol7, str77>();
    constexpr auto scan_result =
        stdx::scan<lol7, str77, const std::int32_t, std::uint32_t, std::string_view, std::string_view>();
    return EXIT_SUCCESS;
}