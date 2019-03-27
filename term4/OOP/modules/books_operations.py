import datetime
from typing import List, Dict, Tuple
from library import Book

def make_shelf(books: List[Book]) -> Dict[Book, int]:
    """Creates a shelf of books with amount for each one"""

    local_books = [book.name for book in books]
    amount: Dict[Book, int] = {}
    amount = {book: local_books.count(
        book.name) for book in books if book not in amount}
    return amount


def count_cost(book: Book, shelf: Dict[Book, int]) -> str:
    """Calculate price for a book"""

    amount: int = shelf.get(book, 0)
    if not amount:
        raise KeyError("This book is not exist in this shelf")
    else:
        actual_price = (datetime.datetime.now().year * 0.3 - book.year * 0.23) / amount / 2
        return f"${round(actual_price, 2)}"


def match_books_with_costs(shelf: Dict[Book, int]) -> Dict[Book, str]:
    """Matches books from shelf with a cost for each book"""

    books_and_costs: Dict[Book, str] = {}
    for book in shelf:
        if book not in books_and_costs:
            books_and_costs[book] = count_cost(book, shelf)
    return books_and_costs