from algorithms import rsa
from helpers.colored_text import *
from helpers.file_info import read_text


def main():
    pub_key, pr_key = rsa.generate_rsa_key_pair(19, 23)
    text = read_text(filename="resources/lab3.txt")
    enc_text = rsa.rsa_encrypt(pub_key, text)
    print(colored_text("ENCODED:"), ''.join(str(i) for i in enc_text))
    dec_text = rsa.rsa_decrypt(pr_key, enc_text)
    print(colored_text("DECODED:"), dec_text)


if __name__ == "__main__":
    main()
