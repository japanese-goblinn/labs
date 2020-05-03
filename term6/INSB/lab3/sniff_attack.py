from scapy.all import *
from scapy.layers.http import HTTPRequest
from colorama import init, Fore

init()

GREEN = Fore.GREEN
RED   = Fore.RED
RESET = Fore.RESET


def process_packet(packet):
    if packet.haslayer(HTTPRequest):
        url = packet[HTTPRequest].Host.decode() + packet[HTTPRequest].Path.decode()
        ip = packet[IP].src
        method = packet[HTTPRequest].Method.decode()
        print(f"\n{GREEN}[+] Requested {url} with {method}{RESET}")
        if show_raw and packet.haslayer(Raw) and method == "POST":
            print(f"\n{RED}[*] Some useful Raw data: {packet[Raw].load}{RESET}")


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("--show-raw", dest="show_raw", action="store_true")
    args = parser.parse_args()
    show_raw = args.show_raw
    sniff(prn=process_packet, store=False)