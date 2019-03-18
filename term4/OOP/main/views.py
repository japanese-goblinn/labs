from django.shortcuts import render
from .models import BookPost


def post_link(request):
    books = BookPost.objects.all()
    return render(request, 'main/post_link.html', {'books': books})