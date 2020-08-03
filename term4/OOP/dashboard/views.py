from django.shortcuts import render
from django.contrib.auth.decorators import login_required
from .models import Sale

@login_required
def sales_list_view(request):
    if request.user.is_superuser:
        user_transactions = Sale.objects.all()
    else:
        user_transactions = Sale.objects.filter(who_bought=request.user)
    return render(request, 'dashboard/sales_list.html', {
        'sales': user_transactions
    })
