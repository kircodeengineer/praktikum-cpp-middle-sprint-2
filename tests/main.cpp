#include <cstdint>
#include <cstdlib>
#include <scan.hpp>

void test_positive() {
    using namespace stdx::details;
    // 1
    static_assert(stdx::scan<"{%d}"_fs, "12345", std::int32_t>().values == std::make_tuple(12345));
    // 2
    static_assert(stdx::scan<"{%d}"_fs, "12345", std::int64_t>().values == std::make_tuple(12345));
    // 3
    static_assert(stdx::scan<"{%u}"_fs, "12345", std::uint32_t>().values == std::make_tuple(12345));
    // 4
    static_assert(stdx::scan<"{%u}"_fs, "12345", std::uint64_t>().values == std::make_tuple(12345));
    // 5
    static_assert(stdx::scan<"{%s}"_fs, "12345", std::string_view>().values == std::make_tuple("12345"));
    // 6
    static_assert(stdx::scan<"{}"_fs, "12345", std::string_view>().values == std::make_tuple("12345"));
    // 7
    static_assert(stdx::scan<""_fs, "12345">().values == std::make_tuple());
    // 8
    static_assert(stdx::scan<"{%d} {%u} {%s}"_fs, "1 2 str", std::int32_t, std::uint32_t, std::string_view>().values ==
                  std::make_tuple(1, 2, "str"));
    // 9
    static_assert(stdx::scan<"Some {%d} text {%u} here {%s}"_fs, "Some 1 text 2 here str", std::int32_t, std::uint32_t,
                             std::string_view>()
                      .values == std::make_tuple(1, 2, "str"));
    // 10
    static_assert(stdx::scan<"{%d}"_fs, "-12345", std::int32_t>().values == std::make_tuple(-12345));
};
void test_negative() {
    using namespace stdx::details;
    // 1
    auto negTest1{stdx::scan<"{"_fs, "12345", std::string_view>()};
    // 2
    auto negTest2{stdx::scan<"}"_fs, "12345", std::string_view>()};
    // 3
    auto negTest3{stdx::scan<"{%f}"_fs, "12345", std::float>()};
    // 4
    auto negTest4{stdx::scan<"{%dd}"_fs, "12345", std::int32_t>()};
    // 5
    auto negTest5{stdx::scan<"{%d}"_fs, "str", std::int32_t>()};
    // 6
    auto negTest6{stdx::scan<"{%u}"_fs, "-12345", std::uint32_t>()};
};

int main() {
    test_positive();
    test_negative();
    return EXIT_SUCCESS;
}