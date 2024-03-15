#include "LEGcpu.h"
#include "LEGcpu_assembly_code.h"
#include "tinyunit.h"

int test_values[8] = { 11, 23, 44, 56, 11, 22, 13, 66 };

TU_TEST(tu_setup) {}

TU_TEST(tu_teardown)
{
    reset_cpu();
}

TU_TEST(test_SAVE_LOAD)
{
    for (int i = 0; i < 8; i++) {
        add_code(IMM12, i, NOP, MEMADDR);
        add_code(SAVE + IMM12, test_values[i], NOP, NOP);
        execute_ticks(2);
        TU_ASSERT_INT_EQ(memory[i].ui_value, test_values[i]);
    }
    for (int i = 8; i < 16; i++) {
        add_code(IMM12, i - 8, NOP, MEMADDR);
        add_code(LOAD, NOP, NOP, REG0);
        execute_ticks(2);
        TU_ASSERT_INT_EQ(reg[0].ui_value, test_values[i - 8]);
    }
}

TU_TEST(test_PUSH_POP)
{
    for (int i = 0; i < 8; i++) {
        add_code(PUSH + IMM12, test_values[i], NOP, NOP);
        execute_ticks(1);
        TU_ASSERT_INT_EQ(stack_top.ui_value, i + 1);
        TU_ASSERT_INT_EQ(stack[i].ui_value, test_values[i]);
    }

    for (int i = 8; i < 16; i++) {
        add_code(POP, NOP, NOP, REG0);
        execute_ticks(1);
        TU_ASSERT_INT_EQ(stack_top.ui_value, 7 - (i - 8));
        TU_ASSERT_INT_EQ(reg[0].ui_value, test_values[7 - (i - 8)]);
    }
}

int
main()
{
    TU_RUN(test_SAVE_LOAD);
    TU_RUN(test_PUSH_POP);
    return TU_EXIT_CODE;
}
