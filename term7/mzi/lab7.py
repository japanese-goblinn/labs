from algorithms import ellipticcurve
from helpers.colored_text import colored_text
from helpers.file_info import read_text
from algorithms.ellipticcurve.ecdsa import Ecdsa
from algorithms.ellipticcurve.privateKey import PrivateKey


def main():
    text = read_text(filename="resources/lab7.txt")
    print(colored_text("TEXT:"), text)

    private_key = PrivateKey()
    public_key = private_key.publicKey()
    print(colored_text("PRIVATE KEY"), private_key.toPem())
    print(colored_text("PUBLIC KEY"), public_key.toPem())
    
    signature = Ecdsa.sign(text, private_key)
    print(colored_text("SIGNATURE:"), signature.toBase64())
    
    print(colored_text("IS SIGNATURE VERIFIED:"), Ecdsa.verify(text, signature, public_key))


if __name__ == "__main__":
    main()
