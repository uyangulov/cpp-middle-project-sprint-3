#include <gtest/gtest.h>

#include "book.hpp"
#include "book_database.hpp"

using namespace bookdb;

namespace {

TEST(BookDatabaseTypes, TypeAliasesExist) {
    BookDatabase<> db;
    BookDatabase<>::iterator it = db.begin();
    BookDatabase<>::const_iterator cit = db.begin();
    BookDatabase<>::AuthorContainer authors;
    SUCCEED();
}

TEST(BookDatabaseBasic, EmptyDatabase) {
    BookDatabase<> db;
    EXPECT_EQ(db.size(), 0u);
    EXPECT_TRUE(db.begin() == db.end());
}

TEST(BookDatabaseBasic, PushBackAddsBook) {
    BookDatabase<> db;
    Book b{"1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190};

    db.PushBack(b);

    EXPECT_EQ(db.size(), 1u);
    EXPECT_EQ(db.GetBooks()[0].title, "1984");
}

TEST(BookDatabaseBasic, PushBackAddsUniqueAuthorOnlyOnce) {
    BookDatabase<> db;

    db.PushBack({"1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190});
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    {
        std::string s = "George Orwell";
        db.EmplaceBack("A", s, 1949, Genre::SciFi, 4., 190);
    }
    {
        std::string s = "George Orwell";
        db.PushBack({"B", s, 1949, Genre::SciFi, 4., 190});
    }
    EXPECT_EQ(db.GetAuthors().size(), 1u);
    EXPECT_EQ(*db.GetAuthors().begin(), "George Orwell");
    EXPECT_EQ(*std::next(db.GetAuthors().begin(), 1), "George Orwell");
    EXPECT_EQ(*std::next(db.GetAuthors().begin(), 2), "George Orwell");
}

TEST(BookDatabaseBasic, EmplaceBackWorks) {
    BookDatabase<> db;

    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);

    EXPECT_EQ(db.size(), 1u);
    EXPECT_EQ(db.GetAuthors().size(), 1u);
}

TEST(BookDatabaseBasic, ClearRemovesAllData) {
    BookDatabase<> db;
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);

    db.Clear();

    EXPECT_EQ(db.size(), 0u);
    EXPECT_TRUE(db.GetAuthors().empty());
}

TEST(BookDatabaseIterators, IteratorAccess) {
    BookDatabase<> db;
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);

    auto it = db.begin();
    EXPECT_EQ(it->author, "George Orwell");
}

}  // namespace
