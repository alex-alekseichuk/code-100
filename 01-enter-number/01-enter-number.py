#!/usr/bin/env python3
import sys

try:
    a = int(input("Enter number: "))
except:
    sys.exit(1)
txt = f"The number: {a}"
print(txt)
