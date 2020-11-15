import random

from algorithms import digital_signature
from helpers.colored_text import colored_text
from helpers.file_info import read_text

primes = [i for i in range(0, 1000) if digital_signature.is_prime(i)]


def main():
    p, q = random.sample(primes, 2)
    public_key, private_key = digital_signature.generate_key_pair(p, q)
    text = read_text(filename="resources/lab6.txt")
    print(colored_text("TEXT:"), text)
    print(colored_text("TEXT HASH:"), digital_signature.hash_function(text))
    encoded = digital_signature.encode(
        private_key=private_key, 
        hashed_text=digital_signature.hash_function(text)
    )
    print(colored_text("ENCODED:"), ''.join(map(lambda x: str(x), encoded)))

    decoded = digital_signature.decode(
        public_key=public_key, 
        cipher_text=encoded
    )
    print(colored_text("DECODED HASH:"), decoded)

    print(colored_text("IS VERIFIED:"), digital_signature.verify(decoded, text))


if __name__ == "__main__":
    main()
