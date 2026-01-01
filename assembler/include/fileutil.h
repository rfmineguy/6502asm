#ifndef FILEUTIL_H
#define FILEUTIL_H
#include <stdlib.h>

typedef enum {
  ERROR_FILE_READ_NONE   = 0,
  ERROR_FILE_READ_OPEN   = 1,
  ERROR_FILE_READ_SEEK   = 2,
  ERROR_FILE_READ_TELL   = 3,
  ERROR_FILE_READ_MALLOC = 4,
  ERROR_FILE_READ_LEN    = 5,
  ERROR_FILE_READ_CLOSE  = 6,
  ERROR_FILE_READ_READ   = 7,
  ERROR_FILE_READ_EOF    = 8,
} error_file_read;

/*
 * return
 *  NULL  failed to read file
 *  else  a malloc buffer that the caller owns
 */
const char* fu_read(const char* filename, error_file_read*);

#endif
