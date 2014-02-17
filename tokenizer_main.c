#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

int eat_int(tokenizer_t t);
int eat_plus(tokenizer_t t);

int main(int argc, char ** argv) {
  tokenizer_t t = mk_tokenizer(argv[1]);
  int x, y, e;

  while (cur_tok(t).kind != tok_eof) {
    // deal with empty lines
    while (cur_tok(t).kind == tok_eol) {
      printf("\n");
      next_tok(t);
    }
    // if after all empty lines, the file ends, exit
    if (cur_tok(t).kind == tok_eof)
      break;
    // begin reading expression
    x = eat_int(t);
    if (x == -1) continue;
    while (cur_tok(t).kind != tok_eol) {
      e = eat_plus(t);
      if (e == -1) break;
      y = eat_int(t);
      if (y == -1) break;
      x = x + y;
    }
    if (e == -1 || y == -1) continue;
    printf("%d\n", x);
    next_tok(t);
  }
  return 0;
}

int eat_int(tokenizer_t t){
  token tok = cur_tok(t);
  if(tok.kind != tok_int){
    syntax_error(t);
    return -1;
  }
  next_tok(t);
  return tok.ival;
}

int eat_plus(tokenizer t){
  token tok = cur_tok(t);
  if(tok.kind != tok_plus){
    syntax_error(t);
    return -1;
  }
  next_tok(t);
  return 1;
}
