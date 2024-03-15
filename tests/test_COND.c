#include "LEGcpu.h"
#include "LEGcpu_assembly_code.h"
#include "tinyunit.h"

TU_TEST(tu_setup)
{
    add_code(ADD + IMM12, 10, 2, REG0);

    execute_ticks(1);
}

TU_TEST(tu_teardown)
{
    reset_cpu();
}

TU_TEST(test_IF_EQ)
{
    add_code(IF_EQ + IMM12, 44, 111, 0);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 8);

    add_code(IF_EQ + IMM12, 55, 55, 4);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 4);
}

TU_TEST(test_IF_N_EQ)
{
    add_code(IF_N_EQ + IMM12, 111, 111, 0);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 8);

    add_code(IF_N_EQ + IMM12, 55, 59, 4);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 4);
}

TU_TEST(test_IF_L)
{
    add_code(IF_L + IMM12, 22, 11, 0);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 8);

    add_code(IF_L + IMM12, 55, 55, 4);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 12);

    add_code(IF_L + IMM12, 55, 56, 4);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 4);
}

TU_TEST(test_IF_L_EQ)
{
    add_code(IF_L_EQ + IMM12, 22, 11, 0);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 8);

    add_code(IF_L_EQ + IMM12, 55, 55, 16);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 16);

    add_code(ADD + IMM12, 55, 56, REG0);
    add_code(IF_L_EQ + IMM12, 55, 100, 4);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 4);
}

TU_TEST(test_IF_G)
{
    add_code(IF_G + IMM12, 11, 22, 0);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 8);

    add_code(IF_G + IMM12, 55, 55, 4);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 12);

    add_code(IF_G + IMM12, 56, 55, 4);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 4);
}

TU_TEST(test_IF_G_EQ)
{
    add_code(IF_G_EQ + IMM12, 11, 22, 0);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 8);

    add_code(IF_G_EQ + IMM12, 100, 55, 16);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 16);

    add_code(ADD + IMM12, 1, 56, REG0);

    add_code(IF_G_EQ + IMM12, 100, 10, 4);

    execute_ticks(1);
    TU_ASSERT_INT_EQ(counter.ui_value, 4);
}

int
main()
{
    TU_RUN(test_IF_EQ);
    TU_RUN(test_IF_N_EQ);
    TU_RUN(test_IF_L);
    TU_RUN(test_IF_L_EQ);
    TU_RUN(test_IF_G);
    TU_RUN(test_IF_G_EQ);
    return TU_EXIT_CODE;
}
