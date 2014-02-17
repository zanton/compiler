#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "cogen.h"

int main(int argc, char** argv)
{

  // check filename argument
  if (argc != 2) {
    perror("Need one input file.\n");
    exit(1);
  }

  // initialize tokenizer object
  tokenizer_t t = mk_tokenizer(argv[1]);

  // parse to get program object
  program_t p = parse_program(t);

  // print out the program object
  cogen_program(stdout, p);

  return 0;
}
