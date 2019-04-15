from django.urls import path
from .views import BookListView, BookDetailView, AuthorDetailView
from . import views

urlpatterns = [
    path('', views.home, name='home'),
    path('all_books/', BookListView.as_view(), name='all_books'),
    path('book/<int:pk>/', BookDetailView.as_view(), name='book_detail'),
    path('author/<int:pk>', AuthorDetailView.as_view(), name='author_detail')
] 