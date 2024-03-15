cmake_minimum_required(VERSION 3.20.1)

project(${LIB_NAME})

aux_source_directory(${LIB_SRC_FILE} LIB_SRC)

add_library(${LIB_NAME} ${LIB_SRC})
