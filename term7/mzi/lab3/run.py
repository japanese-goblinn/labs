import rsa


def file_info(filename="file.txt"):
    with open(filename) as file_handler:
        text = file_handler.readline().strip()
        return text


def main():
    pub_key, pr_key = rsa.generate_rsa_key_pair(19, 23)
    text = file_info()
    enc_text = rsa.rsa_encrypt(pub_key, text)
    print("\033[95m\nENCODED:\033[0m ", ''.join(str(i) for i in enc_text))
    dec_text = rsa.rsa_decrypt(pr_key, enc_text)
    print("\033[95m\nDECODED:\033[0m ", dec_text)


if __name__ == "__main__":
    main()
