from django.urls import path
from . import views

urlpatterns = [
    path('get_book/<str:book>/<str:format_type>', views.api_get, name='book_get'),
    path ('chart_data/', views.ChartData.as_view(), name='chart_data_get'),
]