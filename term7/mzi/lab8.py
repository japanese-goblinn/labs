from helpers.colored_text import colored_text
from helpers.file_info import read_text
from algorithms import sten

BASE_PATH = "resources/lab8/"
IMG_PATH = BASE_PATH + "img.png"
ENCODE_PATH = BASE_PATH + "img_enc.png"

def encode():
    text_to_encode = "Hello World !!!"
    print(colored_text("TEXT TO ENCODE:"), text_to_encode)
    print(colored_text("ENCODING..."))
    sten.encode(message=text_to_encode, img_path=IMG_PATH, encode_img_path=ENCODE_PATH)
    print(colored_text("ENCODED IMAGE CREATED"))


def decode():
    print(colored_text("DECODING..."))
    text = sten.decode(img_path=ENCODE_PATH)
    print(colored_text("IMAGE DECODED"))
    print(colored_text("TEXT FROM IMAGE:"), text)


def main():
    encode()
    decode()


if __name__ == "__main__":
    main()