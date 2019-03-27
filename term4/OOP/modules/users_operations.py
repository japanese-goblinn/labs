from typing import Dict
from library import User, Form


def get_user_form(user: User, register_info: list) -> Form:
    form = Form(*register_info)
    return form


def match_user_with_form(user: User, form: Form) -> Dict[User, Form]:
    forms_dict = {}
    forms_dict[user] = form
    return forms_dict