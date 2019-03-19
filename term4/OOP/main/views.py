from django.shortcuts import render, get_object_or_404
from .models import BookPost
from .forms import FormPost


def post_link(request):
    books = BookPost.objects.all()
    return render(request, 'main/post_link.html', {'books': books})

def book_detail(request, pk):
    book = get_object_or_404(BookPost, pk=pk)
    return render(request, 'main/book_detail.html', {'book': book})

def form_new(request):
    if request.method == 'POST':
        form = FormPost(request.POST)
        if form.is_valid():
            form.save()
    else:
        form = FormPost()
    return render(request, 'main/form_new.html', {'form': form})