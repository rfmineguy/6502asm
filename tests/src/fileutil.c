#include "tests.h"
#include "fileutil_support.h"  /* provide TESTX_CONTENT */
#include "fileutil.h"
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>

#ifndef TESTFILE_DIR
#error "Must define TESTFILE_DIR to be the directory with a .git folder/file"
#define TESTFILE_DIR
#endif

typedef struct {
  const char* file;
  struct {
    bool ok;
    const char* buf;
    error_file_read error;
  } expected;
} fileutil_read_test_struct;

typedef struct {
  const char* file;
  const char* s;
  size_t s_size;
} fileutil_write_test_struct;

MunitResult fileutil_read_test(const MunitParameter params[], void* fixture) {
  fileutil_read_test_struct tests[] = {
    { TESTFILE_DIR "/test1.txt",    { true , TEST1_CONTENT, ERROR_FILE_READ_NONE }},
    { TESTFILE_DIR "/test2.txt",    { true , TEST2_CONTENT, ERROR_FILE_READ_NONE }},
    { TESTFILE_DIR "/test3.txt",    { true , TEST3_CONTENT, ERROR_FILE_READ_NONE }},
    { TESTFILE_DIR "/test4.txt",    { true , TEST4_CONTENT, ERROR_FILE_READ_NONE }},
    { TESTFILE_DIR "/testna.txt",   { false, NULL,          ERROR_FILE_READ_OPEN }},
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

MunitResult fileutil_writebin_test(const MunitParameter params[], void* fixture) {
  fileutil_write_test_struct tests[] = {
    { TESTFILE_DIR "/newfile0.dat", TESTBIN0_CONTENT },
    { TESTFILE_DIR "/newfile1.dat", TESTBIN1_CONTENT },
    { NULL }
  };

  for (int i = 0; tests[i].file; i++) {
    fileutil_write_test_struct test = tests[i];
    error_file_write error;

    fu_writebin(test.file, (const uint8_t*)test.s, test.s_size, &error);
    munit_assert_int(error, ==, ERROR_FILE_WRITE_NONE);
  }
  return MUNIT_OK;
}
