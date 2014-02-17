#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenizer.h"


//check type of keywords
token_kind_t getKeyword(char *s) // -1 ならkeywordじゃない
{
  if(strcmp(s, "break") == 0){
    return tok_break;
  }else if(strcmp(s, "continue") == 0){
    return tok_continue;
  }else if(strcmp(s, "else") == 0){
    return tok_else;
  }else if(strcmp(s, "if") == 0){
    return tok_if;
  }else if(strcmp(s, "int") == 0){
    return tok_int;
  }else if(strcmp(s, "return") == 0){
    return tok_return;
  }else if(strcmp(s, "while") == 0){
    return tok_while;
  }else{
    return -1;
  }
}

//check symbol or not
int isSymbol(char c)
{
  if(c == '(' || c == ')' || c == '{' || c == '}' || c == '*' || c == '+' || c == '-' || c == '/' || c == '%' || c == '!' || c == '<' || c == '>' || c == '=' || c == ';' || c == ','){
    return 1;
  }else{
    return 0;
  }
}

//check type of symbols
token_kind_t getSymbol2(char c1, char c2) 
{
  if(c1 == '<' && c2 == '='){
    return tok_le;
  }else if(c1 == '>' && c2 == '='){
    return tok_ge;
  }else if(c1 == '!' && c2 == '='){
    return tok_neq;
  }else if(c1 == '=' && c2 == '='){
    return tok_eq;
  }else{
    return -1;
  }
}

token_kind_t getSymbol1(char c1){

  if(c1 == '('){
    return tok_lparen;
  }else if(c1 == ')'){
    return tok_rparen;
  }else if(c1 == '{'){
    return tok_lbrace;
  }else if(c1 == '}'){
    return tok_rbrace;
  }else if(c1 == '*'){
    return tok_mul;
  }else if(c1 == '+'){
    return tok_plus;
  }else if(c1 == '-'){
    return tok_minus;
  }else if(c1 == '/'){
    return tok_div;
  }else if(c1 == '%'){
    return tok_rem;
  }else if(c1 == '!'){
    return tok_bang;
  }else if(c1 == '<'){
    return tok_lt;
  }else if(c1 == '>'){
    return tok_gt;
  }else if(c1 == '='){
    return tok_assign;
  }else if(c1 == ';'){
    return tok_semicolon;
  }else if(c1 == ','){
    return tok_comma;
  }else{
    return -1;
  }
}

int isNonDigit(char c)
{
  if(('A'<= c && c <= 'z') || c == '_'){
    return 1;
  }else{
    return 0;
  }
}


void syntax_error(tokenizer_t t) {
  // get the error position
  int err_pos;
  err_pos = t->curTokenPos;

  // display error message
  printf("syntax error: %d:%d\ntoken kind = %d\n", t->line_index, err_pos, cur_tok(t).kind);
  
  // exit
  exit(1);
}

tokenizer_t mk_tokenizer(char * filename) {
  tokenizer_t t = (tokenizer_t) malloc( sizeof(struct tokenizer) );
  FILE * fp = fopen(filename, "rb");
  if (fp == NULL)
    return NULL;
  
  // initialize tokenizer_t
  t->fp = fp;
  t->filename = filename;
  t->old_line = mk_char_buf();
  t->line = mk_char_buf();
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
  while (c == ' ' || c == '\t' || c == '\n')
    c = getOneChar(t);

  // get new position for curTokenPos
  new_pos = char_buf_getlen(t->line) - 1;
  
  // evaluate the character c
  /*if (c == '\n') {   // new line char
    new_tok.kind = tok_eol;
    //c = getOneChar(t);
  } else*/ if (c == EOF) {   // end of file char
    new_tok.kind = tok_eof;
  } else if (isSymbol(c)) {  // symbol char
    char c1 = getOneChar(t);
    token_kind_t kind = getSymbol2(c,c1);
    if (kind != -1) {
      new_tok.kind = kind;
      c = getOneChar(t);
    } else {
      new_tok.kind = getSymbol1(c);
      c = c1;
    }
  } else if (isdigit(c)) {  // digit char
    // get the int string
    new_tok.ival = mk_char_buf();
    while (isdigit(c)) {
      char_buf_addchar(new_tok.ival,c);
      c = getOneChar(t);
    }
    new_tok.kind = tok_int_literal;
  } else if (isNonDigit(c)) { // non digit char
    new_tok.id = mk_char_buf();
    while (isNonDigit(c) || isdigit(c)) {
      char_buf_addchar(new_tok.id, c);
      c = getOneChar(t);
    } 
    // set new token
    new_tok.kind = getKeyword(char_buf_getstr(new_tok.id));
    if (new_tok.kind == -1) 
      new_tok.kind = tok_id;
  } else {  // strange character
    new_tok.kind = tok_unknown;
    c = getOneChar(t);
  }
  
  // renew t
  t->tok = new_tok;
  t->c = c;
  t->lastTokenPos = t->curTokenPos;
  t->curTokenPos = new_pos;

  // change line
  /*if (cur_tok(t).kind == tok_eol) {
    cpy_char_buf_t(t->old_line, t->line);
    clear_char_buf(t->line);
    t->line_index++;
    t->c = getOneChar(t);
  }*/

  return new_tok;
}

int getOneChar(tokenizer_t t) {
  // access file
  FILE * fp = t->fp;
  // check if EOF
  if (feof(fp))
    return 0;
  // get one char
  int c = fgetc(fp); 

  // insert c into line only if it's not EOL and EOF
  if (c != '\n' && c != EOF) {
    char_buf_addchar(t->line,c);
  }

  // increase line_index
  if (c == '\n')
    t->line_index++;

  return c;
}
