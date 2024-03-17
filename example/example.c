#include "LEGcpu.h"
#include "LEGcpu_assembly_code.h"
#include <stdbool.h>

#define FIBB 1 * 4
#define ELSE 4 * 4
#define MAIN 17 * 4

const int tmp = REG2;
const int x = REG1;
const int res = REG0;

void setup()
{
    add_code(IF_EQ+IMM12, NOP, NOP, MAIN);

    // fibb(x)
    // if x <= 2:
    //      res = 1
    //      return res
    // res = fibb(x - 1)
    // res += fibb(x - 2)
    // return res
    // FIBB
    add_code(IF_G+IMM2,x, 2, ELSE);
    add_code(IMM12, 1, NOP, res);
    add_code(RET, NOP, NOP, NOP);
    // ELSE
    add_code(PUSH+IMM2, x, NOP, NOP);
    add_code(SUB+IMM2, x, 1, x);
    add_code(CALL, NOP, NOP, FIBB);
    add_code(POP, NOP, NOP, x);

    add_code(PUSH+IMM2, x, NOP, NOP);
    add_code(PUSH+IMM2, res, NOP, NOP);
    add_code(SUB+IMM2, x, 2, x);
    add_code(CALL, NOP, NOP, FIBB);
    add_code(ADD+IMM2, res, NOP, tmp);
    add_code(POP, NOP, NOP, res);
    add_code(POP, NOP, NOP, x);
    add_code(ADD, tmp, res, res);

    add_code(RET, NOP, NOP, NOP);
    // main
    // fibb(13) expected result 233
    // MAIN
    add_code(ADD+IMM12, 13, NOP, x);
    add_code(CALL, NOP, NOP, FIBB);
    add_code(ADD+IMM2, res, NOP, OUTPUT);
}

int main()
{
    setup();
    static int cnt = 0;
    while (legcpu.output.ui_value == 0) {
        execute_ticks(1);
        cnt++;
        printf("%d tick, counter is %d\n", cnt, legcpu.counter.ui_value);
    }
    printbit8(legcpu.output, UNSIGNED);
}
