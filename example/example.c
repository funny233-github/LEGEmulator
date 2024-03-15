#include "LEGcpu.h"
#include "LEGcpu_assembly_code.h"
#include <stdbool.h>

int
main()
{
    program[0].ui_value = ADD + IMM2;
    program[1].ui_value = REG0;
    program[2].ui_value = 20;
    program[3].ui_value = REG0;

    program[4].ui_value = SUB + IMM2;
    program[5].ui_value = REG0;
    program[6].ui_value = 15;
    program[7].ui_value = REG1;

    program[8].ui_value = IMM12;
    program[9].ui_value = 14;
    program[10].ui_value = 5;
    program[11].ui_value = MEMADDR;

    program[12].ui_value = SAVE + IMM12;
    program[13].ui_value = 114;
    program[14].ui_value = NOP;
    program[15].ui_value = NOP;

    program[16].ui_value = PUSH + IMM12;
    program[17].ui_value = 115;
    program[18].ui_value = NOP;
    program[19].ui_value = NOP;

    program[20].ui_value = PUSH + IMM12;
    program[21].ui_value = 12;
    program[22].ui_value = NOP;
    program[23].ui_value = NOP;

    program[24].ui_value = POP;
    program[25].ui_value = NOP;
    program[26].ui_value = NOP;
    program[27].ui_value = REG3;

    print_program(BINARY);

    execute_ticks(1);

    printf("\n");
    printbit8(reg[0], UNSIGNED);
    printf("\n");

    execute_ticks(1);
    printf("\n");
    printbit8(reg[1], UNSIGNED);
    printf("\n");

    execute_ticks(2);
    printbit8(reg[13], UNSIGNED);
    printf("\n");
    print_memory(UNSIGNED);

    execute_ticks(2);
    printf("\n");
    print_stack(UNSIGNED);
    printf("\n");

    execute_ticks(1);
    printf("\n");
    printbit8(reg[3], UNSIGNED);
    printf("\n");
}
