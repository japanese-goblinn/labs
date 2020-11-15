import random
from hashlib import sha256


def co_prime(a, b):
    while b != 0:
        a, b = b, a % b
    return a


def extended_gcd(aa, bb):
    lastremainder, remainder = abs(aa), abs(bb)
    x, lastx, y, lasty = 0, 1, 1, 0
    while remainder:
        lastremainder, (quotient, remainder) = remainder, divmod(lastremainder, remainder)
        x, lastx = lastx - quotient * x, x
        y, lasty = lasty - quotient * y, y
    return lastremainder, lastx * (-1 if aa < 0 else 1), lasty * (-1 if bb < 0 else 1)


def mod_inv(a, m):
    g, x, y = extended_gcd(a, m)
    if g != 1:
        raise Exception('Modular inverse does not exist')
    return x % m


def is_prime(num):
    if num == 2:
        return True
    if num < 2 or num % 2 == 0:
        return False
    for n in range(3, int(num ** 0.5) + 2, 2):
        if num % n == 0:
            return False
    return True


def generate_key_pair(p, q):
    if not (is_prime(p) and is_prime(q)):
        raise ValueError('Both numbers must be prime.')
    elif p == q:
        raise ValueError('p and q cannot be equal')

    n = p * q
    phi = (p - 1) * (q - 1)
    e = random.randrange(1, phi)
    g = co_prime(e, phi)

    while g != 1:
        e = random.randrange(1, phi)
        g = co_prime(e, phi)

    d = mod_inv(e, phi)
    return (e, n), (d, n)


def encode(private_key, hashed_text):
    key, n = private_key
    return [pow(ord(char), key, n) for char in hashed_text]


def decode(public_key, cipher_text):
    key, n = public_key
    return ''.join([chr(pow(char, key, n)) for char in cipher_text])


def hash_function(message):
    hashed = sha256(message.encode("UTF-8")).hexdigest()
    return hashed


def verify(received_hashed, message):
    our_hashed = hash_function(message)
    if received_hashed == our_hashed:
        return True
    else:
        return False
