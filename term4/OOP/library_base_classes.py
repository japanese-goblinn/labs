import getpass
import sys
import datetime
import random
from dataclasses import dataclass
from typing import List, Dict, Tuple


class Book:
    """
    Class that discribes  an instance of a book
    """

    def __init__(self, name: str, author: str, amount_of_pages: int, year: int, genre: List[str]):
        self.__name = name
        self.__author = author
        self.__year = year
        self.__amount_of_pages = amount_of_pages
        self.__genre = genre

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
    @staticmethod
    def match_books_with_costs(shelf: Dict[Book, int]) -> Dict[Book, str]:
        books_and_costs: Dict[Book, str] = {}
        for book in shelf:
            if book not in books_and_costs:
                books_and_costs[book] = BookCostEstimate.count_cost(
                    book, shelf)
        return books_and_costs


class User:
    def __init__(self, login: str, password: str, balance: str = '$0'):
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


class Enrollment:
    def __init__(self, users: Dict[User, Form], books: Dict[Book, str], books_amount: Dict[Book, int]):
        self.__users = users
        self.__books = books
        self.__books_amount = books_amount

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


class Menu:
    users: List[User] = []
    current_user = None
    library = None

    @classmethod
    def main_interface(cls):
        while True:
            if cls.current_user == Admin():
                print(f'\nWelcome back, {cls.current_user.login}\n')
                choose = input(
                    "\nMake your choise:\n1)See registration form\n2)See all users\n3)balance\nPress anything to log out...\n")
                if choose == '1':
                    FormShow.show(cls.library.users.get(cls.current_user))
                elif choose == '2':
                    print("\nList of all users:\n")
                    for user in cls.users:
                        print(f"{user}")
                elif choose == '3':
                    print(
                        f'\nMy current balance: {cls.current_user.balance}\n')
                else:
                    break
            else:
                print(f'\nWelcome back, {cls.current_user.login}\n')
                choose = input(
                    "\nMake your choise:\n1)See registration form\n2)See my favorite geners\n3)See current balance\n4)See all books in library\nPress anything to logout...\n")
                if choose == '1':
                    FormShow.show(cls.library.users.get(cls.current_user))
                elif choose == '2':
                    my_books = cls.library.users.get(
                        cls.current_user).fav_geners
                    print(f"\nMy favorite geners:")
                    for i, book in enumerate(my_books):
                        print(f"{i+1}.{book}")
                elif choose == '3':
                    print(
                        f'\nMy current balance: {cls.current_user.balance}\n')
                elif choose == '4':
                    print("\nAll library books:\n")
                    for i, book in enumerate(cls.library.books):
                        print(
                            f"{i+1}. Book:\'{book.name}\', Price: {cls.library.books.get(book)}, Available amount: {cls.library.books_amount.get(book)}")
                else:
                    break

    @classmethod
    def login(cls):
        login = input("Login: ")
        password = getpass.getpass("Password: ")
        if Admin() in cls.users:
            print("\nSuccessfully loged in...")
            cls.current_user = Admin()
            cls.main_interface()
        if User(login, password) in cls.users:
            print("\nSuccessfully loged in...")
            cls.current_user = User(login, password)
            cls.main_interface()
        else:
            print("\nNo user with such name and password!\nPlease, regiser first...")

    @classmethod
    def register(cls):
        login = input("Input your login: ")
        password = getpass.getpass("Input your password: ")
        if User(login, password) not in cls.users:
            cls.users.append(User(login, password))
            print("\nSuccessfully registered...")
            cls.current_user = User(login, password)
            print("\nPlease, fill in your registration form:\n")
            register_info = []
            form = Form()
            for field in form.__annotations__:
                if field == 'fav_geners':
                    inpt = list(input(f"Input your {field}: ").split(','))
                    register_info.append(inpt)
                    continue
                inpt = input(f"Input your {field}: ")
                register_info.append(inpt)
            my_form = UserRegistrationForm.get_form(
                cls.current_user, register_info)
            cur_form_and_user = UsersFormsAgregation.match_user_with_form(
                cls.current_user, my_form)
            cls.library.users = cur_form_and_user
            print("Registration comlete...\n")
            cls.main_interface()
        else:
            print("\nYou're already registered!\nPlease, login...")


def main():
    shelf = BookShelf.make_shelf([
        Book(name='1984', author='George Orwell', year=1950, amount_of_pages=330,  genre=[
             'Dystopian', 'Political Fiction', 'Social Science', 'Fiction']),
        Book(name='1984', author='George Orwell', year=1950, amount_of_pages=330, genre=[
             'Dystopian', 'Political Fiction', 'Social Science', 'Fiction']),
        Book(name='1984', author='George Orwell', year=1950, amount_of_pages=330, genre=[
             'Dystopian', 'Political Fiction', 'Social Science', 'Fiction']),
        Book(name='Flowers for Algernon', year=1973, amount_of_pages=210,
             author='Daniel Keyes', genre=['Science Fiction']),
        Book(name='Test3', author='f', amount_of_pages=666,
             year=2018, genre=['Test']),
    ])
    books_cost = BooksCostAgregation.match_books_with_costs(shelf)
    admin = Admin()
    form = UserRegistrationForm.get_form(
        admin, ['Admin', 'Adminovich', 'admin', '32ad3min', ["Fiction"]])
    form_and_user = UsersFormsAgregation.match_user_with_form(admin, form)
    library = Enrollment(form_and_user, books_cost, shelf)
    Menu.library = library
    Menu.users.append(admin)
    while True:
        _input = input(
            "\n!Welcome to ProjectLib alpha 0.1!\n\n Make your choise:\n 1)login\n 2)register\n Press anything to exit...\n")
        if _input == '1':
            print('\nPlease, login:\n')
            Menu.login()
        elif _input == '2':
            print('\nPlease, register:\n')
            Menu.register()
        else:
            print("Goodbye, see you next time ^-^")
            break


if __name__ == "__main__":
    main()
