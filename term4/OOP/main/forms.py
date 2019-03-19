from django import forms
from .models import Form

class FormPost(forms.ModelForm):

    class Meta:
        model = Form
        fields = ('name', 'surname', 'adress', 'passport_id', 'fav_geners')
    