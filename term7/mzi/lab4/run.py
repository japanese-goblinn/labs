import random
import elgamal


def file_info(filename="file.txt"):
    with open(filename) as file_handler:
        text = file_handler.readline().strip()
        return text


def main():
    q = random.randint(pow(10, 20), pow(10, 50))
    g = random.randint(2, q)

    key = elgamal.gen_key(q) 
    h = elgamal.power(g, key, q)

    text = file_info()
    encoded_text, p = elgamal.encode(text, q, h, g)
    print("\033[95m\nENCODED:\033[0m ", ''.join(str(i) for i in encoded_text))

    decoded_text = elgamal.decode(encoded_text, p, key, q)
    print("\033[95m\nDECODED:\033[0m ", "".join(decoded_text))



if __name__ == "__main__":
    main()
