def read_text_and_key(filename=None):
    assert filename != None, "You should provide filename"
    with open(filename) as file_handler: 
        text = file_handler.readline().strip()
        key = file_handler.readline()
        return text, key

def read_text(filename=None):
    assert filename != None, "You should provide filename"
    with open(filename) as file_handler: 
        text = file_handler.readline().strip()
        return text