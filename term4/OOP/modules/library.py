import datetime
import random
import sys
import getpass
import datetime as dt
import price 
from dataclasses import dataclass
from typing import List, Dict, Tuple


class Book:
    """Discribes book instance"""

    def __init__(self, name: str, author: str, amount_of_pages: int, year: int, genres: List[str]):
        self.name = name
        self.author = author
        self.year = year
        self.amount_of_pages = amount_of_pages
        self.genres = genres

    def __repr__(self):
        return self.name

    def __hash__(self):
        return hash(self.name)

    def __eq__(self, value):
        return self.name == value.name


class User:
    def __init__(self, login: str, password: str, balance: str = '$1000'):
        self.login = login
        self.password = password
        self.balance = balance

    def __eq__(self, value):
        return self.login == value.login and self.password == value.password

    def __hash__(self):
        return hash(self.login + self.password)

    def __repr__(self):
        return self.login

    def withdraw(self, amount):
        cur_balance = price.transform_to_number(self.balance)
        withdraw_amount = price.transform_to_number(amount)
        if cur_balance >= withdraw_amount:
            rest = cur_balance - withdraw_amount
            self.balance = price.transform_to_price(rest)
        else:
            return None

@dataclass
class Form:
    name: str = ""
    surname: str = ""
    adress: str = ""
    passport_id: str = ""
    fav_geners: str = ""
    taken_books = None


class Enrollment:
    def __init__(self, users: Dict[User, Form], books: Dict[Book, str], books_amount: Dict[Book, int]):
        self._users = users
        self.books = books
        self.books_amount = books_amount
        self._balance = "$0"

    def get_recommendation(self, user) -> List[Book]:
        books_to_rec: List[Book] = []
        if user in self.users:
            for genre in self.users[user].fav_geners:
                for book in self.books:
                    if genre in book.genres:
                        books_to_rec.append(book)
                    else:
                        continue
        return books_to_rec

    def get_user(self, user_to_get):
        finded = None
        for user in self.users:
            if user == user_to_get:
                finded = user
        return finded

    @property
    def users(self):
        return self._users

    @users.setter
    def users(self, value):
        self._users.update(value)


    @property 
    def balance(self) -> str:
        return self._balance

    @balance.setter
    def balance(self, value):
        cur_balance = price.transform_to_number(self._balance)
        sum_to_add = price.transform_to_number(value)
        cur_balance += sum_to_add
        self._balance = price.transform_to_price(cur_balance)