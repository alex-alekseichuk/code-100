#include <stdio.h>
#include <stdlib.h>

#include "../common/common.h"
#include "cpu.h"

static char reg_to_char(int r) {
    return "ABCD"[r & 3];
}

static void disassemble(u8 byte) {
    u8 opcode = (byte & 0x80) == 0 ? OP_MOVI : (byte >> 4);
    u8 dst    = (byte >> 2) & 0x03;
    u8 src    = byte & 0x03;

    switch (opcode) {
    case OP_MOVI:
        printf("MOVI %u\n", byte & 0x7F);
        break;

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
               reg_to_char(dst));
        break;

    default:
        printf("UNKNOWN\n");
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
    while (EOF != (c = fgetc(f))) {
        u8 byte = c & 0xFF;
        disassemble(byte);
    }

    fclose(f);

    return 0;
}
