#include "LEGcpu.h"
#include "LEGcpu_assembly_code.h"
#include "tinyunit.h"

bit8_t* internals[16] = {
    &legcpu.reg[0],  &legcpu.reg[1],  &legcpu.reg[2],  &legcpu.reg[3],
    &legcpu.reg[4],  &legcpu.reg[5],  &legcpu.counter, &legcpu.output,
    &legcpu.reg[6],  &legcpu.reg[7],  &legcpu.reg[8],  &legcpu.reg[9],
    &legcpu.reg[10], &legcpu.reg[11], &legcpu.reg[12], &legcpu.reg[13],
};

#define TEST_INTERNAL(internal)                                                \
    TU_TEST(test_##internal)                                                   \
    {                                                                          \
        add_code(ADD + IMM12, 12, NOP, internal);                              \
        execute_ticks(1);                                                      \
        TU_ASSERT_INT_EQ((*internals[internal]).ui_value, 12);                 \
        reset_cpu();                                                           \
        add_code(ADD + IMM2, internal, 5, internal);                           \
        execute_ticks(1);                                                      \
        if (internal != OUTPUT)                                                \
            TU_ASSERT_INT_EQ((*internals[internal]).ui_value, 5);              \
    }

TU_TEST(tu_setup) {}

TU_TEST(tu_teardown)
{
    reset_cpu();
}

TEST_INTERNAL(REG0);
TEST_INTERNAL(REG1);
TEST_INTERNAL(REG2);
TEST_INTERNAL(REG3);
TEST_INTERNAL(REG4);
TEST_INTERNAL(REG5);
TEST_INTERNAL(COUNTER);
TEST_INTERNAL(OUTPUT);
TEST_INTERNAL(REG6);
TEST_INTERNAL(REG7);
TEST_INTERNAL(REG8);
TEST_INTERNAL(REG9);
TEST_INTERNAL(REG10);
TEST_INTERNAL(REG11);
TEST_INTERNAL(REG12);
TEST_INTERNAL(MEMADDR);

int
main()
{
    TU_RUN(test_REG0);
    TU_RUN(test_REG1);
    TU_RUN(test_REG2);
    TU_RUN(test_REG3);
    TU_RUN(test_REG4);
    TU_RUN(test_REG5);
    TU_RUN(test_COUNTER);
    TU_RUN(test_OUTPUT);
    TU_RUN(test_REG6);
    TU_RUN(test_REG7);
    TU_RUN(test_REG8);
    TU_RUN(test_REG9);
    TU_RUN(test_REG10);
    TU_RUN(test_REG11);
    TU_RUN(test_REG12);
    TU_RUN(test_MEMADDR);
    return TU_EXIT_CODE;
}
