#include "fileutil.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define handle_error_fmt(errcode, fmt, ...) {\
  fprintf(stderr, "[ERROR]  %s : " fmt "\n", strerror(errno), __VA_ARGS__);\
  *error = errcode;\
  return NULL;\
}

#define handle_error(errcode, msg) {\
  fprintf(stderr, "[ERROR]  %s : " msg "\n", strerror(errno));\
  *error = errcode;\
  return NULL;\
}

const char* fu_read(const char* filename, error_file_read* error) {
  assert(error && "error must not be null");
  size_t len;
  FILE* f = fopen(filename, "r");
  if (!f)                          handle_error_fmt(ERROR_FILE_READ_OPEN, "Failed to open filename %s", filename);
  if (fseek(f, 0, SEEK_END) == -1) handle_error(ERROR_FILE_READ_SEEK, "Failed to seek to end of file");
  if ((len = ftell(f))      == -1) handle_error(ERROR_FILE_READ_TELL, "Failed to ftell file");
  if (fseek(f, 0, SEEK_SET) == -1) handle_error(ERROR_FILE_READ_SEEK, "Failed to seek to start of file");

  char* buf = malloc(len);
  if (!buf) handle_error_fmt(ERROR_FILE_READ_MALLOC, "Failed to malloc %zu bytes for file %s", len, filename);

  size_t read = fread(buf, 1, len, f);
  if (read != len) {
    if (ferror(f)) {
      handle_error_fmt(ERROR_FILE_READ_READ, "I/O error while reading file %s", filename);
    }
    else if (feof(f) == 0) {
      handle_error_fmt(ERROR_FILE_READ_EOF, "Unexpected eof error in file %s (read %zu/%zu bytes)", filename, read, len);
    }
    handle_error_fmt(ERROR_FILE_READ_LEN, "Failed to read %zu bytes from file %s (read %zu/%zu bytes)", len, filename, read, len);
  }
  if (fclose(f) == EOF) handle_error_fmt(ERROR_FILE_READ_CLOSE, "Failed to close file %s", filename);
  buf[read] = 0;

  *error = ERROR_FILE_READ_NONE;
  return buf;
}
