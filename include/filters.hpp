#pragma once

#include <algorithm>
#include <any>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
auto YearBetween(int l, int r) {
    return [l, r](const Book &book) { return l <= book.year && book.year <= r; };
}

auto RatingAbove(double thresh) {
    return [thresh](const Book &book) { return book.rating > thresh; };
}

auto GenreIs(Genre genre) {
    return [genre](const Book &book) { return book.genre == genre; };
}

template <BookPredicate... Preds>
auto all_of(Preds... preds) {
    return [=](const Book &book) { return (preds(book) && ...); };
}

template <BookPredicate... Preds>
auto any_of(Preds... preds) {
    return [=](const Book &book) { return (preds(book) || ...); };
}

}  // namespace bookdb