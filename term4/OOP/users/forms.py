from django import forms
from django.contrib.auth.forms import UserCreationForm
from .models import Profile, CustomUser
from django.core.exceptions import ValidationError
from django.contrib.auth import authenticate


class UserRegistrationForm(UserCreationForm):
    email = forms.EmailField(required=True, widget=forms.EmailInput(attrs={'placeholder': 'Email'}))

    class Meta:
        model = CustomUser
        fields = ['username', 'email', 'password1', 'password2']


class UserLoginForm(forms.Form):
    username = forms.CharField(required=True, widget=forms.TextInput(attrs={'placeholder': 'Username'}))
    password = forms.CharField(required=True, widget=forms.PasswordInput(attrs={'placeholder': 'Password'}))

    def clean(self):
        password = self.cleaned_data.get('password')
        username = self.cleaned_data.get('username')
        if password and username:
            user = authenticate(username=username, password=password)
            if not user:
                raise ValidationError("This user is not exist, please register")
        return self.cleaned_data


class UserUpdateForm(forms.ModelForm):
    email = forms.EmailField(required=True)

    class Meta:
        model = CustomUser
        fields = ['username', 'email']


class ProfileForm(forms.ModelForm):
    class Meta:
        model = Profile
        fields = [
            'name', 
            'last_name',
            'passport_id',
            'fav_genres',
            'image'
            ]