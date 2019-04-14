from django.shortcuts import render, get_object_or_404
from django.views.generic import ListView, DetailView
from django.contrib.auth.mixins import LoginRequiredMixin
from .models import Book, BookInstance


class BookListView(ListView):
    model = Book
    paginate_by = 1
    template_name = 'main/book_list.html'
    context_object_name = 'books'


class BookDetailView(DetailView):
    model = Book
    

class BooksTakenByUser(LoginRequiredMixin ,ListView):
    model = BookInstance

    def get_queryset(self):
        return BookInstance.objects.filter(taken_by=self.request.user)
    