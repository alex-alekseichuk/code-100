#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

typedef struct {
    u8 A;
    u8 B;
    u8 C;
    u8 D;
} CPU;

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

static char reg_to_char(int r) {
    return "ABCD"[r & 3];
}

static void dump_cpu(CPU* cpu)
{
    printf("A=%3u B=%3u C=%3u D=%3u\n",
           cpu->A,
           cpu->B,
           cpu->C,
           cpu->D);
}

static void execute(CPU* cpu, u8 byte)
{
    if ((byte & 0x80) == 0) {
        cpu->D = byte & 0x7F;
        return;
    }

    u8 opcode = byte >> 4;
    u8 dst    = (byte >> 2) & 0x03;
    u8 src    = byte & 0x03;

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
        printf("OUT: %u\n", *rd);
        break;
    }
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        print_log("Error: Missing argument input file with program byte code");
        exit(1);
    }

    const char* filename = argv[1];

    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        print_log("Error opening file %s", filename);
        exit(2);
    }

    int c;
    size_t pc = 0;
    CPU cpu = {0};
    while (EOF != (c = fgetc(f))) {
        u8 byte = c & 0xFF;
        printf("PC=%zu  ", pc);
        // disassemble(byte);
        execute(&cpu, byte);
        dump_cpu(&cpu);
        printf("\n");
    }

    fclose(f);

    return 0;
}
