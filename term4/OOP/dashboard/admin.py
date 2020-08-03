from django.contrib import admin
from .models import Sale

# Register your models here.

@admin.register(Sale)
class SaleAdmin(admin.ModelAdmin):
    list_display = ['who_bought', 'gained_money', 'transaction_date']
