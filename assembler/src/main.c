#include "cmdline.h"
#include "parser.h"
#include "fileutil.h"
#include <assert.h>

int main(int argc, char** argv) {
  // Parse arguments using gengetopt
  struct gengetopt_args_info args_info;
  if (cmdline_parser(argc, argv, &args_info) != 0) {
    return 1;
  }

  // Assert required arguments
  assert(args_info.input_given);

  char* buf = fu_read(args_info.input_arg);
  if (!buf) return 1;

  char* curr = buf;
  instruction ins;
  while ((curr = parse_line(curr, &ins))) {
  }

  printf("%s\n", buf);
}
