from django.urls import path
from . import views

urlpatterns = [
    path('', views.post_link, name='post_link')
] 