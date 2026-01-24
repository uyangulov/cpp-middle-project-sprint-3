#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = true;

template <typename T>
concept BookIterator = std::forward_iterator<T>;

template <typename S, typename I>
concept BookSentinel = true;

template <typename P>
concept BookPredicate = std::predicate<P, const Book &>;

template <typename C>
concept BookComparator = std::predicate<C, const Book &, const Book &>;

}  // namespace bookdb