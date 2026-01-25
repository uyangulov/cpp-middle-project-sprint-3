#include <algorithm>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statistics.hpp"
#include <format>
#include <iostream>
#include <vector>

using namespace bookdb;

int main() {

    // Create a book database
    BookDatabase<std::vector<Book>> db;

    // Add some books
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    const Book b = {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143};
    db.PushBack(b);
    db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);

    std::print("Books: {}\n\n", db);

    std::sort(db.begin(), db.end(), comp::LessByAuthor{});
    std::print("Books sorted by author: {}\n\n==================\n", db);

    std::sort(db.begin(), db.end(), comp::LessByPopularity{});
    std::print("Books sorted by popularity: {}\n\n==================\n", db);

    auto histogram = buildAuthorHistogramFlat(db);
    auto genreRatings = calculateGenreRatings(db.begin(), db.end());

    std::cout << "Author histogram:\n";
    for (const auto &[author, count] : histogram) {
        std::cout << "  " << author << ": " << count << '\n';
    }

    std::cout << "\nGenre ratings:\n";
    for (const auto &[genre, rating] : genreRatings) {
        std::cout << "  " << static_cast<int>(genre) << ": " << rating << '\n';
    }

    auto avrRating = calculateAverageRating(db);
    std::print("Average books rating in library: {}\n", avrRating);

    auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    std::print("\n\nBooks from the 20th century with rating ≥ 4.5:\n");
    std::for_each(filtered.cbegin(), filtered.cend(), [](const auto &v) { std::print("{}\n", v.get()); });

    auto topBooks = getTopNBy(db, 3, comp::LessByRating{});
    std::print("\n\nTop 3 books by rating:\n");
    std::for_each(topBooks.cbegin(), topBooks.cend(), [](const auto &v) { std::print("{}\n", v.get()); });

    auto orwellBookIt = std::find_if(db.begin(), db.end(), [](const auto &v) { return v.author == "George Orwell"; });
    if (orwellBookIt != db.end())
        std::print("\n\nTransparent lookup by authors. Found Orwell's book:{}\n", *orwellBookIt);

    return 0;
}