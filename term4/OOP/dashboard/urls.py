from django.urls import path
from .import views

urlpatterns = [
    path('transactions/', views.sales_list_view, name='account_sales')
]