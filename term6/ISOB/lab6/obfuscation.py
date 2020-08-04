import os
import sys
import subprocess
from pathlib import Path
import random
import string
from shutil import copyfile


def generate_funcs(src, dst, amount, func_name_lenth):
    copyfile(src, dst)
    with open(dst, "a+") as f: 
        for _ in range(amount):
            func_name = ''.join(random.choices(string.ascii_lowercase, k=func_name_lenth))
            f.write(f"\n\ndef {func_name}():")
            f.write(f"\n    return '{func_name}'")


def main(dir):
    s = subprocess.Popen([f"pyminifier -O -o obf_inst0.py {dir}"], shell=True)
    s.wait()
    
    s1 = subprocess.Popen([f"pyminifier --nonlatin --replacement-length=10 -O -o obf_inst1.py {dir}"], shell=True)
    s1.wait()

    generate_funcs(src="installer.py", dst="obf_inst2.py", amount=100, func_name_lenth=40)

    generate_funcs(src="installer.py", dst="obf_inst12.py", amount=100, func_name_lenth=40)
    s2 = subprocess.Popen([f"pyminifier -O -o obf_inst12.py obf_inst12.py"], shell=True)
    s2.wait()

    generate_funcs(src="installer.py", dst="obf_inst012.py", amount=100, func_name_lenth=40)
    s3 = subprocess.Popen([f"pyminifier -O -o obf_inst012.py obf_inst012.py"], shell=True)
    s3.wait()
    s4 = subprocess.Popen([f"pyminifier --nonlatin --replacement-length=10 -O -o obf_inst012.py obf_inst012.py"], shell=True)
    s4.wait()


if __name__ == "__main__":
    try:
        dir_name = sys.argv[1]
    except:
        print("No directory provided")
    main(dir=dir_name)