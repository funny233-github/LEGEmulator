#include "LEGcpu.h"
#include <stdbool.h>
#include <stdlib.h>

static uint8_t cursor = 0;
PUBLIC(monitored)
legcpu = {
    .input = {},
    .output = {},

    .reg = {},

    .counter = {},
    .program = {},

    .memory = {},

    .stack = {},
    .stack_top = {},
};

PUBLIC(void) printbit8(bit8_t value, ftype format)
{
    switch (format) {
    case SIGNED:
        printf("%d", value.si_value);
        break;
    case UNSIGNED:
        printf("%d", value.ui_value);
        break;
    case HEX:
        printf("%x", value.ui_value);
        break;
    case BINARY:
        printf("%d%d%d%d%d%d%d%d",
               value.f7,
               value.f6,
               value.f5,
               value.f4,
               value.f3,
               value.f2,
               value.f1,
               value.f0);
        break;
    }
}

static void
print_bit8_grid(bit8_t* grid, ftype format)
{
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            printbit8(*(grid + i * 16 + j), format);
            printf(",");
        }
        printf("\n");
    }
}

PUBLIC(void) print_program(ftype format)
{
    print_bit8_grid(legcpu.program, format);
}

PUBLIC(void) print_memory(ftype format)
{
    print_bit8_grid(legcpu.memory, format);
}

PUBLIC(void) print_stack(ftype format)
{
    print_bit8_grid(legcpu.stack, format);
}

PUBLIC(void) import_program_from_file(FILE* file)
{
    if (file == NULL) {
        perror("can't open file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 256; i++) {
        bit8_t value = { .ui_value = fgetc(file) };
        legcpu.program[i] = value;
    }
}

PUBLIC(void) export_program_to_file(FILE* file)
{
    if (file == NULL) {
        perror("can't open file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 256; i++) {
        bit8_t value = legcpu.program[i];
        fputc(value.ui_value, file);
    }
}

static void
ALU(bit8_t opcode, bit8_t attr1, bit8_t attr2, bit8_t* output)
{
    if (opcode.cond == 1)
        return;

    if (opcode.cond == 0 && opcode.xx == 0b01)
        return;

    switch (opcode.tail) {
    case 0: // ADD
        output->ui_value = attr1.ui_value + attr2.ui_value;
        break;
    case 1: // SUB
        output->ui_value = attr1.ui_value - attr2.ui_value;
        break;
    case 2: // AND
        output->ui_value = attr1.ui_value & attr2.ui_value;
        break;
    case 3: // OR
        output->ui_value = attr1.ui_value | attr2.ui_value;
        break;
    case 4: // NOT
        output->ui_value = ~attr1.ui_value;
        break;
    case 5: // XOR
        output->ui_value = attr1.ui_value ^ attr2.ui_value;
        break;
    case 6: // SHL
        output->ui_value = attr1.ui_value << attr2.ui_value;
        break;
    case 7: // SHR
        output->ui_value = attr1.ui_value >> attr2.ui_value;
        break;
    }
}

static void
COND(bit8_t opcode, bit8_t attr1, bit8_t attr2, bool* output)
{
    if (opcode.cond == 0 || opcode.xx == 0b01)
        return;

    switch (opcode.tail) {
    case 0: // IF_EQUAL
        *output = attr1.ui_value == attr2.ui_value;
        break;
    case 1: // IF_NOT_EQUAL
        *output = attr1.ui_value != attr2.ui_value;
        break;
    case 2: // IF_LESS
        *output = attr1.ui_value < attr2.ui_value;
        break;
    case 3: // IF_LESS_OR_EQUAL
        *output = attr1.ui_value <= attr2.ui_value;
        break;
    case 4: // IF_GREATER
        *output = attr1.ui_value > attr2.ui_value;
        break;
    case 5: // IF_GREATER_OR_EQUAL
        *output = attr1.ui_value >= attr2.ui_value;
        break;
    default:
        *output = false;
        break;
    }
}

static void
internal_write(bit8_t addr, bit8_t value, bit8_t opcode)
{
    if (opcode.cond == 1 || opcode.f4 == 1 || opcode.func == 0b0101)
        return;
    if (addr.ui_value < 6)
        legcpu.reg[addr.ui_value] = value;
    else if (addr.ui_value == 6)
        legcpu.counter = value;
    else if (addr.ui_value == 7)
        legcpu.output = value;
    else if (addr.ui_value > 7 && addr.ui_value < 16)
        legcpu.reg[addr.ui_value - 2] = value;
}

static void
internal_read(bit8_t addr, bit8_t* output)
{
    if (addr.ui_value < 6)
        *output = legcpu.reg[addr.ui_value];
    else if (addr.ui_value == 6)
        *output = legcpu.counter;
    else if (addr.ui_value == 7)
        *output = legcpu.input;
    else if (addr.ui_value > 7 && addr.ui_value < 16)
        *output = legcpu.reg[addr.ui_value - 2];
}

static void
memory_control(bit8_t opcode, bit8_t* io)
{
    if (opcode.xx == 0b10) {
        legcpu.memory[legcpu.reg[13].ui_value] = *io;
    }
    if (opcode.xx == 0b01 && opcode.tail == 0b000) {
        *io = legcpu.memory[legcpu.reg[13].ui_value];
    }

    return;
}

static void
stack_control(bit8_t opcode, bit8_t* io)
{
    if ((opcode.xx == 0b11) ||
        (opcode.func == 0b0101 && opcode.the_tail == 0)) {
        legcpu.stack[legcpu.stack_top.ui_value] = *io;
        legcpu.stack_top.ui_value++;
    }

    if ((opcode.xx == 0b01 && opcode.tail == 0b001) ||
        (opcode.func == 0b0101 && opcode.the_tail == 1)) {
        legcpu.stack_top.ui_value--;
        *io = legcpu.stack[legcpu.stack_top.ui_value];
    }

    return;
}

static void
execute()
{
    bit8_t opcode = legcpu.program[legcpu.counter.ui_value];
    bit8_t attr1 = legcpu.program[legcpu.counter.ui_value + 1];
    bit8_t attr2 = legcpu.program[legcpu.counter.ui_value + 2];
    bit8_t addr = legcpu.program[legcpu.counter.ui_value + 3];

    bit8_t attr1_value[1] = { 0 };
    bit8_t attr2_value[1] = { 0 };
    if (opcode.imm1 == 0) {
        internal_read(attr1, attr1_value);
    } else {
        *attr1_value = attr1;
    }

    if (opcode.imm2 == 0) {
        internal_read(attr2, attr2_value);
    } else {
        *attr2_value = attr2;
    }

    bit8_t ALU_ret[1] = { 0 };
    bool COND_ret[1] = { 0 };
    ALU(opcode, *attr1_value, *attr2_value, ALU_ret);
    COND(opcode, *attr1_value, *attr2_value, COND_ret);

    bool iscall = opcode.func == 0b0101 && opcode.the_tail == 0;
    bool isret = opcode.func == 0b0101 && opcode.the_tail == 1;

    if (iscall) {
        ALU_ret->ui_value = legcpu.counter.ui_value + 4;
    }

    memory_control(opcode, ALU_ret);
    stack_control(opcode, ALU_ret);
    internal_write(addr, *ALU_ret, opcode);

    if (*COND_ret || iscall) {
        *ALU_ret = addr;
    }

    if (*COND_ret || iscall || isret || addr.ui_value == 6) {
        legcpu.counter = *ALU_ret;
        return;
    }
    legcpu.counter.ui_value += 4;
}

PUBLIC(void) execute_ticks(int tick)
{
    for (int i = 0; i < tick; i++) {
        execute();
    }
}

PUBLIC(void)
add_code(uint8_t opcode, uint8_t attr1, uint8_t attr2, uint8_t addr)
{
    legcpu.program[cursor].ui_value = opcode;
    legcpu.program[cursor + 1].ui_value = attr1;
    legcpu.program[cursor + 2].ui_value = attr2;
    legcpu.program[cursor + 3].ui_value = addr;
    cursor += 4;
}

PUBLIC(void) reset_cpu()
{
    cursor = 0;
    legcpu.counter.ui_value = 0;
    legcpu.input.ui_value = 0;
    legcpu.output.ui_value = 0;
    legcpu.stack_top.ui_value = 0;

    for (int i = 0; i < 256; i++) {
        if (i < 14)
            legcpu.reg[i].ui_value = 0;

        legcpu.program[i].ui_value = 0;
        legcpu.memory[i].ui_value = 0;
        legcpu.stack[i].ui_value = 0;
    }
}
