cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_STANDARD 99)
project(assembler LANGUAGES C)

set(LIB_SOURCES
  src/fileutil.c
  # src/codegen.c
  src/parser.c
  src/parser_directive.c
  src/parser_instruction.c
  src/parser_instruction_util.c
  src/instruction_tables.c
  src/parser_operand.c
  src/parser_label.c
  src/parser_util.c
  src/parser_errors.c
  src/ht_label.c
)

set(EXE_SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/src/cmdline.c
  src/main.c
)

set(DRIVER_SOURCES
  driver.c
)

# If you want the exe, the lib is required
if (BUILD_EXE)
  set(BUILD_LIB ON)
endif()

if (BUILD_LIB)
  add_library(lib65asm STATIC ${LIB_SOURCES})
  target_include_directories(lib65asm PRIVATE include libs)
else()
  message(STATUS "Set BUILD_LIB to build lib6502asm")
endif()

if (BUILD_EXE)
  add_executable(65asm ${EXE_SOURCES})
  target_link_libraries(65asm PRIVATE lib65asm)
  target_include_directories(65asm PRIVATE include libs)

  add_executable(driver ${DRIVER_SOURCES})
  target_link_libraries(driver PRIVATE lib65asm)
  target_include_directories(driver PRIVATE include libs)

  add_custom_command(OUTPUT
    ${CMAKE_CURRENT_LIST_DIR}/src/cmdline.c
    ${CMAKE_CURRENT_LIST_DIR}/src/cmdline.h
                     COMMAND gengetopt
                     --input=${CMAKE_CURRENT_LIST_DIR}/config.ggo
                     --header-output-dir=${CMAKE_SOURCE_DIR}/assembler/src
                     --src-output-dir=${CMAKE_SOURCE_DIR}/assembler/src
                     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
else()
  message(STATUS "Set BUILD_EXE to build 65asm and driver")
endif()
