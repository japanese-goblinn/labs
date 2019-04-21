from django.shortcuts import render, redirect
from django.contrib.auth import authenticate
from django.contrib.auth import login as auth_login
from django.contrib.auth.models import User
from django.contrib import messages
from django.contrib.auth.decorators import login_required
from django.views.generic import ListView
from .forms import UserRegistrationForm, UserLoginForm, UserUpdateForm, ProfileForm
from main.models import BookInstance


def login(request):
    choose = 'container'
    if request.method == 'POST':
        form = UserLoginForm(request.POST)
        if form.is_valid(): 
            username = form.cleaned_data.get('username')
            password = form.cleaned_data.get('password')
            user = authenticate(username=username, password=password)
            auth_login(request, user)
            messages.success(request, f'Welcome back, {username}')
            return redirect('home')
    else:
        form = UserLoginForm()
    return render(request, 'users/sign_in_or_up.html', {
        'form': form,
        'choose': choose
        })


def register(request):
    choose = 'container right-panel-active'
    if request.method == 'POST':
        form = UserRegistrationForm(request.POST)
        if form.is_valid():
            username = form.cleaned_data.get('username')
            password = form.cleaned_data.get('password1')
            user = form.save()
            auth_login(request, user)
            messages.success(request, f'Welcome to library, {username}')
            return redirect('profile_register')
    else:
        form = UserRegistrationForm()
    return render(request, 'users/sign_in_or_up.html', {
        'form': form, 
        'choose': choose
        })


@login_required 
def profile_register(request):
    if request.method == 'POST':
        form = ProfileForm(request.POST, request.FILES, instance=request.user.profile)
        if form.is_valid():
            form.save()
            return redirect('home')
    else:
        form = ProfileForm()
    return render(request, 'users/profile_registration.html', {'form': form})


@login_required
def profile_update(request):
    if request.method == 'POST':
        u_form = UserUpdateForm(request.POST, instance=request.user)
        p_form = ProfileForm(request.POST, request.FILES,
         instance=request.user.profile)
        if u_form.is_valid() and p_form.is_valid():
            u_form.save()
            p_form.save()
            messages.success(request, 'Your account has been updated')
    else:
        u_form = UserUpdateForm(instance=request.user)
        p_form = ProfileForm(instance=request.user.profile)
        this_user_books = BookInstance.objects.filter(taken_by=request.user)
    
    return render(request, 'users/profile.html', {
        'u_form': u_form,
        'p_form': p_form,
        'user_books': this_user_books
        })