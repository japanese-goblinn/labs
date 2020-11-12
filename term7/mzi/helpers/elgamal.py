import random
from math import pow, gcd


def gen_key(q):
    key = random.randint(pow(10, 20), q)
    while gcd(q, key) != 1:
        key = random.randint(pow(10, 20), q)
    return key


def power(a, b, c):
    x = 1
    y = a
    while b > 0:
        if b % 2 == 0:
            x = (x * y) % c
        y = (y * y) % c
        b = int(b / 2)
    return x % c


def encode(message, q, h, g):
    en_msg = []
    k = gen_key(q) 
    s = power(h, k, q)
    p = power(g, k, q)
    for i in range(0, len(message)):
        en_msg.append(message[i])
    for i in range(0, len(en_msg)):
        en_msg[i] = s * ord(en_msg[i])
    return en_msg, p


def decode(en_message, p, key, q):
    dr_message = []
    h = power(p, key, q)
    for i in range(0, len(en_message)):
        dr_message.append(chr(int(en_message[i] / h)))
    return dr_message
