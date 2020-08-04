import json
from django.http import HttpResponse, JsonResponse
from django.db import connection
from django.shortcuts import render
from django.views.decorators.csrf import csrf_exempt


def dict_for(cursor):
    columns = [col[0] for col in cursor.description]
    return [dict(zip(columns, row)) for row in cursor.fetchall()]


@csrf_exempt
def execute_query(request):
    with connection.cursor() as cursor:
        try:
            cursor.execute(json.loads(request.body))
            try: 
                return JsonResponse(dict_for(cursor), safe=False)
            except:
                return JsonResponse("OK", safe=False)
        except Exception as e:
            return JsonResponse({ 
                "error": str(e) 
            })


@csrf_exempt
def execute_procedure(request):
    with connection.cursor() as cursor:
        json_data = json.loads(request.body)
        try:
            cursor.callproc(json_data["procedureName"], json_data["params"])
            try: 
                return JsonResponse(dict_for(cursor), safe=False)
            except:
                return JsonResponse("OK", safe=False)
        except Exception as e:
            return JsonResponse({ 
                "error": str(e) 
            })