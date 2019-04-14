from django.urls import path
from .views import BookListView, BookDetailView
from . import views

urlpatterns = [
    path('', BookListView.as_view(), name='home'),
    path('book/<int:pk>/', BookDetailView.as_view(), name='book_detail')
] 