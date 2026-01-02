#pragma once
#include <algorithm>
#include <assert.h>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины
template <std::size_t SIZE>
struct fixed_string {
    char data[SIZE]{};

    fixed_string() = delete;
    fixed_string(const fixed_string &) = default;
    fixed_string(fixed_string &&) = default;
    fixed_string &operator=(const fixed_string &) = delete;
    fixed_string &operator=(fixed_string &&) = delete;
    ~fixed_string() = default;

    constexpr fixed_string(const char (&arrChar)[SIZE]) noexcept { std::copy_n(arrChar, SIZE, data); }

    template <std::size_t ARR_CHAR_SIZE>
    constexpr fixed_string(const char (&arrChar)[ARR_CHAR_SIZE]) noexcept
        requires(ARR_CHAR_SIZE != SIZE)
    {
        static_assert(ARR_CHAR_SIZE <= SIZE);
        std::copy_n(arrChar, ARR_CHAR_SIZE, data);
    }

    constexpr fixed_string(const char *begin, const char *end) noexcept {
        // компилятор не позволяет работать с указателями в compile-time
        assert(std::distance(begin, end) <= SIZE);
        std::copy(begin, end, data);
    }

    [[nodiscard]] constexpr const std::size_t size() const noexcept { return SIZE; }
};

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга
constexpr std::size_t PARSE_ERROR_MAX_LEN{100};
struct parse_error : fixed_string<PARSE_ERROR_MAX_LEN> {};

// Шаблонный класс для хранения результатов парсинга
template <typename... Ts>
struct scan_result {
    scan_result() = delete;
    scan_result(const scan_result &) = delete;
    scan_result(scan_result &&) = delete;
    scan_result &operator=(const scan_result &) = delete;
    scan_result &operator=(scan_result &&) = delete;
    ~scan_result() = default;

    constexpr scan_result(std::tuple<Ts...> values) : values{std::move(values)} {}

    std::tuple<Ts...> values{};
};

}  // namespace stdx::details
