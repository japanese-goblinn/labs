import os,sys,inspect
current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parent_dir = os.path.dirname(current_dir)
sys.path.insert(0, parent_dir) 
from colored_text import colored_text

from md5 import MD5


def file_info(filename="file.txt"):
    with open(filename) as file_handler:
        text = file_handler.readline().strip()
        return text


def main():
    text = file_info()
    print(colored_text("MESSAGE:"), text)
    print(colored_text("DIGEST:"), MD5.hash(text))


if __name__ == "__main__":
    main()
