from django.db import models
from users.models import CustomUser
from decimal import Decimal

# Create your models here.

class Sale(models.Model):
    who_bought = models.ForeignKey(CustomUser, on_delete=models.SET_NULL, null=True)
    gained_money = models.DecimalField(max_digits=10, decimal_places=2, default=0.00)
    transaction_date = models.IntegerField(default=0)

    def count_amount(self):
        amount = self.objects.all()
        return sum(Decimal(sale.gained_money) for sale in amount)

    def count_for(self, month):
        amount = self.objects.filter(transaction_date=month)
        if amount:
            return sum(Decimal(sale.gained_money) for sale in amount)
        else:
            return 0

    def __str__(self):
        return f'{self.transaction_date}'