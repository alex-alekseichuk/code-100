#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../common/common.h"
#include "cpu.h"

#define ERR_INSTR (instr_t){.opcode = OP_ERR}

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

static ops_t parse_ops(const char *args) {
    char rd_char, rs_char;

    if (sscanf(args, "%c,%c", &rd_char, &rs_char) == 2) {
        return (ops_t){
            .rd = reg_from_char(rd_char),
            .rs = reg_from_char(rs_char)
        };
    }

    return (ops_t){.rd = -1, .rs = -1};
}

static instr_t parse_ops_instr(const opcode_t opcode, const char* args) {
    const ops_t ops = parse_ops(args);
    if (ops.rd == -1 || ops.rs == -1) return ERR_INSTR;
    return (instr_t){.opcode = opcode, .operand = {.ops = ops}};
}

static int parse_rop(const char *args) {
    char rop_char;

    if (sscanf(args, "%c", &rop_char) == 1) {
        return reg_from_char(rop_char);
    }

    return -1;
}

static instr_t parse_rop_instr(const opcode_t opcode, const char* args) {
    const int rop = parse_rop(args);
    if (rop == -1) return ERR_INSTR;
    return (instr_t){.opcode = opcode, .operand = {.rop = rop}};
}

static instr_t parse_instr(const char* line) {
    char op[16];

    if (sscanf(line, "%15s ", op) != 1) {
        print_log("Can't read mnemonic");

        return ERR_INSTR;
    }

    const char *args = line + strlen(op) + 1;

    if (!strcmp(op, "MOVI")) return (instr_t){.opcode = OP_MOVI, .operand = {.imm = atoi(args)}};

    if (!strcmp(op, "ADD")) return parse_ops_instr(OP_ADD, args);
    if (!strcmp(op, "SUB")) return parse_ops_instr(OP_SUB, args);
    if (!strcmp(op, "MUL")) return parse_ops_instr(OP_MUL, args);
    if (!strcmp(op, "DIV")) return parse_ops_instr(OP_DIV, args);

    if (!strcmp(op, "IN")) return parse_rop_instr(OP_IN, args);
    if (!strcmp(op, "OUT")) return parse_rop_instr(OP_OUT, args);

    print_log("Unknown mnemonic: %s", op);

    return ERR_INSTR;
}

static u8 encode(const instr_t instr)
{
    switch (instr.opcode) {
    case OP_MOVI:
        return instr.operand.imm & 0x7F;
    case OP_ADD:
    case OP_SUB:
    case OP_MUL:
    case OP_DIV:
        return (instr.opcode << 4) | ((instr.operand.ops.rd & 0x03) << 2) | (instr.operand.ops.rs & 0x03);
    case OP_IN:
    case OP_OUT:
        return (instr.opcode << 4) | ((instr.operand.rop & 0x03) << 2);
    default:
        return OP_ERR << 4; // unknown instruction
    }
}

static u8 assemble_line(const char* line) {
    instr_t instr = parse_instr(line);

    if (instr.opcode == OP_ERR) {
        print_log("Assembler error: %s", line);
        exit(1);
    }

    return encode(instr);
}

#define MAX 256

int main(int argc, char *argv[]) {
    char *line = NULL;
    size_t len = 0;

    u8 byte_code[MAX];
    size_t counter = 0;

    const char* filename = "./a.byte_code";

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-o")) {
            if (i + 1 < argc) {
                filename = argv[++i];
            } else {
                print_log("Error: Missing argument for -o option");
                exit(3);
            }
        }
    }

    while (-1 != read_line(&line, &len, stdin) && counter < MAX) {
        byte_code[counter++] = assemble_line(line);
    }
    free(line);

    if (counter >= MAX) {
        print_log("Too big program! Maximum allowed size is %d bytes", MAX);
        exit(1);
    }

    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        print_log("Error opening file %s", filename);
        exit(2);
    }

    fwrite(byte_code, sizeof(u8), counter, f);
    fclose(f);

    return 0;
}
