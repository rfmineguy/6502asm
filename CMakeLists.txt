cmake_minimum_required(VERSION 3.20)
project(assembler LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ASSEMBLER_DIR ${CMAKE_CURRENT_LIST_DIR}/assembler)
set(TESTS_DIR ${CMAKE_CURRENT_LIST_DIR}/tests)

enable_testing()
add_subdirectory(assembler)
add_subdirectory(tests)
