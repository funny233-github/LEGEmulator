#include "LEGcpu.h"
#include "LEGcpu_assembly_code.h"
#include "tinyunit.h"

bit8_t test_value = { .ui_value = 0b10100101 };

TU_TEST(tu_setup)
{
    add_code(SAVE + IMM12, 144, NOP, NOP);
    add_code(PUSH + IMM12, 51, NOP, NOP);
    execute_ticks(2);
}

TU_TEST(tu_teardown)
{
    reset_cpu();
}

TU_TEST(test_printbit8)
{
    printf("pirnt signed bit8:\n");
    printbit8(test_value, SIGNED);
    printf("\npirnt unsigned bit8:\n");
    printbit8(test_value, UNSIGNED);
    printf("\npirnt hex bit8:\n");
    printbit8(test_value, HEX);
    printf("\npirnt binary bit8:\n");
    printbit8(test_value, BINARY);
    printf("\n");
}

TU_TEST(test_print_program)
{
    print_program(BINARY);
    printf("\n");
}

TU_TEST(test_print_memory)
{
    print_memory(UNSIGNED);
    printf("\n");
}

TU_TEST(test_print_stack)
{
    print_stack(UNSIGNED);
    printf("\n");
}

int
main()
{
    TU_RUN(test_printbit8);
    TU_RUN(test_print_program);
    TU_RUN(test_print_memory);
    TU_RUN(test_print_stack);
    return TU_EXIT_CODE;
}
