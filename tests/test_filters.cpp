#include <gtest/gtest.h>

#include "book.hpp"
#include "filters.hpp"
#include "statistics.hpp"

using namespace bookdb;

namespace {

TEST(FilterPredicates, YearBetween) {
    Book b{"Test", "Author", 1950, Genre::Fiction, 4.0, 100};

    EXPECT_TRUE(YearBetween(1900, 2000)(b));
    EXPECT_FALSE(YearBetween(1960, 2000)(b));
}

TEST(FilterPredicates, RatingAbove) {
    Book b{"Test", "Author", 1950, Genre::Fiction, 4.6, 100};

    EXPECT_TRUE(RatingAbove(4.5)(b));
    EXPECT_FALSE(RatingAbove(4.8)(b));
}

TEST(FilterPredicates, GenreIs) {
    Book b{"Test", "Author", 1950, Genre::SciFi, 4.0, 100};

    EXPECT_TRUE(GenreIs(Genre::SciFi)(b));
    EXPECT_FALSE(GenreIs(Genre::Fiction)(b));
}

TEST(FilterBooks, CombinedFilters) {
    std::vector<Book> books = {{"A", "X", 1950, Genre::Fiction, 4.6, 100},
                               {"B", "Y", 1980, Genre::SciFi, 4.2, 120},
                               {"C", "Z", 2001, Genre::Fiction, 4.9, 90}};

    auto res = filterBooks(books.begin(), books.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));

    ASSERT_EQ(res.size(), 1u);
    EXPECT_EQ(res[0].get().title, "A");
}

}  // namespace
