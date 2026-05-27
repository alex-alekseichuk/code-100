Simple 8-bit CPU
----------------

4 registers:
    A B C D (8-bit)

7 instructions:
```
MOVI D, i
ADD rx, rs
SUB rx, rs
MUL rx, rs
DIV rx, rs
IN rd
OUT rs
```

Instruction encoding:

MOVI: `0XXXXXXX`
load immediate (0..127) into D

Other instructions: `1XXXYYZZ`
```
    XXX = opcode
    YY  = destination register
    ZZ  = source register
```

Opcodes:
```
    000 ADD
    001 SUB
    010 MUL
    011 DIV
    100 IN
    101 OUT
```

Example assembly:
```
    MOVI 10
    ADD A,B
    OUT A
```

CPU / Calculator

```
                               +-- keyboard
                               |
programm --[load]--> PseudoMK--+
                               |
                               +-- screen
```

## Build
```bash
    mkdir build && cd build
    cmake ..
    make
    cd ..
```

## Assemble
```bash
./build/assembler -o example.bin < example.asm
```

## Disassemble
```bash
./build/disassembler example.bin
IN A
IN B
MOVI 42
ADD A,B
OUT A
```

## Run in simulator
```bash
./build/simulator example.bin
PC=0  INPUT A: 3
A=  3 B=  0 C=  0 D=  0

PC=0  INPUT B: 4
A=  3 B=  4 C=  0 D=  0

PC=0  A=  3 B=  4 C=  0 D= 42

PC=0  A=  7 B=  4 C=  0 D= 42

PC=0  OUT: 7
A=  7 B=  4 C=  0 D= 42
```
