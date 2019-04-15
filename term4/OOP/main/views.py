from django.shortcuts import render, get_object_or_404
from django.views.generic import ListView, DetailView
from django.contrib.auth.mixins import LoginRequiredMixin
from .models import Book, BookInstance, Author, Genre


class BookListView(ListView):
    model = Book
    paginate_by = 3
    template_name = 'main/book_list.html'
    context_object_name = 'books'


class BookDetailView(DetailView):
    model = Book
    
    
class AuthorDetailView(DetailView):
    model = Author


class BooksTakenByUser(LoginRequiredMixin, ListView):
    model = BookInstance

    def get_queryset(self):
        return BookInstance.objects.filter(taken_by=self.request.user)
    

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
