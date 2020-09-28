from stb import STB, PLAIN, BLOCK_LINKING, GAMMING, COUNTER


def file_info(filename="file.txt"):
    with open(filename) as file_handler: 
        text = file_handler.readline().strip()
        key = file_handler.readline()
        return text, key


def print_stb(stb, text, title, s_link=None):
    print(f"\033[95m\n" + title + f"\n{len(title) * '-'}\033[0m")
    encoded = stb.encode(text, synchro_link=s_link)
    print("\nENCODED: ", encoded)
    decoded = stb.decode(encoded, synchro_link=s_link)
    print("\nDECODED: ", decoded)


def main():
    text, key = file_info()
    synchro_link = 16
    print_stb(
        STB(key, mode=PLAIN), 
        text=text, 
        title="В режиме простой замены"
    )
    print_stb(
        STB(key, mode=BLOCK_LINKING), 
        text=text, 
        title="В режиме сцепления блоков",
        s_link=synchro_link
    )
    print_stb(
        STB(key, mode=GAMMING), 
        text=text, 
        title="В режиме гаммирования с обратной связью",
        s_link=synchro_link
    )
    print_stb(
        STB(key, mode=COUNTER), 
        text=text, 
        title="В режиме счетчика",
        s_link=synchro_link
    )
    

if __name__ == "__main__":
    main()