#include "LEGcpu.h"
#include <stdbool.h>
#include <stdlib.h>

static uint8_t cursor = 0;
bit8_t counter = { .ui_value = 0 };
bit8_t input = { .ui_value = 0 };
bit8_t output = { .ui_value = 0 };
bit8_t reg[14];
bit8_t program[256];
bit8_t memory[256];
bit8_t stack[256];
bit8_t stack_top;

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
    print_bit8_grid(program, format);
}

PUBLIC(void) print_memory(ftype format)
{
    print_bit8_grid(memory, format);
}

PUBLIC(void) print_stack(ftype format)
{
    print_bit8_grid(stack, format);
}

PUBLIC(void) import_program_from_file(FILE* file)
{
    if (file == NULL) {
        perror("can't open file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 256; i++) {
        bit8_t value = { .ui_value = fgetc(file) };
        program[i] = value;
    }
}

PUBLIC(void) export_program_to_file(FILE* file)
{
    if (file == NULL) {
        perror("can't open file");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 256; i++) {
        bit8_t value = program[i];
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
    switch (addr.ui_value) {
    case 0:
        reg[0] = value;
        break;
    case 1:
        reg[1] = value;
        break;
    case 2:
        reg[2] = value;
        break;
    case 3:
        reg[3] = value;
        break;
    case 4:
        reg[4] = value;
        break;
    case 5:
        reg[5] = value;
        break;
    case 6:
        counter = value;
        break;
    case 7:
        output = value;
        break;
    case 8:
        reg[6] = value;
        break;
    case 9:
        reg[7] = value;
        break;
    case 10:
        reg[8] = value;
        break;
    case 11:
        reg[9] = value;
        break;
    case 12:
        reg[10] = value;
        break;
    case 13:
        reg[11] = value;
        break;
    case 14:
        reg[12] = value;
        break;
    case 15:
        reg[13] = value;
        break;
    default:
        break;
    }
}

static void
internal_read(bit8_t addr, bit8_t* output)
{
    switch (addr.ui_value) {
    case 0:
        *output = reg[0];
        break;
    case 1:
        *output = reg[1];
        break;
    case 2:
        *output = reg[2];
        break;
    case 3:
        *output = reg[3];
        break;
    case 4:
        *output = reg[4];
        break;
    case 5:
        *output = reg[5];
        break;
    case 6:
        *output = counter;
        break;
    case 7:
        *output = input;
        break;
    case 8:
        *output = reg[6];
        break;
    case 9:
        *output = reg[7];
        break;
    case 10:
        *output = reg[8];
        break;
    case 11:
        *output = reg[9];
        break;
    case 12:
        *output = reg[10];
        break;
    case 13:
        *output = reg[11];
        break;
    case 14:
        *output = reg[12];
        break;
    case 15:
        *output = reg[13];
        break;
    default:
        output->ui_value = 0;
        break;
    }
}

static void
memory_control(bit8_t opcode, bit8_t* io)
{
    if (opcode.xx == 0b10) {
        memory[reg[13].ui_value] = *io;
    }
    if (opcode.xx == 0b01 && opcode.tail == 0b000) {
        *io = memory[reg[13].ui_value];
    }

    return;
}

static void
stack_control(bit8_t opcode, bit8_t* io)
{
    if ((opcode.xx == 0b11) ||
        (opcode.func == 0b0101 && opcode.the_tail == 0)) {
        stack[stack_top.ui_value] = *io;
        stack_top.ui_value++;
    }

    if ((opcode.xx == 0b01 && opcode.tail == 0b001) ||
        (opcode.func == 0b0101 && opcode.the_tail == 1)) {
        stack_top.ui_value--;
        *io = stack[stack_top.ui_value];
    }

    return;
}

static void
execute()
{
    bit8_t opcode = program[counter.ui_value];
    bit8_t attr1 = program[counter.ui_value + 1];
    bit8_t attr2 = program[counter.ui_value + 2];
    bit8_t addr = program[counter.ui_value + 3];

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
        ALU_ret->ui_value = counter.ui_value + 4;
    }

    memory_control(opcode, ALU_ret);
    stack_control(opcode, ALU_ret);
    internal_write(addr, *ALU_ret, opcode);

    if (*COND_ret || iscall) {
        *ALU_ret = addr;
    }

    if (*COND_ret || iscall || isret || addr.ui_value == 6) {
        counter = *ALU_ret;
        return;
    }
    counter.ui_value += 4;
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
    program[cursor].ui_value = opcode;
    program[cursor + 1].ui_value = attr1;
    program[cursor + 2].ui_value = attr2;
    program[cursor + 3].ui_value = addr;
    cursor += 4;
}

PUBLIC(void) reset_cpu()
{
    cursor = 0;
    counter.ui_value = 0;
    input.ui_value = 0;
    output.ui_value = 0;
    stack_top.ui_value = 0;

    for (int i = 0; i < 256; i++) {
        if (i < 14)
            reg[i].ui_value = 0;

        program[i].ui_value = 0;
        memory[i].ui_value = 0;
        stack[i].ui_value = 0;
    }
}
