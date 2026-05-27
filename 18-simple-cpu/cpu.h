#pragma once
#include <stdint.h>

typedef uint8_t u8;

// Registers

typedef enum reg_t {
    REG_A = 0,
    REG_B,
    REG_C,
    REG_D,
    REG_LAST
} reg_t;

// Opcodes

typedef enum opcode_t {
    OP_MOVI = 0,
    OP_ADD = 8,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_IN,
    OP_OUT,
    OP_ERR,
} opcode_t;

typedef struct ops_t {
    int rd, rs;
} ops_t;

typedef union operand_t {
    ops_t ops;
    int rop;
    int imm;
} operand_t;

typedef struct instr_t {
    opcode_t opcode;
    operand_t operand;
} instr_t;
