#pragma once

#include <algorithm>
#include <cstddef>
#include <flat_set>
#include <string>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases

    using iterator = BookContainer::iterator;
    using const_iterator = BookContainer::const_iterator;

    using AuthorContainer = std::flat_set<std::string>;

    BookDatabase() = default;

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    void PushBack(const Book &book) {
        authors_.insert(static_cast<std::string>(book.author));
        books_.push_back(book);
    }

    void PushBack(Book &&book) {
        authors_.insert(static_cast<std::string>(book.author));
        books_.push_back(std::move(book));
    }

    template <class... Args>
    void EmplaceBack(Args &&...args) {
        // TODO: прояснить строчки и отсуствие наличие &&
        auto arg_tuple = std::forward_as_tuple(std::forward<Args>(args)...);
        auto &&author_arg = std::get<1>(arg_tuple);
        authors_.insert(static_cast<std::string>(author_arg));
        books_.emplace_back(std::forward<Args>(args)...);
    }

    const AuthorContainer &GetAuthors() const { return authors_; }

    const BookContainer &GetBooks() const { return books_; }

    size_t size() const { return books_.size(); }

    const_iterator begin() const { return books_.begin(); }

    iterator begin() { return books_.begin(); }

    const_iterator end() const { return books_.end(); }

    iterator end() { return books_.end(); }

    Book *data() { return books_.data(); }

    const Book *data() const { return books_.data(); }

private:
    std::vector<Book> books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {

        format_to(fc.out(), "BookDatabase (size = {}): \n", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
