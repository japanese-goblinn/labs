from django.urls import path
from django.contrib.auth import views as auth_views
from . import views


urlpatterns = [
    path('register/', views.register, name='register'),
    path('login/', views.login, name='login'),
    path('register/profile/', views.profile_register, name='profile_register'),
    path('profile/', views.profile_update, name='profile_update'),
    path('logout/', auth_views.LogoutView.as_view(template_name='users/logout.html'), name='logout'),
] 