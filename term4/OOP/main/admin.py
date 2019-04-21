from django.contrib import admin
from .models import Book, BookInstance, Author, Genre, Sale

   
class BookInline(admin.TabularInline):
    model = Book
    extra = 0


@admin.register(Author)
class AuthorAdmin(admin.ModelAdmin):
    list_display = ['last_name', 'first_name']
    inlines = [BookInline]


class BookInstanceInline(admin.TabularInline):
    model = BookInstance
    extra = 0


@admin.register(Book)
class BookAdmin(admin.ModelAdmin):
    list_display = ['name', 'author', 'pages', 'price']
    list_filter = ['author']
    inlines = [BookInstanceInline]


@admin.register(BookInstance)
class BookInstanceAdmin(admin.ModelAdmin):
    list_display = ['book', 'status', 'back_date', 'taken_by', 'id']
    list_filter = ['back_date', 'status', 'book']


admin.site.register(Genre)

@admin.register(Sale)
class SaleAdmin(admin.ModelAdmin):
    list_display = ['gained_money', 'transaction_date']
    
    