cmake_minimum_required(VERSION 3.21)

project(example)

if(${DEBUGGER})
    add_compile_options(-g)
endif()

include_directories(${LIB_SRC_FILE})
add_executable(example example.c)
target_link_libraries(example ${LIB_NAME})
