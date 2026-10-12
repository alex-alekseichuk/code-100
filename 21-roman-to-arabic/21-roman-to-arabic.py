#!/usr/bin/env python3

from sys import exit

latin_num = input()
lat2ar = {
    "I": 1,
    "V": 5,
    "X": 10,
    "L": 50,
    "C": 100,
    "D": 500,
    "M": 1000,
}

res = 0
n = len(latin_num)

for i in range(n):
    ch = latin_num[i]

    if ch not in lat2ar:
        print(f'Unknow digit: {ch}')
        exit(1)

    d = lat2ar[ch]

    if i == n - 1:
        res += d
        break

    next_ch = latin_num[i+1]
    if next_ch not in lat2ar:
        print(f'Unknow digit: {next_ch}')
        exit(1)

    next_d = lat2ar[next_ch]

    if d < next_d:
        res -= d
    else:
        res += d

print(res + 1)

