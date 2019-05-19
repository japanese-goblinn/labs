from django.shortcuts import render, redirect
from django.contrib.auth import authenticate
from django.contrib.auth import logout as auth_logout
from django.contrib.auth import login as auth_login
from django.contrib.sites.shortcuts import get_current_site
from django.template.loader import render_to_string
from django.contrib import messages
from django.contrib.auth.decorators import login_required
from .forms import UserRegistrationForm, UserLoginForm, UserUpdateForm, ProfileForm
from .models import CustomUser
from main.models import BookInstance
# from admin_dashboard.models import Sale
from django.utils.encoding import force_bytes, force_text
from django.utils.http import urlsafe_base64_encode, urlsafe_base64_decode
from .tokens import account_activation_token
from django.core.mail import EmailMessage
from django.http import HttpResponse
from decimal import Decimal


def login(request):
    choose = 'container'
    if request.method == 'POST':
        form = UserLoginForm(request.POST)
        if form.is_valid(): 
            username = form.cleaned_data.get('username')
            password = form.cleaned_data.get('password')
            user = authenticate(request, username=username, password=password)
            auth_login(request, user)
            messages.success(request, f'Welcome back, {username}')
            return redirect('home')
    else:
        form = UserLoginForm()
    return render(request, 'users/sign_in_or_up.html', {
        'form': form,
        'choose': choose
        })


def logout(request):
    auth_logout(request)
    return login(request)


def register(request):
    choose = 'container right-panel-active'
    if request.method == 'POST':
        form = UserRegistrationForm(request.POST)
        if form.is_valid():
            user = form.save(commit=False)
            user.is_active = False
            user.save()
            current_site = get_current_site(request)
            mail_subject = 'Please, activate your ProjectLib account'
            message = render_to_string('users/email.html', {
                'user': user,
                'domain': current_site.domain,
                'uid': urlsafe_base64_encode(force_bytes(user.pk)),
                'token': account_activation_token.make_token(user)
            })
            email_to = form.cleaned_data.get('email')
            email = EmailMessage(mail_subject, message, to=[email_to])
            email.send()
            return render(request, 'users/email_confirm.html')
    else:
        form = UserRegistrationForm()
    return render(request, 'users/sign_in_or_up.html', {
        'form': form, 
        'choose': choose
        })


def activate(request, uidb64, token):
    try:
        uid = force_text(urlsafe_base64_decode(uidb64))
        user = CustomUser.objects.get(pk=uid)
    except (TypeError, ValueError, OverflowError, CustomUser.DoesNotExist):
        user = None
    if user and account_activation_token.check_token(user, token):
        user.is_active = True
        user.balance = Decimal('100')
        user.save()
        auth_login(request, user)
        messages.success(request, f'Welcome, {user.username}, check your balance c:')
        return redirect('profile_register')
    else:
        return HttpResponse('Activation link is invalid')


@login_required
def user_books(request):
    user_books = BookInstance.objects.filter(taken_by=request.user)
    return render(request, 'users/user_books.html', {
        'user_books': user_books
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
    return render(request, 'users/profile_registration.html', {
        'form': form
    })


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
    books_count = BookInstance.objects.filter(taken_by=request.user).count()
    user_books_count = books_count if books_count else "Empty"
    return render(request, 'users/profile.html', {
        'u_form': u_form,
        'p_form': p_form,
        'books_count': user_books_count
        })