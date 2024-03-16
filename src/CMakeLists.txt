cmake_minimum_required(VERSION 3.20.1)

project(${LIB_NAME})

if(${COVERAGE})
    add_compile_options(-fprofile-arcs
                        -ftest-coverage)
    link_libraries(gcov)
endif()
if(${DEBUGGER})
    add_compile_options(-g)
endif()
aux_source_directory(${LIB_SRC_FILE} LIB_SRC)

add_library(${LIB_NAME} ${LIB_SRC})
