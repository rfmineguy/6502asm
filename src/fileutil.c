#include "fileutil.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define handle_error(fmt, ...) {\
  fprintf(stderr, "[ERROR]  " fmt " : %s\n", ##__VA_ARGS__, strerror(errno));\
  return NULL;\
}

char* fu_read(const char* filename) {
  size_t len;
  FILE* f = fopen(filename, "r");
  if (!f)                          handle_error("Failed to open filename %s", filename);
  if (fseek(f, 0, SEEK_END) == -1) handle_error("Failed to seek to end of file");
  if ((len = ftell(f))      == -1) handle_error("Failed to ftell file");
  if (fseek(f, 0, SEEK_SET) == -1) handle_error("Failed to seek to start of file");

  char* buf = malloc(len);
  if (!buf) handle_error("Failed to malloc %zu bytes for file %s", len, filename);

  size_t read = fread(buf, 1, len, f);
  if (read != len) handle_error("Failed to read %zu bytes from file %s (read %zu bytes)", len, filename, read);
  if (fclose(f) == EOF) handle_error("Failed to close file %s", filename);

  return buf;
}
