ALU / calculator

                               +-- keyboard
                               |
programm --[load]--> PseudoMK--+
|
+-- screen

4 registers 8-bits: A, B, C, D

MOVI D, i
ADD rx, rs
SUB rx, rs
MUL rx, rs
DIV rx, rs
IN rd
OUT rs

All opcodes are 8 bits long.

First bit is a flag:
0... - move instruction
1... - other instructions

0...
MOVI 7 bits for int value
1XXX-YY-ZZ
other commands

XXX - opcode for non-move instructions (3 bits)
YY  - destination register (2 bits)
ZZ  - source register (2 bits)

