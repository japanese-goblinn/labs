import uuid
import datetime
from decimal import Decimal
from django.db import models
from django.conf import settings
from django.utils import timezone
from django.contrib.auth.models import User


class Author(models.Model):
    first_name = models.CharField(max_length=100)
    last_name = models.CharField(max_length=100)
    
    def __str__(self):
        return f"{self.first_name} {self.last_name}"
        

class Genre(models.Model):
    name = models.CharField(max_length=100)

    def __str__(self):
        return self.name


class Book(models.Model):
    name = models.CharField(max_length=100)
    author = models.ForeignKey("Author", on_delete=models.SET_NULL, null=True)
    description = models.TextField(default="Book Description") 
    publication_date = models.DateField()
    pages = models.IntegerField()
    genres = models.ManyToManyField(Genre)
    price = models.DecimalField(max_digits=10, decimal_places=2, default=10.00)

    def __str__(self):
        return self.name

    def get_price(self):
        return f"$ {self.price}"

    def save(self, force_insert=False, force_update=False, using=None, update_fields=None):
        actual_price = (datetime.datetime.now().year - self.publication_date.year) / 5
        self.price = Decimal(actual_price)
        super().save()


class BookInstance(models.Model):
    id = models.AutoField(primary_key=True)
    book = models.ForeignKey("Book", on_delete=models.SET_NULL, null=True)
    back_date = models.DateField(null=True, blank=True)
    taken_by = models.ForeignKey(User, on_delete=models.SET_NULL, null=True, blank=True)

    STATUS = [
        ('av', 'Available'),
        ('add', 'Added to Library'),
        ('na', 'Not Available')
    ]

    status = models.CharField(max_length=3, choices=STATUS, default='av')

    def __str__(self):
        return f"Book Instance {self.book}"

    @property
    def is_expired(self):
        if self.taken_by:
            if self.back_date < datetime.date.today():
                return True
            return False
