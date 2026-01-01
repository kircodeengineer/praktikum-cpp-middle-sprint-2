#pragma once

#include <array>
#include <expected>
#include <string_view>

#include "types.hpp"

namespace stdx::details {

// Шаблонный класс для хранения форматирующей строчки и ее особенностей
template <fixed_string STR>
class format_string {
private:
    static constexpr auto str{STR};
    // Метод для получения количества плейсхолдеров и проверки корректности формирующей строки
    static consteval std::expected<std::size_t, parse_error> get_number_placeholders() {
        constexpr size_t N = str.GetSize();
        if (!N)
            return 0;
        size_t placeholder_count = 0;
        size_t pos = 0;
        const size_t size = N - 1;  // -1 для игнорирования нуль-терминатора

        while (pos < size) {
            // Пропускаем все символы до '{'
            if (str.GetData()[pos] != '{') {
                ++pos;
                continue;
            }

            // Проверяем незакрытый плейсхолдер
            if (pos + 1 >= size) {
                return std::unexpected(parse_error{"Unclosed last placeholder"});
            }

            // Начало плейсхолдера
            ++placeholder_count;
            ++pos;

            // Проверка спецификатора формата
            if (str.GetData()[pos] == '%') {
                ++pos;
                if (pos >= size) {
                    return std::unexpected(parse_error{"Unclosed last placeholder"});
                }

                // Проверяем допустимые спецификаторы
                const char spec = str.GetData()[pos];
                constexpr char valid_specs[] = {'d', 'u', 'f', 's'};
                bool valid = false;

                for (const char s : valid_specs) {
                    if (spec == s) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    return std::unexpected(parse_error{"Invalid specifier."});
                }
                ++pos;
            }

            // Проверяем закрывающую скобку
            if (pos >= size || str.GetData()[pos] != '}') {
                return std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
            }
            ++pos;
        }

        return placeholder_count;
    }

    // Метод для получения позиций плейсхолдеров
    static consteval auto get_placeholder_positions() {
        std::array<std::pair<std::size_t, std::size_t>, number_placeholders> result;
        std::size_t placeholder_i{};

        for (std::size_t i = 0; i < str.GetSize(); ++i) {
            switch (str.GetData()[i]) {
            case '{':
                result[placeholder_i].first = i;
                break;
            case '}':
                result[placeholder_i].second = i;
                ++placeholder_i;
                break;
            default:
                break;
            }
        }
        return result;
    }

private:
    static constexpr const auto exp_number_placeholders{get_number_placeholders()};
    static_assert(exp_number_placeholders.has_value(), std::string_view(exp_number_placeholders.error().data));

public:
    static constexpr const auto number_placeholders{exp_number_placeholders.value()};
    static constexpr const auto placeholder_positions{get_placeholder_positions()};
};
};  // namespace stdx::details

// Пользовательский литерал
/*
ваш код здесь
ваш код здесь operator"" _fs()  сигнатуру также поменяйте
{
ваш код здесь
}
*/
