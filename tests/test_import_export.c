#include "LEGcpu.h"
#include "LEGcpu_assembly_code.h"
#include "tinyunit.h"
#include <stdio.h>

TU_TEST(tu_setup) {}

TU_TEST(tu_teardown)
{
    reset_cpu();
}

TU_TEST(test_export)
{
    add_code(ADD + IMM12, 15, NOP, REG0);
    FILE* outfile = fopen("export.program", "w+");
    TU_ASSERT(outfile != NULL, "error while open file");
    export_program_to_file(outfile);
    fclose(outfile);
    outfile = NULL;
}

TU_TEST(test_import)
{
    FILE* readfile = fopen("export.program", "r+");
    TU_ASSERT(readfile != NULL, "error while open file");
    import_program_from_file(readfile);

    TU_ASSERT_INT_EQ(program[0].ui_value, ADD + IMM12);
    TU_ASSERT_INT_EQ(program[1].ui_value, 15);
    TU_ASSERT_INT_EQ(program[2].ui_value, NOP);
    TU_ASSERT_INT_EQ(program[3].ui_value, REG0);

    fclose(readfile);
    readfile = NULL;
}

int
main()
{
    TU_RUN(test_export);
    TU_RUN(test_import);
    return TU_EXIT_CODE;
}
