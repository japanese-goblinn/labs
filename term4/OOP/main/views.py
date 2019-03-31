from django.shortcuts import render, get_object_or_404
from django.views.generic import ListView, DetailView
from .models import Book
from .forms import FormPost


def list_books(request):
    books = Book.objects.all()
    return render(request, 'main/book_list.html', {'books': books})

class BookListView(ListView):
    model = Book
    template_name = 'main/book_list.html'
    context_object_name = 'books'


class BookDetailView(DetailView):
    model = Book
    

def form_new(request):
    if request.method == 'POST':
        form = FormPost(request.POST)
        if form.is_valid():
            form.save()
    else:
        form = FormPost()
    return render(request, 'main/form_new.html', {'form': form})