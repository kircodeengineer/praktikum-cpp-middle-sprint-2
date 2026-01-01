#pragma once
#include <algorithm>
#include <assert.h>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины
template <std::size_t SIZE>
class fixed_string {
private:
    char data[SIZE]{};

public:
    fixed_string() = delete;
    fixed_string(fixed_string &) = delete;
    constexpr fixed_string(fixed_string &&) = default;
    fixed_string &operator=(fixed_string &) = delete;
    fixed_string &operator=(fixed_string &&) = delete;
    ~fixed_string() = default;

    template <std::size_t ARR_CHAR_SIZE>
    constexpr fixed_string(const char (&arrChar)[ARR_CHAR_SIZE]) noexcept
        requires(ARR_CHAR_SIZE == SIZE)
    {
        std::copy_n(arrChar, SIZE, data);
    }

    template <std::size_t ARR_CHAR_SIZE>
    constexpr fixed_string(const char (&arrChar)[ARR_CHAR_SIZE]) noexcept
        requires(ARR_CHAR_SIZE != SIZE)
    {
        static_assert(ARR_CHAR_SIZE <= SIZE);
        std::copy_n(arrChar, ARR_CHAR_SIZE, data);
    }

    fixed_string(const char *begin, const char *end) noexcept {
        // компилятор не позволяет работать с указателями в compile-time
        assert(std::distance(begin, end) <= SIZE);
        std::copy(begin, end, data);
    }

    [[nodiscard]] constexpr const std::size_t GetSize() noexcept { return SIZE; }

    [[nodiscard]] constexpr const char *GetData() noexcept { return data; }
};

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга
constexpr std::size_t PARSE_ERROR_MAX_LEN{100};
class parse_error : public fixed_string<PARSE_ERROR_MAX_LEN> {};

// Шаблонный класс для хранения результатов парсинга
template <typename... Ts>
class scan_result {
public:
    scan_result() = delete;
    scan_result(scan_result &) = delete;
    scan_result(scan_result &&) = delete;
    scan_result &operator=(scan_result &) = delete;
    scan_result &operator=(scan_result &&) = delete;
    ~scan_result() = default;

    // debug
    constexpr scan_result(std::tuple<Ts...> values) : values{std::move(values)} {}

    [[nodiscard]] constexpr const auto &GetValues() noexcept { return values; }

private:
    std::tuple<Ts...> values{};
};

}  // namespace stdx::details
