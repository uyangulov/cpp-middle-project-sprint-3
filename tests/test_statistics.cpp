#include <gtest/gtest.h>

#include "book_database.hpp"
#include "comparators.hpp"
#include "statistics.hpp"

using namespace bookdb;

namespace {

TEST(Statistics, TestCalculateAverageRating) {
    BookDatabase<> db;
    db.EmplaceBack("A", "X", 1950, Genre::Fiction, 4.0, 100);
    db.EmplaceBack("B", "Y", 1951, Genre::Fiction, 6.0, 100);

    EXPECT_DOUBLE_EQ(calculateAverageRating(db), 5.0);
}

TEST(Statistics, TestCalculateGenreRatings) {
    std::vector<Book> books = {{"A", "X", 1950, Genre::Fiction, 4.0, 100},
                               {"B", "Y", 1951, Genre::Fiction, 6.0, 100},
                               {"C", "Z", 1952, Genre::SciFi, 5.0, 100}};

    auto res = calculateGenreRatings(books.begin(), books.end());

    EXPECT_DOUBLE_EQ(res[Genre::Fiction], 5.0);
    EXPECT_DOUBLE_EQ(res[Genre::SciFi], 5.0);
}

TEST(Statistics, TestGetTopNByRating) {
    BookDatabase<> db;
    db.EmplaceBack("A", "X", 1950, Genre::Fiction, 3.0, 100);
    db.EmplaceBack("B", "Y", 1951, Genre::Fiction, 5.0, 100);
    db.EmplaceBack("C", "Z", 1952, Genre::Fiction, 4.0, 100);

    auto top = getTopNBy(db, 2, bookdb::comp::LessByRating{});

    ASSERT_EQ(top.size(), 2u);
    EXPECT_EQ(top[0].get().title, "B");
    EXPECT_EQ(top[1].get().title, "C");
}

}  // namespace
