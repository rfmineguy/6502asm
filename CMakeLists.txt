cmake_minimum_required(VERSION 3.20)
project(assembler LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ASSEMBLER_DIR ${CMAKE_CURRENT_LIST_DIR}/assembler)
set(TESTS_DIR ${CMAKE_CURRENT_LIST_DIR}/tests)

option(BUILD_LIB "Whether to build lib65asm or not" ON)
option(BUILD_EXE "Whether to build 65asm or not"    ON)
option(BUILD_TEST"Whether to build tests or not"    ON)

enable_testing()
add_subdirectory(assembler)
add_subdirectory(tests)
