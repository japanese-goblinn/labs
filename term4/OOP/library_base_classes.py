import getpass
import sys
import datetime
import random
from dataclasses import dataclass
from abc import ABC, abstractmethod
from typing import List, Dict, Tuple


class Book:
    """
    Discribes book instance
    """

    def __init__(self, name: str, author: str, amount_of_pages: int, year: int, genres: List[str]):
        self.__name = name
        self.__author = author
        self.__year = year
        self.__amount_of_pages = amount_of_pages
        self.__genres = genres

    def __repr__(self):
        return self.__name

    def __hash__(self):
        lenth = sum(ord(char) for key in Book.__dict__.keys() for char in key)
        return lenth

    def __eq__(self, value):
        return self.__name == value.__name

    @property
    def name(self) -> str:
        return self.__name

    @property
    def year(self) -> int:
        return self.__year

    @property
    def author(self) -> str:
        return self.__author

    @property
    def genre(self) -> List[str]:
        return self.__genres


class BookShelf:
    """
    Creates a shelf of books with amount for each one
    """

    @staticmethod
    def make_shelf(books: List[Book]) -> Dict[Book, int]:
        local_books = [book.name for book in books]
        amount: Dict[Book, int] = {}
        amount = {book: local_books.count(
            book.name) for book in books if book not in amount}
        return amount


class BookCostEstimate:
    """
    Calculate price for a book
    """

    @staticmethod
    def count_cost(book: Book, shelf: Dict[Book, int]) -> str:
        amount: int = shelf.get(book, 0)
        if not amount:
            raise KeyError("This book is not exist in this shelf")
        else:
            actual_price = (datetime.datetime.now().year * 0.3 - book.year * 0.23) / amount / 2
            return f"${round(actual_price, 2)}"


class BooksCostAgregation:
    """
    Matches books from shelf with a cost for each book
    """

    @staticmethod
    def match_books_with_costs(shelf: Dict[Book, int]) -> Dict[Book, str]:
        books_and_costs: Dict[Book, str] = {}
        for book in shelf:
            if book not in books_and_costs:
                books_and_costs[book] = BookCostEstimate.count_cost(
                    book, shelf)
        return books_and_costs


class PriceCut:
    @staticmethod
    def transform_to_float(price: str) -> float:
        if price[0] == '$':
            return float(price[1:])
        else:
            return 0
    
    @staticmethod
    def transform_to_price(price: float) -> str:
        return f"${round(price, 2)}"


class User:
    def __init__(self, login: str, password: str, balance: str = '$1000'):
        self.__login = login
        self.__password = password
        self.__balance = balance

    def __eq__(self, value):
        return self.__login == value.__login and self.__password == value.__password

    def __hash__(self):
        lenth = sum(ord(char) for key in User.__dict__ for char in key)
        return lenth

    def __repr__(self):
        return self.__login

    def withdraw(self, amount):
        cur_balance = PriceCut.transform_to_float(self.__balance)
        withdraw_amount = PriceCut.transform_to_float(amount)
        if cur_balance >= withdraw_amount:
            rest = cur_balance - withdraw_amount
            self.__balance = PriceCut.transform_to_price(rest)
        else:
            print("\n!Not enought money!\n")
            return 0

    @property
    def login(self):
        return self.__login

    @property
    def balance(self):
        return self.__balance


class Admin(User):
    def __init__(self):
        return super().__init__('admin', 'admin', '$1000000')


@dataclass
class Form:
    name: str = ""
    surname: str = ""
    adress: str = ""
    passport_id: str = ""
    fav_geners: str = ""
    taken_books = None


class FormShow:
    @staticmethod
    def show(form: Form):
        print('Registration Form:\n')
        for i, field in enumerate(form.__dict__):
            print(f'{i+1}.{field}: {getattr(form, field)}\n')


class UserRegistrationForm:
    @staticmethod
    def get_form(user: User, register_info: list) -> Form:
        form = Form(*register_info)
        return form


class UsersFormsAgregation:
    @staticmethod
    def match_user_with_form(user: User, form: Form) -> Dict[User, Form]:
        forms_dict = {}
        forms_dict[user] = form
        return forms_dict


class Recommendation(ABC):
    @abstractmethod
    def get_recommendation(self, user) -> List[Book]:
        pass


class Enrollment(Recommendation):
    def __init__(self, users: Dict[User, Form], books: Dict[Book, str], books_amount: Dict[Book, int]):
        self.__users = users
        self.__books = books
        self.__books_amount = books_amount
        self.__balance = "$0"

    def get_recommendation(self, user) -> List[Book]:
        books_to_rec: List[Book] = []
        if user in self.__users:
            for genre in self.__users[user].fav_geners:
                for book in self.__books:
                    if genre in book.genre:
                        books_to_rec.append(book)
                    else:
                        continue
        return books_to_rec

    @property
    def users(self):
        return self.__users

    @users.setter
    def users(self, value):
        self.__users.update(value)

    @property
    def books_amount(self) -> Dict[Book, int]:
        return self.__books_amount

    @property
    def books(self) -> Dict[Book, str]:
        return self.__books

    @property 
    def balance(self) -> str:
        return self.__balance

    @balance.setter
    def balance(self, value):
        cur_balance = PriceCut.transform_to_float(self.__balance)
        sum_to_add = PriceCut.transform_to_float(value)
        cur_balance += sum_to_add
        self.__balance = PriceCut.transform_to_price(cur_balance)
