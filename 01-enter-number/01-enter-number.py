#!/usr/bin/env python3
import sys

try:
    a = int(input("Введите число: "))
except:
    sys.exit(1)
txt = f"Ваше число: {a}" 
print(txt)
