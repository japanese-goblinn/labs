from algorithms.md5 import MD5
from helpers.file_info import read_text
from helpers.colored_text import colored_text

def main():
    text = read_text(filename="resources/lab5.txt")
    print(colored_text("MESSAGE:"), text)
    print(colored_text("DIGEST:"), MD5.hash(text))


if __name__ == "__main__":
    main()
