import struct
from enum import Enum
from math import (
    floor,
    sin,
)

from bitarray import bitarray


class MD5Buffer(Enum):
    A = 0x67452301
    B = 0xEFCDAB89
    C = 0x98BADCFE
    D = 0x10325476


class MD5(object):
    _string = None
    _buffers = {
        MD5Buffer.A: None,
        MD5Buffer.B: None,
        MD5Buffer.C: None,
        MD5Buffer.D: None,
    }

    @classmethod
    def hash(cls, string):
        cls._string = string

        preprocessed_bit_array = cls._step_2(cls._step_1())
        cls._step_3()
        cls._step_4(preprocessed_bit_array)
        return cls._step_5()

    @classmethod
    def _step_1(cls):
        bit_array = bitarray(endian="big")
        bit_array.frombytes(cls._string.encode("utf-8"))
        bit_array.append(1)
        while bit_array.length() % 512 != 448:
            bit_array.append(0)
        return bitarray(bit_array, endian="little")

    @classmethod
    def _step_2(cls, step_1_result):
        length = (len(cls._string) * 8) % pow(2, 64)
        length_bit_array = bitarray(endian="little")
        length_bit_array.frombytes(struct.pack("<Q", length))

        result = step_1_result.copy()
        result.extend(length_bit_array)
        return result

    @classmethod
    def _step_3(cls):
        for buffer_type in cls._buffers.keys():
            cls._buffers[buffer_type] = buffer_type.value

    @classmethod
    def _step_4(cls, step_2_result):
        def F(x, y, z): return (x & y) | (~x & z)
        def G(x, y, z): return (x & z) | (y & ~z)
        def H(x, y, z): return x ^ y ^ z
        def I(x, y, z): return y ^ (x | ~z)

        def rotate_left(x, n): return (x << n) | (x >> (32 - n))
        def modular_add(a, b): return (a + b) % pow(2, 32)

        T = [floor(pow(2, 32) * abs(sin(i + 1))) for i in range(64)]

        N = len(step_2_result) // 32

        for chunk_index in range(N // 16):
            start = chunk_index * 512
            X = [step_2_result[start + (x * 32): start + (x * 32) + 32]
                 for x in range(16)]
            X = [int.from_bytes(word.tobytes(), byteorder="little")
                 for word in X]
            A = cls._buffers[MD5Buffer.A]
            B = cls._buffers[MD5Buffer.B]
            C = cls._buffers[MD5Buffer.C]
            D = cls._buffers[MD5Buffer.D]

            for i in range(4 * 16):
                if 0 <= i <= 15:
                    k = i
                    s = [7, 12, 17, 22]
                    temp = F(B, C, D)
                elif 16 <= i <= 31:
                    k = ((5 * i) + 1) % 16
                    s = [5, 9, 14, 20]
                    temp = G(B, C, D)
                elif 32 <= i <= 47:
                    k = ((3 * i) + 5) % 16
                    s = [4, 11, 16, 23]
                    temp = H(B, C, D)
                elif 48 <= i <= 63:
                    k = (7 * i) % 16
                    s = [6, 10, 15, 21]
                    temp = I(B, C, D)

                temp = modular_add(temp, X[k])
                temp = modular_add(temp, T[i])
                temp = modular_add(temp, A)
                temp = rotate_left(temp, s[i % 4])
                temp = modular_add(temp, B)

                A = D
                D = C
                C = B
                B = temp

            cls._buffers[MD5Buffer.A] = modular_add(
                cls._buffers[MD5Buffer.A], A)
            cls._buffers[MD5Buffer.B] = modular_add(
                cls._buffers[MD5Buffer.B], B)
            cls._buffers[MD5Buffer.C] = modular_add(
                cls._buffers[MD5Buffer.C], C)
            cls._buffers[MD5Buffer.D] = modular_add(
                cls._buffers[MD5Buffer.D], D)

    @classmethod
    def _step_5(cls):
        A = struct.unpack("<I", struct.pack(
            ">I", cls._buffers[MD5Buffer.A]))[0]
        B = struct.unpack("<I", struct.pack(
            ">I", cls._buffers[MD5Buffer.B]))[0]
        C = struct.unpack("<I", struct.pack(
            ">I", cls._buffers[MD5Buffer.C]))[0]
        D = struct.unpack("<I", struct.pack(
            ">I", cls._buffers[MD5Buffer.D]))[0]

        return f"{format(A, '08x')}{format(B, '08x')}{format(C, '08x')}{format(D, '08x')}"
