from django.urls import path
from .views import BookListView, AuthorListView, AuthorDetailView
from . import views

urlpatterns = [
    path('', views.home, name='home'),
    path('all_books/', BookListView.as_view(), name='all_books'),
    path('book/<int:pk>/', views.book_detail, name='book_detail'),
    path('book_add/<int:pk>/<int:book_id>', views.add_book, name="book_add"),
    path('book_remove/<int:pk>/<int:book_id>', views.remove_book, name="book_remove"),
    path('all_authors/', AuthorListView.as_view(), name='all_authors'),
    path('author/<int:pk>', AuthorDetailView.as_view(), name='author_detail')
] 