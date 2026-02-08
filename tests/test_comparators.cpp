#include <gtest/gtest.h>

#include "book.hpp"
#include "comparators.hpp"

using namespace bookdb;

namespace {

TEST(LessByAuthor, OrdersLexicographically) {
    Book a{"A", "George Orwell", 1949, Genre::Fiction, 4.0, 100};
    Book b{"B", "Jane Austen", 1813, Genre::Fiction, 4.5, 200};

    comp::LessByAuthor cmp;

    EXPECT_TRUE(cmp(a, b));  // "Jane Austen" < "George Orwell"
    EXPECT_FALSE(cmp(b, a));
    EXPECT_FALSE(cmp(b, b));
    EXPECT_FALSE(cmp(a, a));
}

TEST(LessByAuthor, EqualAuthorsReturnFalseBothWays) {
    Book a{"A", "Orwell", 1945, Genre::Fiction, 4.0, 100};
    Book b{"B", "Orwell", 1950, Genre::SciFi, 4.5, 200};

    comp::LessByAuthor cmp;

    EXPECT_FALSE(cmp(a, b));
    EXPECT_FALSE(cmp(b, a));
    EXPECT_FALSE(cmp(b, b));
    EXPECT_FALSE(cmp(a, a));
}

TEST(LessByPopularity, OrdersByReadCount) {
    Book low{"A", "X", 1950, Genre::Fiction, 4.0, 10};
    Book high{"B", "Y", 1951, Genre::Fiction, 4.0, 100};

    comp::LessByPopularity cmp;

    EXPECT_TRUE(cmp(low, high));
    EXPECT_FALSE(cmp(high, low));
    EXPECT_FALSE(cmp(high, high));
    EXPECT_FALSE(cmp(low, low));
}

TEST(LessByPopularity, EqualPopularityIsNotLess) {
    Book a{"A", "X", 1950, Genre::Fiction, 4.0, 50};
    Book b{"B", "Y", 1951, Genre::Fiction, 4.5, 50};

    comp::LessByPopularity cmp;

    EXPECT_FALSE(cmp(a, b));
    EXPECT_FALSE(cmp(b, a));
    EXPECT_FALSE(cmp(a, a));
    EXPECT_FALSE(cmp(b, b));
}

TEST(LessByRating, OrdersByRating) {
    Book low{"A", "X", 1950, Genre::Fiction, 3.5, 100};
    Book high{"B", "Y", 1951, Genre::Fiction, 4.8, 100};

    comp::LessByRating cmp;

    EXPECT_TRUE(cmp(low, high));
    EXPECT_FALSE(cmp(high, low));
    EXPECT_FALSE(cmp(low, low));
    EXPECT_FALSE(cmp(high, high));
}

TEST(LessByRating, EqualRatingIsNotLess) {
    Book a{"A", "X", 1950, Genre::Fiction, 4.5, 100};
    Book b{"B", "Y", 1951, Genre::SciFi, 4.5, 200};

    comp::LessByRating cmp;

    EXPECT_FALSE(cmp(a, b));
    EXPECT_FALSE(cmp(b, a));
    EXPECT_FALSE(cmp(a, a));
    EXPECT_FALSE(cmp(b, b));
}

TEST(Comparators, CanBeUsedWithStdSort) {
    std::vector<Book> books = {
        {"C", "Z", 1952, Genre::Fiction, 4.0, 30},
        {"A", "X", 1950, Genre::Fiction, 3.0, 10},
        {"B", "Y", 1951, Genre::Fiction, 5.0, 20},
    };

    std::sort(books.begin(), books.end(), comp::LessByRating{});

    EXPECT_EQ(books[0].rating, 3.0);
    EXPECT_EQ(books[1].rating, 4.0);
    EXPECT_EQ(books[2].rating, 5.0);
}

}  // namespace
