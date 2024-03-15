#ifndef __LEGCPU_ASSEMBLY_CODE_H
#define __LEGCPU_ASSEMBLY_CODE_H

#define NOP 0b00000000

#define IMM1 0b10000000
#define IMM2 0b01000000
#define IMM12 0b11000000

#define ADD 0b00000000
#define SUB 0b00000001
#define AND 0b00000010
#define OR 0b00000011
#define NOT 0b00000100
#define XOR 0b00000101
#define SHL 0b00000110
#define SHR 0b00000111

#define IF_EQ 0b00100000
#define IF_N_EQ 0b00100001
#define IF_L 0b00100010
#define IF_L_EQ 0b00100011
#define IF_G 0b00100100
#define IF_G_EQ 0b00100101

#define REG0 0
#define REG1 1
#define REG2 2
#define REG3 3
#define REG4 4
#define REG5 5
#define COUNTER 6
#define INPUT 7
#define OUTPUT 7
#define REG6 8
#define REG7 9
#define REG8 10
#define REG9 11
#define REG10 12
#define REG11 13
#define REG12 14
#define MEMADDR 15

#define SAVE 0b00010000
#define LOAD 0b00001000
#define PUSH 0b00011000
#define POP 0b00001001

#define CALL 0b00001010
#define RET 0b00001011

#endif
