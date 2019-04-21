from django import forms
from django.contrib.auth.forms import UserCreationForm
from .models import Profile, CustomUser


class UserRegistrationForm(UserCreationForm):
    email = forms.EmailField()

    #TODO: add validation for fields 

    class Meta:
        model = CustomUser
        fields = ['username', 'email', 'password1', 'password2']


class UserLoginForm(forms.Form):

    #TODO: add validation for fields 

    username = forms.CharField()
    password = forms.CharField()


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