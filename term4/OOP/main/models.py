from django.db import models
from django.conf import settings
from django.utils import timezone


class Book(models.Model):
    name = models.CharField(max_length=100)
    author = models.CharField(max_length=100)
    year = models.IntegerField()
    amount_of_pages = models.IntegerField()
    genres = models.CharField(max_length=200)

    def __str__(self):
        return f"Book \'{self.name}\''"


class Form(models.Model):
    name = models.CharField(max_length=100)
    surname = models.CharField(max_length=100)
    adress = models.CharField(max_length=100)
    passport_id = models.CharField(max_length=100)
    fav_geners = models.CharField(max_length=200)