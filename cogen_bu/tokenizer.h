#include <stdio.h>
#include "char_buf.h"

typedef enum {
  tok_int_literal,
  tok_id,
  tok_break,
  tok_continue,
  tok_else,
  tok_if,
  tok_int,
  tok_return,
  tok_while,
  tok_lparen,
  tok_rparen,
  tok_lbrace,
  tok_rbrace,
  tok_mul,
  tok_plus,
  tok_minus,
  tok_div,
  tok_rem,
  tok_bang,
  tok_lt,
  tok_gt,
  tok_le,
  tok_ge,
  tok_eq,
  tok_neq,
  tok_semicolon,
  tok_assign,
  tok_comma,
  tok_eol,
  tok_eof,
  tok_unknown
} token_kind_t;

typedef struct token {
  token_kind_t kind;
  char_buf_t ival;
  char_buf_t id;
} token;

typedef struct tokenizer {
  FILE * fp;
  char * filename;
  token tok;
  int c;
  char_buf_t old_line;
  char_buf_t line;
  int lastTokenPos;
  int curTokenPos;
  int line_index;
} * tokenizer_t;

/*token_kind_t getKeyword(char *s);
int isSymbol(char c);
token_kind_t getSymbol(char c1, char c2);
token_kind_t getSymbol(char c);
int isNonDigit(char c);*/
void syntax_error(tokenizer_t t);
tokenizer_t mk_tokenizer(char * filename);
token cur_tok(tokenizer_t t);
token next_tok(tokenizer_t t);
int getOneChar(tokenizer_t t);
