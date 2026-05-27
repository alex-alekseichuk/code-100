cmake_minimum_required(VERSION 3.28)
project(cpu C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(assembler
        assembler.c
        ../common/common.c
        cpu.c
)

add_executable(disassembler
    disassembler.c
    ../common/common.c
    cpu.c
)

add_executable(simulator
    simulator.c
    ../common/common.c
    cpu.c
)
