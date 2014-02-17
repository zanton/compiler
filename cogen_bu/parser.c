#include "parser.h"

// parse a program
program_t parse_program(tokenizer_t t) 
{
  fun_def_list_t fun_defs = mk_fun_def_list();
  fun_def_t fun_def;
  while (cur_tok(t).kind != tok_eof) {
    fun_def = parse_fun_def(t);
    fun_def_list_add(fun_defs, fun_def);
  }
  return mk_program(t->filename, fun_defs);
}

// parse a function definition
fun_def_t parse_fun_def(tokenizer_t t) 
{
  int line = t->line_index;
  // eat int type
  eat_it(t, tok_int);
  // eat function id
  char * f = eat_id(t);
  // eat parameter list
  var_decl_list_t params = parse_param_list(t);
  // eat compound statement
  stmt_t body = parse_stmt_compound(t);

  return mk_fun_def(t->filename, line, f, params, body);
}

// parse a parameter list
var_decl_list_t parse_param_list(tokenizer_t t) 
{
  var_decl_list_t params = mk_var_decl_list();
  eat_it(t, tok_lparen);
  while (cur_tok(t).kind != tok_rparen) {
    if (cur_tok(t).kind == tok_comma)
      eat_it(t, tok_comma);
    var_decl_t para = parse_param(t);
    var_decl_list_add(params, para);
  }
  eat_it(t, tok_rparen);
  return params;
}

// parse a parameter
var_decl_t parse_param(tokenizer_t t) 
{
  int line = t->line_index;
  eat_it(t, tok_int);
  char *id = eat_id(t);

  return mk_var_decl(t->filename, line, id);
}


/*-----------------------------------------------------------------------*/

// parse statement
stmt_t parse_stmt(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  if(cur_tok(t).kind == tok_semicolon){
    eat_it(t, tok_semicolon);
    return mk_stmt_empty(filename, line);
  }else if(cur_tok(t).kind == tok_continue){
    eat_it(t, tok_continue);
    eat_it(t, tok_semicolon);
    return mk_stmt_continue(filename, line);
  }else if(cur_tok(t).kind == tok_break){
    eat_it(t, tok_break);
    eat_it(t, tok_semicolon);
    return mk_stmt_break(filename, line);    
  }else if(cur_tok(t).kind == tok_return){
    eat_it(t, tok_return);
    expr_t e = parse_expr(t);
    eat_it(t, tok_semicolon);
    return mk_stmt_return(filename, line, e);
  }else if(cur_tok(t).kind == tok_lbrace){
    return parse_stmt_compound(t);
  }else if(cur_tok(t).kind == tok_if){
    return parse_stmt_if(t);
  }else if(cur_tok(t).kind == tok_while){
    return parse_stmt_while(t);
  }else{
    expr_t e = parse_expr(t);
    eat_it(t, tok_semicolon);
    return mk_stmt_expr(filename, line, e);
  }
}

// parse compound-statement
stmt_t parse_stmt_compound(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  eat_it(t, tok_lbrace);

  var_decl_list_t var_list = mk_var_decl_list();
  stmt_list_t stmt_list = mk_stmt_list();  
  var_decl_t var_decl;
  stmt_t body;

  while(cur_tok(t).kind != tok_rbrace){
    if(cur_tok(t).kind == tok_int){
      //case var-decleration
      var_decl = parse_var_decl(t);
      var_decl_list_add(var_list, var_decl);
    }else{
      //case statement
      body = parse_stmt(t);
      stmt_list_add(stmt_list, body);
    }
  }

  eat_it(t, tok_rbrace);

  return mk_stmt_compound(filename, line, var_list, stmt_list);
}

// parse var-declaration statement
var_decl_t parse_var_decl(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  eat_it(t, tok_int);
  char *id = eat_id(t);
  eat_it(t, tok_semicolon);

  return mk_var_decl(filename, line, id);
}

// parse if-statement
stmt_t parse_stmt_if(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  eat_it(t, tok_if);
  eat_it(t, tok_lparen);
  expr_t e = parse_expr(t);
  eat_it(t, tok_rparen);
  stmt_t body_if = parse_stmt(t);
  stmt_t body_else = NULL;
  
  //else statement
  if(cur_tok(t).kind == tok_else){
    eat_it(t, tok_else);
    body_else = parse_stmt(t);
  }

  return mk_stmt_if(filename, line, e, body_if, body_else);  
}

// parse while-statement
stmt_t parse_stmt_while(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  eat_it(t, tok_while);
  eat_it(t, tok_lparen);
  expr_t e = parse_expr(t);
  eat_it(t, tok_rparen);
  stmt_t body = parse_stmt(t);
  return mk_stmt_while(filename, line, e, body);
}


/*------------------------------------------------------------------------*/


// parse expression
expr_t parse_expr(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  expr_t e1 = parse_equality_expr(t);
  expr_t e2 = NULL;

  while(cur_tok(t).kind == tok_assign){
    eat_it(t, tok_assign);
    e2 = parse_expr(t);
    e1 = mk_expr_bin_op(filename, line, op_kind_assign, e1, e2);
  }
  return e1;
}

// parse equality-expression
expr_t parse_equality_expr(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  expr_t e1 = parse_relational_expr(t);
  expr_t e2 = NULL;
  op_kind_t kind;

  while((kind = get_equality_op(cur_tok(t))) != -1){
    eat_it(t, cur_tok(t).kind);
    e2 = parse_relational_expr(t);
    e1 = mk_expr_bin_op(filename, line, kind, e1, e2);
  }
  return e1;
}

// parse relational-expression
expr_t parse_relational_expr(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  expr_t e1 = parse_additive_expr(t);
  expr_t e2 = NULL;
  op_kind_t kind;

  while((kind = get_relational_op(cur_tok(t))) != -1){
    eat_it(t, cur_tok(t).kind);
    e2 = parse_additive_expr(t);
    e1 =  mk_expr_bin_op(filename, line, kind, e1, e2);
  }
  return e1;
}

// parse additive-expression
expr_t parse_additive_expr(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  expr_t e1 = parse_multiplicative_expr(t);
  expr_t e2 = NULL;
  op_kind_t kind;

  while((kind = get_additive_op(cur_tok(t))) != -1){
    eat_it(t, cur_tok(t).kind);
    e2 = parse_multiplicative_expr(t);
    e1 = mk_expr_bin_op(filename, line, kind, e1, e2);
  }
  return e1;
}

// parse multiplicative-expression
expr_t parse_multiplicative_expr(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  expr_t e1 = parse_unary_expr(t);
  expr_t e2 = NULL;
  op_kind_t kind;

  while((kind = get_multiplicative_op(cur_tok(t))) != -1){
    eat_it(t, cur_tok(t).kind);
    e2 = parse_unary_expr(t);
    e1 = mk_expr_bin_op(filename, line, kind, e1, e2);
  }
  return e1;
}

// parse unary-expression
expr_t parse_unary_expr(tokenizer_t t)
{
  char *filename = t->filename;
  int line = t->line_index;

  if(cur_tok(t).kind == tok_int_literal){
    char *s = eat_int_literal(t);
    return mk_expr_int_literal(filename, line, s); 
  }else if(cur_tok(t).kind == tok_id){
    char *s = eat_id(t);
    if(cur_tok(t).kind == tok_lparen){
      expr_list_t args = parse_argument_expr_list(t);
      return mk_expr_call(filename, line, s, args);
    }
    return mk_expr_id(filename, line, s);
  }else if(cur_tok(t).kind == tok_lparen){
    eat_it(t, tok_lparen);
    expr_t e = parse_expr(t);
    eat_it(t, tok_rparen);
    return mk_expr_paren(filename, line, e);
  }else{
    op_kind_t kind = get_unary_op(cur_tok(t));
    if (kind == -1)
      syntax_error(t);
    eat_it(t, cur_tok(t).kind);
    expr_t e = parse_unary_expr(t);
    return mk_expr_un_op(filename, line, kind, e);
  }

}

// parse argument-expression-list
expr_list_t parse_argument_expr_list(tokenizer_t t)
{
  eat_it(t, tok_lparen);

  expr_list_t e_list = mk_expr_list();
  expr_t e = NULL;

  while (cur_tok(t).kind != tok_rparen) {
    if (cur_tok(t).kind == tok_comma)
      eat_it(t, tok_comma);
    e = parse_expr(t);
    expr_list_add(e_list, e);
  }

  eat_it(t, tok_rparen);

  return e_list;
}

// get equality op_kind_t from token object
op_kind_t get_equality_op(token tok)
{
  switch (tok.kind) {
  case tok_eq: return op_kind_eq;
  case tok_neq: return op_kind_neq;
  default: return -1; 
  }
}

// get relational op_kind_t from token object
op_kind_t get_relational_op(token tok)
{
  switch (tok.kind) {
  case tok_lt: return op_kind_lt;    
  case tok_gt: return op_kind_gt;
  case tok_ge: return op_kind_ge;
  case tok_le: return op_kind_le;
  default: return -1; 
  }
}

// get additive op_kind_t from token object
op_kind_t get_additive_op(token tok)
{
  switch (tok.kind) {
  case tok_plus: return op_kind_bin_plus;
  case tok_minus: return op_kind_bin_minus;
  default: return -1;
  }
}
 
// get multiplicative op_kind_t from token object
op_kind_t get_multiplicative_op(token tok)
{
  switch (tok.kind) {
  case tok_mul: return op_kind_mult;
  case tok_div: return op_kind_div;
  case tok_rem: return op_kind_rem;
  default: return -1;
  }
}
  
// get unary op_kind_t from token object
op_kind_t get_unary_op(token tok)
{
  switch (tok.kind) {
  case tok_plus: return op_kind_un_plus;
  case tok_minus: return op_kind_un_minus;
  case tok_bang: return op_kind_logneg;
  default: return -1;
  }
}

/*------------------------------------------------------------------*/

// eat a particular token_kind_t, no return
void eat_it(tokenizer_t t, token_kind_t kind)
{ 
  if(cur_tok(t).kind != kind){
    syntax_error(t);
  }

  next_tok(t);
}

// eat token_id, return the id
char* eat_id(tokenizer_t t)
{
  if(cur_tok(t).kind != tok_id){
    syntax_error(t);
  }

  char* s = char_buf_getstr(cur_tok(t).id);

  next_tok(t);

  return s;
}

// eat token_int_literal, return the int_literal
char* eat_int_literal(tokenizer_t t)
{
  if(cur_tok(t).kind != tok_int_literal){
    syntax_error(t);
  }

  char* s = char_buf_getstr(cur_tok(t).ival);

  next_tok(t);

  return s;
}
