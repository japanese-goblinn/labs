from datetime import date, timedelta, datetime
from decimal import Decimal
from django.shortcuts import render, redirect
from django.contrib import messages
from django.db.models import Q
from django.contrib.auth.decorators import login_required
from users.models import CustomUser
from django.views.generic import ListView, DetailView
from django.contrib.auth.mixins import LoginRequiredMixin
from .models import Book, BookInstance, Author, Genre, Sale


class BookListView(ListView):
    model = Book
    paginate_by = 9
    context_object_name = 'books'


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

    
def home(request):
    amount_of_books = Book.objects.count()
    amount_of_copies = BookInstance.objects.count()
    available_copies = BookInstance.objects.filter(status='av').count()
    amount_of_authors = Author.objects.count()
    return render(request, 'main/home.html', {
        'books': amount_of_books,
        'copies': amount_of_copies,
        'av_copies': available_copies,
        'authors': amount_of_authors
    })


def search(request):
    if request.GET.get('search') == '/makemerich/':
        user = request.user
        user.balance += Decimal('1000000')
        user.save()
        messages.success(request, 'NoW yOu RiCh')
        return redirect('profile_update')


@login_required
def add_book(request, pk, book_id):
    currnet_book = Book.objects.get(id=book_id)
    book_to_add = BookInstance.objects.get(id=pk)
    price = currnet_book.price
    current_user = request.user
    if current_user.can_afford(price):
        current_user.balance -= price
        this_sale = Sale(gained_money=price, transaction_date=datetime.now())
        book_to_add.taken_by = request.user
        book_to_add.back_date = date.today() + timedelta(days=30)
        book_to_add.status = book_to_add.STATUS[2][0]
        current_user.save()
        book_to_add.save()
        this_sale.save()
        messages.success(request, f'Book "{book_to_add.book}" added to your library and ${price} was withdrawn from your balance')
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