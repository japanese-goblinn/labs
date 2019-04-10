from django import forms
from .models import RegistrationForm

class FormPost(forms.ModelForm):

    class Meta:
        model = RegistrationForm
        fields = ('name', 'surname', 'adress', 'passport_id')
    