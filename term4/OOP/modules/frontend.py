import books_operations
import users_operations 
import forms_operations
from typing import List, Dict, Tuple
from library import User, Book, Form, Enrollment
import getpass


#TODO:
#1)add dates to books that was taken from library
#2)think about how to ask a user about money


class Menu:
    current_user = None
    library = None

    @classmethod
    def main_interface(cls):
        while True:
            if cls.current_user == User('admin', 'admin', '$1000000'):
                print(f'\nWelcome back, {cls.current_user.login}\n')
                choose = input(
                    "\nMake your choise:\n1)See registration form\n2)See all users\n3)Balance\n4)Library Balance\nPress anything to log out...\n")
                if choose == '1':
                    forms_operations.show_form(cls.library.users.get(cls.current_user))
                elif choose == '2':
                    print("\nList of all users:\n")
                    for user in cls.library.users:
                        print(f"{user}")
                elif choose == '3':
                    print(
                        f'\nMy current balance: {cls.current_user.balance}\n')
                elif choose == '4':
                    print(
                        f'\nLibrary current balance: {cls.library.balance}\n')
                else:
                    break
            else:
                print(f'\nWelcome back, {cls.current_user.login}\n')
                choose = input(
                    "\nMake your choise:\n1)See registration form\n2)See my favorite geners\n3)See current balance\n4)See all books in library\n5)Get reccomendations\n6)Arend a book\nPress anything to logout...\n")
                if choose == '1':
                    forms_operations.show_form(cls.library.users.get(cls.current_user))
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
                elif choose == '5':
                    books_for_cur_user = cls.library.get_recommendation(cls.current_user)
                    print("We recommend you to read: ", books_for_cur_user)
                elif choose == '6':
                    book_to_find = input('Write name of the book, that you whant to get: ')
                    library_books = [book for book in cls.library.books if book.name == book_to_find]
                    if library_books != []:
                        form = cls.library.users[cls.current_user]
                        book_to_get = library_books[0]
                        price = cls.library.books[book_to_get]
                        res = cls.current_user.withdraw(price)
                        if res == 0:
                            continue 
                        if cls.library.books_amount[book_to_get] > 0:
                            cls.library.books_amount[book_to_get] -= 1
                        else:
                            print("\nOperation failed")
                            continue
                        if form.taken_books is None:
                            form.taken_books = [book_to_get]
                        else:
                            form.taken_books.append(book_to_get)
                        cls.library.balance = price
                        print("\nOperation comlete succesfully\n")
                    else:
                        print("\nBook with such name is not exist")
                else:
                    break

    @classmethod
    def login(cls):
        login = input("Login: ")
        password = getpass.getpass("Password: ")
        if login == 'admin' and password == 'admin':
            print("\nSuccessfully loged in...")
            if User('admin', 'admin', '$1000000') in cls.library.users:
                cls.current_user = cls.library.get_user(User('admin', 'admin', '$1000000'))
            cls.main_interface()
        elif User(login, password) in cls.library.users:
            print("\nSuccessfully loged in...")
            if User(login, password) in cls.library.users:
                cls.current_user = cls.library.get_user(User(login, password))
            cls.main_interface()
        else:
            print("\nNo user with such name and password!\nPlease, regiser first...")

    @classmethod
    def register(cls):
        login = input("Input your login: ")
        password = getpass.getpass("Input your password: ")
        if User(login, password) not in cls.library.users:
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
            my_form = users_operations.get_user_form(
                cls.current_user, register_info)
            cur_form_and_user = users_operations.match_user_with_form(
                cls.current_user, my_form)
            cls.library.users = cur_form_and_user
            print("Registration comlete...\n")
            cls.main_interface()
        else:
            print("\nYou're already registered!\nPlease, login...")


def main():
    shelf = books_operations.make_shelf([
        Book(name='1984', author='George Orwell', year=1950, amount_of_pages=330,  genres=[
             'Dystopian', 'Political Fiction', 'Social Science', 'Fiction']),
        Book(name='1984', author='George Orwell', year=1950, amount_of_pages=330, genres=[
             'Dystopian', 'Political Fiction', 'Social Science', 'Fiction']),
        Book(name='1984', author='George Orwell', year=1950, amount_of_pages=330, genres=[
             'Dystopian', 'Political Fiction', 'Social Science', 'Fiction']),
        Book(name='Flowers for Algernon', year=1973, amount_of_pages=210,
             author='Daniel Keyes', genres=['Science Fiction']),
        Book(name='Test3', author='f', amount_of_pages=666,
             year=2018, genres=['Test']),
    ])
    books_cost = books_operations.match_books_with_costs(shelf)
    admin = User('admin', 'admin', '$1000000')
    form = users_operations.get_user_form(
        admin, ['Admin', 'Adminovich', 'admin', '32ad3min', ["Fiction"]])
    form_and_user = users_operations.match_user_with_form(admin, form)
    library = Enrollment(form_and_user, books_cost, shelf)
    Menu.library = library
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

main()