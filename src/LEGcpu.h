#ifndef __LEGCPU_H__
#define __LEGCPU_H__

#include <stdint.h>
#include <stdio.h>

#define PUBLIC(type) type

typedef enum
{
    SIGNED,
    UNSIGNED,
    HEX,
    BINARY,
} ftype;

typedef union
{
    uint8_t ui_value;
    int8_t si_value;
    struct
    {
        uint8_t f0 : 1, f1 : 1, f2 : 1, f3 : 1, f4 : 1, f5 : 1, f6 : 1, f7 : 1;
    };
    struct
    {
        uint8_t tail : 3, xx : 2, cond : 1, imm2 : 1, imm1 : 1;
    }; // use to determine code
    struct
    {
        uint8_t the_tail : 1, func : 4, the_head : 3;
    }; // use to determine code which use function
} bit8_t;

typedef struct
{
    bit8_t input;
    bit8_t output;

    bit8_t reg[14];

    bit8_t counter;
    bit8_t program[256];

    bit8_t memory[256];

    bit8_t stack[256];
    bit8_t stack_top;
} monitored;

PUBLIC(extern monitored) legcpu;

PUBLIC(void) printbit8(bit8_t value, ftype format);
PUBLIC(void) print_program(ftype format);
PUBLIC(void) print_memory(ftype format);
PUBLIC(void) print_stack(ftype format);
PUBLIC(void) import_program_from_file(FILE* file);
PUBLIC(void) export_program_to_file(FILE* file);
PUBLIC(void) execute_ticks(int tick);
PUBLIC(void)
add_code(uint8_t opcode, uint8_t attr1, uint8_t attr2, uint8_t addr);
PUBLIC(void) reset_cpu();

#endif
