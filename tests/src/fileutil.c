#include "tests.h"
#include "fileutil_support.h"  /* provide TESTX_CONTENT */
#include "fileutil.h"
#include <stdbool.h>
#include <unistd.h>

#ifndef GIT_ROOT_BASELINE
#error "Must define GIT_ROOT_BASELINE to be the directory with a .git folder/file"
#define GIT_ROOT_BASELINE
#endif

typedef struct {
  const char* file;
  struct {
    bool ok;
    const char* buf;
    error_file_read error;
  } expected;
} fileutil_read_test_struct;

MunitResult fileutil_read_test(const MunitParameter params[], void* fixture) {
  fileutil_read_test_struct tests[] = {
    { GIT_ROOT_BASELINE "/out/testfiles/test1.txt",    { true , TEST1_CONTENT, ERROR_FILE_READ_NONE }},
    { GIT_ROOT_BASELINE "/out/testfiles/test2.txt",    { true , TEST2_CONTENT, ERROR_FILE_READ_NONE }},
    { GIT_ROOT_BASELINE "/out/testfiles/test3.txt",    { true , TEST3_CONTENT, ERROR_FILE_READ_NONE }},
    { GIT_ROOT_BASELINE "/out/testfiles/test4.txt",    { true , TEST4_CONTENT, ERROR_FILE_READ_NONE }},
    { GIT_ROOT_BASELINE "/out/testfiles/testna.txt",   { false, NULL,          ERROR_FILE_READ_OPEN }},
    { NULL },
  };

  for (int i = 0; tests[i].file; i++) {
    fileutil_read_test_struct test = tests[i];
    error_file_read error;
    munit_logf(MUNIT_LOG_INFO, "'%s'", test.file);
    const char* buf = fu_read(test.file, &error);
    munit_assert_int(test.expected.error, ==, error);
    if (test.expected.ok) {
      munit_assert_string_equal(test.expected.buf, buf);
    }
    else {
      munit_assert_ptr_null(buf);
    }
  }
  return MUNIT_OK;
}
