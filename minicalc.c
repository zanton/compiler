#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define NUM_LENGTH 9

typedef enum {
  tok_int,
  tok_plus,
  tok_eol,
  tok_eof,
  tok_unknown
} token_kind_t;

typedef struct token {
  token_kind_t kind;
  int ival;
} token;

typedef struct tokenizer {
  FILE * fp;
  token tok;
  int c;
  char old_line[50];
  char line[50];
  int nLine;
  int lastTokenPos;
  int curTokenPos;
  int line_index;
} * tokenizer_t;

void syntax_error(tokenizer_t t);
tokenizer_t mk_tokenizer(char * filename);
token cur_tok(tokenizer_t t);
token next_tok(tokenizer_t t);
int eat_int(tokenizer_t t);
int eat_plus(tokenizer_t t);
int getOneChar(tokenizer_t t);


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




void syntax_error(tokenizer_t t) {
  // get the error position
  int err_pos;
  if (cur_tok(t).kind==tok_eol || cur_tok(t).kind==tok_eof)
    err_pos = t->lastTokenPos;
  else
    err_pos = t->curTokenPos;

  // read all the current line
  while (cur_tok(t).kind != tok_eol && cur_tok(t).kind != tok_eof)
    next_tok(t);

  // display error message
  printf("synctax error: %d:%d\n%s\n", t->line_index-1, err_pos, t->old_line);
  int i;
  for (i=0; i<err_pos-1; i++)
    printf(" ");
  printf("^\n");
  
  // reinitialize tokenizer_t to next line
  next_tok(t);
}

tokenizer_t mk_tokenizer(char * filename) {
  tokenizer_t t = (tokenizer_t) malloc( sizeof(struct tokenizer) );
  FILE * fp = fopen(filename, "rb");
  if (fp == NULL)
    return NULL;
  
  // initialize tokenizer_t
  t->fp = fp;
  t->nLine = 0;
  t->line[0] = 0;
  t->lastTokenPos = 0;
  t->curTokenPos = 0;
  t->line_index = 1;

  // read first tok
  t->c = getOneChar(t);
  next_tok(t);

  return t;
}

token cur_tok(tokenizer_t t) {
  return t->tok;
}

token next_tok(tokenizer_t t) {
  // exit if the cur_tok is tok_eof already
  if (cur_tok(t).kind == tok_eof)
    return cur_tok(t);

  token new_tok;
  int new_pos = t->curTokenPos;

  
  // pass all space character
  int c = t->c;
  while (c == ' ')
    c = getOneChar(t);

  // get new position for curTokenPos
  new_pos = t->nLine;
  
  // evaluate the character c
  if (isdigit(c)) {
    // new int
    char num[10];
    int nNum = 0;
    while (isdigit(c)) {
      num[nNum++] = c;
      c = getOneChar(t);
    }
    num[nNum] = 0;
    new_tok.ival = atoi(num);
    new_tok.kind = tok_int;
  } else if (c == '+') {
    // new plus sign
    new_tok.kind = tok_plus;
    c = getOneChar(t);
  } else if (c == '\n') {
    // new newline character
    new_tok.kind = tok_eol;
    //c = getOneChar(t); // wait after changing line
  } else if (c == EOF) {
    // EOF character
    new_tok.kind = tok_eof;
    c = getOneChar(t);
  } else {
    // unknown character
    new_tok.kind = tok_unknown;
    while (c!=' ' && c!='\n' && c!=EOF) c = getOneChar(t);
  }
  
  t->tok = new_tok;
  t->c = c;
  t->lastTokenPos = t->curTokenPos;
  t->curTokenPos = new_pos;

  // change line
  if (cur_tok(t).kind == tok_eol) {
    t->line[t->nLine] = 0;
    strcpy(t->old_line, t->line);
    t->nLine = 0;
    t->line[0] = 0;
    t->line_index++;
    t->c = getOneChar(t);
  }

  return new_tok;
}

int eat_int(tokenizer_t t) {
  token tok = cur_tok(t);
  if (tok.kind != tok_int) {
    syntax_error(t);
    return -1;
  }
  next_tok(t);
  return tok.ival;
}

int eat_plus(tokenizer_t t) {
  token tok = cur_tok(t);
  if (tok.kind != tok_plus) {
    syntax_error(t);
    return -1;
  }
  next_tok(t);
  return 1;
}

int getOneChar(tokenizer_t t) {
  FILE * fp = t->fp;
  if (feof(fp))
    return 0;
  int c = fgetc(fp); 
  if (c != '\n' && c != EOF) {
    t->line[t->nLine] = c;
    t->nLine++;
  }
  return c;
}
