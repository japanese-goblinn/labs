from django.urls import path

from . import views

urlpatterns = [
    path('query', views.execute_query, name='query'),
    path('procedure', views.execute_procedure, name='procedure')
]