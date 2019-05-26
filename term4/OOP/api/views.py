from django.shortcuts import render
from django.core import serializers
from main.models import Book
from users.models import CustomUser
from dashboard.models import Sale
from rest_framework.views import APIView
from rest_framework.response import Response
import datetime


class ChartData(APIView):
    authentication_classes = []
    permission_classes = []

    def get(self, request, format=None):
        this_month_num = datetime.date.today().month
        labels = [
            'January',
            'February',
            'March',
            'April',
            'May',
            'June',
            'July',
            'August',
            'September',
            'October',
            'November',
            'December'
        ]
        labels = labels[:this_month_num]
        default_items = [Sale.count_for(Sale, i) for i in range(1, 12)][:this_month_num + 1]
        data = {
            "labels": labels,
            "default": default_items
        }
        return Response(data)


class BookSerializer:
    def serialize(self, book, format_type):
        serializer = serialize(book, format_type)
        if serializer:
            return serializer(book)
        else:
            return None


def get_serializer(func):
    def wrapper(book, format_type):
        if format_type == 'json':
            def serialize_to_json(book):
                data = serializers.serialize("json", book)
                return data
            return serialize_to_json
        elif format_type == 'xml':
            def serialize_to_xml(book):
                data = serializers.serialize("xml", book)
                return data
            return serialize_to_xml
        elif format_type == 'yaml':
            def serialize_to_yaml(book):
                data = serializers.serialize("yaml", book)
                return data
            return serialize_to_yaml
        else:
            return None
    return wrapper


@get_serializer
def serialize(book, format_type):
    # serializing to xml, json and yaml
    pass


# class Iterator:


def api_get(request, book, format_type):
    book = Book.objects.filter(name=book)
    if book:
        data = BookSerializer().serialize(book, format_type)
        if data:
            return render(request, 'api/api_result.html', {
                'serialized_data': data
            })
    return render(request, 'api/api_result.html', {
        'serialized_data': 'API matching error'
    })