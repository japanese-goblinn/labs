from library import Form

def show_form(form: Form):
        print('Registration Form:\n')
        for i, field in enumerate(form.__dict__):
            print(f'{i+1}.{field}: {getattr(form, field)}\n')