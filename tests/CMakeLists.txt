cmake_minimum_required(VERSION 3.20)
project(tests LANGUAGES C)

set(ASSEMBLER_INCLUDE_DIRS ${ASSEMBLER_DIR}/include ${ASSEMBLER_DIR}/libs)
set(TESTFILE_DIR ${CMAKE_BINARY_DIR}/testfiles)

set(TESTFILES
  ${TESTFILE_DIR}/test1.txt
  ${TESTFILE_DIR}/test2.txt
  ${TESTFILE_DIR}/test3.txt
  ${TESTFILE_DIR}/test4.txt
)

set(TESTFILES_C ${TESTFILES})
list(TRANSFORM TESTFILES_C REPLACE "\\.txt$" ".h")
list(LENGTH TESTFILES_C _len)
math(EXPR _last "${_len} - 1")

set(SOURCES
  src/parser_directive.c
  src/parser_operand.c
  src/parser_util.c
  src/parser_instruction_ora.c
  src/parser_instruction_pha.c
  src/parser_instruction_asl.c
  src/parser_instruction_bcc.c
  src/parser_label.c
  src/fileutil.c
  src/munit.c
  src/test_main.c
)

if (NOT BUILD_TEST)
  message(STATUS "Building tests disabled")
else()
  add_executable(tests ${SOURCES})
  target_link_libraries(tests PRIVATE lib65asm)
  add_dependencies(tests generate_test_files)
  target_include_directories(tests PRIVATE ${TESTFILE_DIR})
  target_sources(tests PRIVATE ${GENERATED_HEADERS})

  target_include_directories(tests PRIVATE ${ASSEMBLER_INCLUDE_DIRS})
  target_compile_definitions(tests PRIVATE -DTESTFILE_DIR="${TESTFILE_DIR}")

  add_test(
    NAME lib65asm
    COMMAND tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )

  file(COPY genfiles.py DESTINATION ${CMAKE_BINARY_DIR}/tests)
  add_custom_command(
    OUTPUT ${TESTFILES}
    COMMAND python3 genfiles.py --outdir=${TESTFILE_DIR} -v
    COMMENT "Generating test files"
  )

  foreach(i RANGE ${_last})
    list(GET TESTFILES ${i} infile)
    list(GET TESTFILES_C ${i} outfile)
    get_filename_component(basename "${infile}" NAME_WE)

    add_custom_command(
      OUTPUT "${outfile}"
      COMMAND xxd -i -n "${basename}_txt" "${infile}" > "${outfile}"
      DEPENDS "${infile}"
      COMMENT "Generating ${outfile} from ${infile}"
      VERBATIM
    )

    list(APPEND GENERATED_HEADERS "${outfile}")
  endforeach()

  add_custom_target(generate_test_files ALL
    DEPENDS ${TESTFILES_C}
  )
endif()
