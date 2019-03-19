from django.urls import path
from . import views

urlpatterns = [
    path('', views.post_link, name='home'),
    path('book/<int:pk>/', views.book_detail, name='book_detail'),
    path('form/new', views.form_new, name='form_new')
] 