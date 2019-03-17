from django.db import models
from django.conf import settings
from django.utils import timezone
# from library_base_classes import Book


class BookPost(models.Model):
    name = models.CharField(max_length=100)
    author = models.CharField(max_length=100)
    year = models.IntegerField()
    amount_of_pages = models.IntegerField()
    genres = models.CharField(max_length=200)
