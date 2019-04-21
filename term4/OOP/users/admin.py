from django.contrib import admin
from .models import Profile, CustomUser
from django.contrib.auth.admin import UserAdmin


admin.site.register(Profile)


class CustomUserAdmin(admin.ModelAdmin):
    list_display = ['username', 'email', 'balance', 'is_superuser']

admin.site.register(CustomUser, CustomUserAdmin)    
    