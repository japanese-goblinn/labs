import uuid
import datetime
from django.db import models
from django.conf import settings
from django.utils import timezone


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


class BookInstance(models.Model):
    ##FIXME: change to standart id
    id = models.UUIDField(primary_key=True, default=uuid.uuid4)
    book = models.ForeignKey("Book", on_delete=models.SET_NULL, null=True)
    back_date = models.DateField(null=True, blank=True)

    STATUS = [
        ('av', 'Available'),
        ('add', 'Added to Library'),
        ('na', 'Not Available')
    ]

    status = models.CharField(max_length=3, choices=STATUS, default='av')

    def __str__(self):
        return f"Book Instance {self.book}"

    # def count_cost(self) -> str:
    #     amount = self.objects.all().filter(name='book__name').count()
    #     actual_price = (datetime.datetime.now().year * 0.3 - self.pages * 0.23) / amount / 2
    #     return f'${actual_price}'

    # @property';\\\
    # def price(self):
    #     return self.count_cost()
    

class RegistrationForm(models.Model):
    name = models.CharField(max_length=100)
    surname = models.CharField(max_length=100)
    adress = models.CharField(max_length=100)
    passport_id = models.CharField(max_length=100)
    genres = models.ManyToManyField(Genre)