# Вариант 3
# С клавиатуры вводится расширение файла для поиска и имя результирующего файла. 
# Необходимо найти все файлы с этим расширением 
# и сохранить их имена в результирующем файле.

import os

def main(file_type, result_file):
    for file in os.listdir("."):
        if file.endswith(file_type):
            with open(result_file + ".txt", "a+") as f:
                f.write(file.split(".")[0])

if __name__ == "__main__":
    file_type, result_file = input().split()
    if file_type[0] != ".":
        print("Wrong file type")
        exit(9)
    main(file_type, result_file)