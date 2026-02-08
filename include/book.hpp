#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

using namespace std::literals::string_view_literals;
enum class Genre { Unknown = 0, Fiction, NonFiction, SciFi, Biography, Mystery, COUNT };

static constexpr std::array<std::string_view, static_cast<std::size_t>(Genre::COUNT)> genre_to_string = {
    "Unknown"sv, "Fiction"sv, "NonFiction"sv, "SciFi"sv, "Biography"sv, "Mystery"sv};

constexpr Genre GenreFromString(std::string_view s) {
    const auto it = std::ranges::find(genre_to_string, s);
    const std::size_t index = std::ranges::distance(genre_to_string.begin(), it);
    return static_cast<Genre>(index);
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    Book(std::string_view title, std::string_view author, int year, Genre genre, double rating, int read_count)
        : author(author), title(title), year(year), genre(genre), rating(rating), read_count(read_count) {}

    Book(std::string_view title, std::string_view author, int year, std::string_view genre, double rating,
         int read_count)
        : author(author), title(title), year(year), genre(GenreFromString(genre)), rating(rating),
          read_count(read_count) {}
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {

    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::size_t index = static_cast<std::size_t>(g);
        return format_to(fc.out(), "{}", bookdb::genre_to_string[index]);
    }
    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book> {
    template <typename FormatContext>
    auto format(const bookdb::Book &book, FormatContext &fc) const {
        format_to(fc.out(), "{}. Written in {} by {}. Genre: {}. Read {} times. Rating {} \n", book.title, book.year,
                  book.author, book.genre, book.read_count, book.rating);
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
