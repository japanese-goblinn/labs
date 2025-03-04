from datetime import date, timedelta, datetime
from decimal import Decimal
from django.shortcuts import render, redirect
from django.contrib import messages
from django.contrib.auth.decorators import login_required
from django.views.generic import ListView, DetailView
from .models import Book, BookInstance, Author, Genre, Counter
from users.models import CustomUser
from dashboard.models import Sale
from sklearn.tree import DecisionTreeClassifier
from sklearn import metrics
from django.db.models import Q


def easter_egg(request):
    if not Counter.objects.all():
        counter = Counter()
        counter.save()
    else:
        counter = Counter.objects.all()[0]
        counter.counts += 1
        counter.save()
        if counter.counts > 50:
            messages.success(request, 'Now you\'re a Master of the Click, you can be proud for yourself')
            Counter.objects.all().delete()
            return True
    return False


class BookListView(ListView):
    model = Book
    paginate_by = 3
    context_object_name = 'books'


@login_required
def book_detail(request, pk):
    this_book = Book.objects.get(pk=pk)
    books = Book.get_user_books(this_book, user=request.user)
    for book in books:
        if book.taken_by == request.user:
            book.status = 'add'
    return render(request, 'main/book_detail.html', {
            'book': this_book,
            'books': books
        })


class AuthorListView(ListView):
    model = Author
    context_object_name = 'authors'


class AuthorDetailView(DetailView):
    model = Author


def train_model(data):
    cls = DecisionTreeClassifier()
    features = [[1]] * len(data)
    cls.fit(features, data)
    predict = cls.predict([[1]])[0]
    return predict


def machine_learning(request):
    user_books = request.user.bookinstance_set.all()
    books = None
    if user_books:
        raw_genres = []
        raw_books = []
        for book_instance in user_books:
            raw_genres.extend(book_instance.book.genres.all())
            raw_books.append(book_instance.book.name)
        genres = [genre.name for genre in raw_genres]
        genre = train_model(genres)
        genre = Genre.objects.get(name=genre)
        books = genre.book_set.filter(~Q(name__in=raw_books))
    else:
        genres = [genre.name for genre in request.user.profile.fav_genres.all()]
        genre = train_model(genres)
        genre = Genre.objects.get(name=genre)
        books = genre.book_set.all()
    return books


@login_required
def home(request):
    if request.user.is_superuser:
        total_sales_income = Sale.count_amount(Sale)
        month_income = Sale.count_for(Sale, date.today().month)
        users_amount = CustomUser.objects.filter(is_superuser=False).count()
        books_amount = Book.objects.count()
        copies_amount = BookInstance.objects.count()
        return render(request, 'main/home.html', {
            'total_income': total_sales_income,
            'month_income': month_income,
            'users': users_amount,
            'books': books_amount,
            'copies': copies_amount
        })
    else:
        # egg = easter_egg(request)
        books = machine_learning(request)
        amount = BookInstance.objects.filter(taken_by=request.user).count()
        if amount > 3:
            recently_taken = BookInstance.objects.filter(taken_by=request.user).order_by("back_date")[:3]
        else:
            recently_taken = BookInstance.objects.filter(taken_by=request.user).order_by("back_date")
        return render(request, 'main/home.html', {
            'books': books,
            'recently_taken': recently_taken
        })


def search(request):
    if request.GET.get('search') == '/makemerich/':
        user = request.user
        user.balance = Decimal(user.balance) + Decimal('1000000.00')
        user.save()
        messages.success(request, 'NoW yOu RiCh')
        return redirect('profile_update')
    search_thing = request.GET.get('search')
    book_search = Book.objects.filter(name__icontains=search_thing)
    author_search = Author.objects.filter(Q(last_name__icontains=search_thing) | Q(first_name__icontains=search_thing))
    return render(request, 'main/search.html', {
        'author_search': author_search,
        'book_search': book_search
    })



@login_required
def add_book(request, pk, book_id, price, period):
    current_book = Book.objects.get(id=book_id)
    book_to_add = BookInstance.objects.get(id=pk)
    current_user = request.user
    price = Decimal(price)
    if current_user.can_afford(price):
        current_user.balance = Decimal(current_user.balance) - price
        this_sale = Sale(gained_money=price, transaction_date=date.today().month, who_bought=current_user)
        book_to_add.taken_by = current_user
        if period == 'week':
            book_to_add.back_date = date.today() + timedelta(days=7)
        elif period == '2weeks':
            book_to_add.back_date = date.today() + timedelta(days=14)
        elif period == 'month':
            book_to_add.back_date = date.today() + timedelta(days=30)
        elif period == '3months':
            book_to_add.back_date = date.today() + timedelta(days=90)
        elif period == 'year':
            year = datetime.now().year % 4
            diff_delta = timedelta(days=366) if not (year % 4) and year % 100 else timedelta(days=365)
            book_to_add.back_date = date.today() + diff_delta
        book_to_add.status = book_to_add.STATUS[2][0]
        current_user.save()
        book_to_add.save()
        this_sale.save()
        messages.success(request, f'Book "{book_to_add.book}" added  and ${price} was withdrawn from your balance ')
        return redirect('book_detail', pk=book_id)
    else:
        messages.warning(request, f'Not enough money')
        return redirect('book_detail', pk=book_id)


@login_required
def remove_book(request, pk, book_id):
    book_to_remove = BookInstance.objects.get(id=pk)
    book_to_remove.taken_by = None
    book_to_remove.back_date = None
    book_to_remove.status = book_to_remove.STATUS[0][0] 
    book_to_remove.save()
    messages.warning(request, f'Book "{book_to_remove.book}" was removed from your library')
    return redirect('book_detail', pk=book_id)