import random
from helpers import elgamal
from helpers.colored_text import *
from helpers.file_info import read_text

def main():
    q = random.randint(pow(10, 20), pow(10, 50))
    g = random.randint(2, q)

    key = elgamal.gen_key(q) 
    h = elgamal.power(g, key, q)

    text = read_text(filename="resources/lab4.txt")
    encoded_text, p = elgamal.encode(text, q, h, g)
    print(colored_text("ENCODED:"), ''.join(str(i) for i in encoded_text))

    decoded_text = elgamal.decode(encoded_text, p, key, q)
    print(colored_text("DECODED:"), "".join(decoded_text))



if __name__ == "__main__":
    main()
