#include "LEGcpu.h"
#include "LEGcpu_assembly_code.h"
#include "tinyunit.h"

#define FUNC1 4 * 1
#define MAIN 4 * 3

TU_TEST(tu_setup) {}

TU_TEST(tu_teardown)
{
    reset_cpu();
}

TU_TEST(test_CALL_RET)
{
    add_code(IF_EQ + IMM12, NOP, NOP, MAIN);
    // func 1
    add_code(ADD, REG0, REG1, REG0);
    add_code(RET, NOP, NOP, NOP);
    // main
    add_code(IMM12, 12, NOP, REG0);
    add_code(IMM12, 15, NOP, REG1);
    add_code(CALL, NOP, NOP, FUNC1);
    add_code(ADD + IMM2, REG0, NOP, OUTPUT);

    execute_ticks(4);
    TU_ASSERT_INT_EQ(counter.ui_value, 4);
    execute_ticks(3);

    TU_ASSERT_INT_EQ(output.ui_value, 27);
}

int
main()
{
    TU_RUN(test_CALL_RET);
    return TU_EXIT_CODE;
}
