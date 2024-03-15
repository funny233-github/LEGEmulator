#include "LEGcpu.h"
#include "LEGcpu_assembly_code.h"
#include "tinyunit.h"

TU_TEST(tu_setup) {}

TU_TEST(tu_teardown)
{
    reset_cpu();
}

TU_TEST(test_ADD)
{
    add_code(ADD + IMM12, 10, 2, REG0);

    execute_ticks(1);

    TU_ASSERT_INT_EQ(reg[0].ui_value, 12);
}

TU_TEST(test_SUB)
{
    add_code(SUB + IMM12, 10, 2, REG0);

    execute_ticks(1);

    TU_ASSERT_INT_EQ(reg[0].ui_value, 8);
}

TU_TEST(test_AND)
{
    add_code(AND + IMM12, 0b00001100, 0b00001010, REG0);

    execute_ticks(1);

    TU_ASSERT_INT_EQ(reg[0].ui_value, 0b00001000);
}

TU_TEST(test_OR)
{
    add_code(OR + IMM12, 0b00001100, 0b00001010, REG0);

    execute_ticks(1);

    TU_ASSERT_INT_EQ(reg[0].ui_value, 0b00001110);
}

TU_TEST(test_NOT)
{
    add_code(NOT + IMM12, 0b10101101, NOP, REG0);

    execute_ticks(1);

    TU_ASSERT_INT_EQ(reg[0].ui_value, 0b01010010);
}

TU_TEST(test_XOR)
{
    add_code(XOR + IMM12, 0b00001100, 0b00001010, REG0);

    execute_ticks(1);

    TU_ASSERT_INT_EQ(reg[0].ui_value, 0b00000110);
}

TU_TEST(test_SHL)
{
    add_code(SHL + IMM12, 0b01001100, 3, REG0);

    execute_ticks(1);

    TU_ASSERT_INT_EQ(reg[0].ui_value, 0b01100000);
}

TU_TEST(test_SHR)
{
    add_code(SHR + IMM12, 0b01001100, 3, REG0);

    execute_ticks(1);

    TU_ASSERT_INT_EQ(reg[0].ui_value, 0b00001001);
}

int
main()
{
    TU_RUN(test_ADD);
    TU_RUN(test_SUB);
    TU_RUN(test_AND);
    TU_RUN(test_OR);
    TU_RUN(test_NOT);
    TU_RUN(test_XOR);
    TU_RUN(test_SHL);
    TU_RUN(test_SHR);
    return TU_EXIT_CODE;
}
