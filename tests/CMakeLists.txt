cmake_minimum_required(VERSION 3.20.1)

project(test)

set(Unittests
    test_ALU
    test_COND
    test_MEM_STACK
    test_internal
    test_FUNC
    test_print
    test_import_export)

include_directories(${LIB_SRC_FILE} tinyunit)
if(${COVERAGE})
    add_compile_options(-fprofile-arcs
                        -ftest-coverage)
    link_libraries(gcov)
endif()
if(${DEBUGGER})
    add_compile_options(-g)
endif()
foreach(Unittest ${Unittests})
    add_executable(${Unittest} ${Unittest}.c)
    target_link_libraries(${Unittest} ${LIB_NAME})
    add_test(NAME ${Unittest} COMMAND ./${Unittest})
endforeach()
