/*
    Tiny 8-bit CPU
    ----------------

    Registers:
        A B C D (8-bit)

    Instruction encoding:

    MOVI:
        0IIIIIII
        load immediate (0..127) into D

    Other instructions:
        1XXXYYZZ

        XXX = opcode
        YY  = destination register
        ZZ  = source register

    Opcodes:
        000 ADD
        001 SUB
        010 MUL
        011 DIV
        100 IN
        101 OUT

    Example assembly:

        MOVI 10
        ADD A,B
        OUT A
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PROGRAM_SIZE 256

typedef uint8_t u8;

// CPU

typedef struct {
    u8 A;
    u8 B;
    u8 C;
    u8 D;
} CPU;

// Registers

enum {
    REG_A = 0,
    REG_B = 1,
    REG_C = 2,
    REG_D = 3
};

static u8* get_reg(CPU* cpu, int r)
{
    switch (r) {
        case REG_A: return &cpu->A;
        case REG_B: return &cpu->B;
        case REG_C: return &cpu->C;
        case REG_D: return &cpu->D;
    }

    return NULL;
}

static int reg_from_char(char c)
{
    switch (toupper((unsigned char)c)) {
        case 'A': return REG_A;
        case 'B': return REG_B;
        case 'C': return REG_C;
        case 'D': return REG_D;
    }

    return -1;
}

// Opcodes

enum {
    OP_ADD = 0,
    OP_SUB = 1,
    OP_MUL = 2,
    OP_DIV = 3,
    OP_IN  = 4,
    OP_OUT = 5
};

// Encoding

static u8 encode_movi(u8 imm)
{
    return imm & 0x7F;
}

static u8 encode_other(u8 opcode, u8 dst, u8 src)
{
    return 0x80 |
           ((opcode & 0x07) << 4) |
           ((dst    & 0x03) << 2) |
           (src    & 0x03);
}

// Assembler

static int starts_with(const char* s, const char* prefix)
{
    while (*prefix) {
        if (toupper((unsigned char)*s) !=
            toupper((unsigned char)*prefix))
            return 0;

        s++;
        prefix++;
    }

    return 1;
}

static u8 assemble_line(const char* line)
{
    if (starts_with(line, "MOVI")) {
        int imm = atoi(line + 5);

        return encode_movi((u8)imm);
    }

    char op[16];
    char rd_char;
    char rs_char;

    if (sscanf(line, "%15s %c,%c", op, &rd_char, &rs_char) >= 2) {
        int rd = reg_from_char(rd_char);
        int rs = reg_from_char(rs_char);

        if (strcmp(op, "ADD") == 0)
            return encode_other(OP_ADD, rd, rs);

        if (strcmp(op, "SUB") == 0)
            return encode_other(OP_SUB, rd, rs);

        if (strcmp(op, "MUL") == 0)
            return encode_other(OP_MUL, rd, rs);

        if (strcmp(op, "DIV") == 0)
            return encode_other(OP_DIV, rd, rs);

        if (strcmp(op, "IN") == 0)
            return encode_other(OP_IN, rd, 0);

        if (strcmp(op, "OUT") == 0)
            return encode_other(OP_OUT, 0, rd);
    }

    fprintf(stderr, "Assembler error: %s\n", line);
    exit(1);
}

// Disassembler

static char reg_to_char(int r)
{
    return "ABCD"[r & 3];
}

static void disassemble(u8 instr)
{
    if ((instr & 0x80) == 0) {
        printf("MOVI %u\n", instr & 0x7F);
        return;
    }

    u8 opcode = (instr >> 4) & 0x07;
    u8 dst    = (instr >> 2) & 0x03;
    u8 src    = instr & 0x03;

    switch (opcode) {

        case OP_ADD:
            printf("ADD %c,%c\n",
                   reg_to_char(dst),
                   reg_to_char(src));
            break;

        case OP_SUB:
            printf("SUB %c,%c\n",
                   reg_to_char(dst),
                   reg_to_char(src));
            break;

        case OP_MUL:
            printf("MUL %c,%c\n",
                   reg_to_char(dst),
                   reg_to_char(src));
            break;

        case OP_DIV:
            printf("DIV %c,%c\n",
                   reg_to_char(dst),
                   reg_to_char(src));
            break;

        case OP_IN:
            printf("IN %c\n",
                   reg_to_char(dst));
            break;

        case OP_OUT:
            printf("OUT %c\n",
                   reg_to_char(src));
            break;

        default:
            printf("UNKNOWN\n");
    }
}

// Simulator

static void dump_cpu(CPU* cpu)
{
    printf("A=%3u B=%3u C=%3u D=%3u\n",
           cpu->A,
           cpu->B,
           cpu->C,
           cpu->D);
}

static void execute(CPU* cpu, u8 instr)
{
    if ((instr & 0x80) == 0) {
        cpu->D = instr & 0x7F;
        return;
    }

    u8 opcode = (instr >> 4) & 0x07;
    u8 dst    = (instr >> 2) & 0x03;
    u8 src    = instr & 0x03;

    u8* rd = get_reg(cpu, dst);
    u8* rs = get_reg(cpu, src);

    switch (opcode) {

        case OP_ADD:
            *rd = *rd + *rs;
            break;

        case OP_SUB:
            *rd = *rd - *rs;
            break;

        case OP_MUL:
            *rd = *rd * *rs;
            break;

        case OP_DIV:
            if (*rs != 0)
                *rd = *rd / *rs;
            break;

        case OP_IN:
        {
            int x;

            printf("INPUT %c: ", reg_to_char(dst));
            scanf("%d", &x);

            *rd = (u8)x;
            break;
        }

        case OP_OUT:
            printf("OUT: %u\n", *rs);
            break;
    }
}

// Demo

int main(void)
{
    const char* asm_program[] = {
        "IN A",
        "IN B",
        "ADD A,B",
        "OUT A"
    };

    u8 program[PROGRAM_SIZE];
    size_t program_size = 0;

    CPU cpu = {0};

    printf("ASSEMBLING:\n\n");

    for (size_t i = 0; i < 4; i++) {
        u8 code = assemble_line(asm_program[i]);
        program[program_size++] = code;
        printf("%-10s -> 0x%02X\n",
               asm_program[i],
               code);
    }

    printf("\nDISASSEMBLY:\n\n");

    for (size_t i = 0; i < program_size; i++) {
        printf("0x%02X : ", program[i]);
        disassemble(program[i]);
    }

    printf("\nEXECUTION:\n\n");

    for (size_t pc = 0; pc < program_size; pc++) {
        printf("PC=%zu  ", pc);
        disassemble(program[pc]);
        execute(&cpu, program[pc]);
        dump_cpu(&cpu);
        printf("\n");
    }

    return 0;
}