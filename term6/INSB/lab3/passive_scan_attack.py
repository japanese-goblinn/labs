import socket
from datetime import datetime


def scanner(socket, ip, port):
    try:
        socket.connect((ip, port))
        return True
    except:
        return False
    

def main():
    target_ip = input("Enter target IP: ")
    t1 = datetime.now()
    for port in range(0, 5001):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            if scanner(s, target_ip, port):
                print(f"Port {port} is opened")
    print(f"Scanning finished in {datetime.now() - t1}")
    

if __name__ == "__main__":
    main()

