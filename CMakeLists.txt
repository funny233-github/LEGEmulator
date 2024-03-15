cmake_minimum_required(VERSION 3.21)

project(project)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(LIB_NAME LEGCPU)
set(LIB_SRC_FILE ${PROJECT_SOURCE_DIR}/src)
set(LIB_FILE ${PROJECT_BINARY_DIR}/src)

add_compile_options(-g)

add_subdirectory(example)
add_subdirectory(src)
add_subdirectory(tests)
