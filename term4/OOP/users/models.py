from django.db import models
from django.contrib.auth.models import AbstractUser
from django.db import models
from PIL import Image

class CustomUser(AbstractUser):
    balance = models.DecimalField(max_digits=10, decimal_places=2, default=0.00)

    def __str__(self):
        return self.username

    def get_balance(self):
        return f'$ {self.balance}'
    
    def can_afford(self, amount):
        if self.balance - amount >= 0:
            return True
        else:
            return False


class Profile(models.Model):
    user = models.OneToOneField(CustomUser, on_delete=models.CASCADE)
    image = models.ImageField(default='default.png', upload_to='profile_pics')
    name = models.CharField(max_length=100, default='Name')
    last_name = models.CharField(max_length=100, default='Last Name')
    adress = models.CharField(max_length=100, default='Minsk, Belarus')
    passport_id = models.CharField(max_length=100, default='MP1239023')
    fav_genres = models.ManyToManyField('main.Genre', blank=True)

    def __str__(self):
        return f'Profile of {self.user.username} user'

    def save(self, force_insert=False, force_update=False, using=None, update_fields=None):
        super().save()

        img = Image.open(self.image.path)

        if img.height > 300 or img.width > 300:
            output_size = (300, 300)
            img.thumbnail(output_size)
            img.save(self.image.path)
