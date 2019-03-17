from django.shortcuts import render

# Create your views here.
def post_link(request):
    return render(request, 'main/post_link.html', {})