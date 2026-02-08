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
    auto it = std::ranges::find(genre_to_string, s);
    std::size_t index = std::ranges::distance(genre_to_string.begin(), it);
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
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
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
