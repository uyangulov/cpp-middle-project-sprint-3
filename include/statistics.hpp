#pragma once

#include <algorithm>
#include <cstddef>
#include <flat_map>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <span>
#include <stdexcept>
#include <string_view>

#include "book.hpp"
#include "book_database.hpp"
#include "concepts.hpp"

#include <map>
#include <print>
#include <unordered_map>
#include <utility>
#include <vector>

namespace bookdb {

inline auto buildAuthorHistogramFlat(std::span<const Book> cont) {
    std::flat_map<std::string_view, size_t> counts;
    for (const auto &book : cont) {
        auto [iter, is_inserted] = counts.try_emplace(book.author, 0);
        ++iter->second;
    }
    return counts;
}

template <BookIterator Iterator>
auto calculateGenreRatings(Iterator begin, Iterator end) {
    std::flat_map<Genre, double> averages;
    std::flat_map<Genre, size_t> counts;
    for (auto it = begin; it != end; ++it) {
        Genre key = it->genre;
        auto [averages_iter, _1] = averages.try_emplace(key, 0.0);
        auto [count_iter, _2] = counts.try_emplace(key, 0);
        count_iter->second++;
        averages_iter->second += (it->rating - averages_iter->second) / count_iter->second;
    }
    return averages;
}

template <BookIterator Iterator, BookPredicate Predicate>
auto filterBooks(Iterator begin, Iterator end, Predicate predicate) {
    std::vector<std::reference_wrapper<const Book>> vec;
    vec.reserve(std::distance(begin, end));
    std::copy_if(begin, end, std::back_inserter(vec), [&](const Book &b) { return predicate(b); });
    return vec;
};

inline auto calculateAverageRating(std::span<const Book> s) -> double {
    if (s.size() == 0)
        return 0.0;
    const double sum =
        std::accumulate(s.begin(), s.end(), 0.0, [](double lhs, const Book &rhs) { return lhs + rhs.rating; });
    return sum / s.size();
};

template <BookComparator Comparator>
auto getTopNBy(std::span<Book> s, size_t N, Comparator comp) {
    const auto mid_pos = std::min(N, s.size());
    auto middle = std::next(s.begin(), mid_pos);
    std::partial_sort(s.begin(), middle, s.end(), std::not_fn(comp));
    std::vector<std::reference_wrapper<const Book>> result;
    result.reserve(N);
    std::transform(s.begin(), middle, std::back_inserter(result), [](auto &x) { return std::cref<Book>(x); });
    return result;
}

}  // namespace bookdb