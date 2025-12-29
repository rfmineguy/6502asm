#ifndef FILEUTIL_H
#define FILEUTIL_H
#include <stdlib.h>

/*
 * return
 *  NULL  failed to read file
 *  else  a malloc buffer that the caller owns
 */
const char* fu_read(const char* filename);

#endif
