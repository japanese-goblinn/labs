from django.urls import path
from . import views

urlpatterns = [
    path('get_book/<str:book>/<str:format_type>', views.api_get, name='book_get')
]