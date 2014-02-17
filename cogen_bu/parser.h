#include "tokenizer.h"
#include "syntree.h"

program_t parse_program(tokenizer_t t);
fun_def_t parse_fun_def(tokenizer_t t);
var_decl_list_t parse_param_list(tokenizer_t t);
var_decl_t parse_param(tokenizer_t t);

stmt_t parse_stmt(tokenizer_t t);
stmt_t parse_stmt_compound(tokenizer_t t);
var_decl_t parse_var_decl(tokenizer_t t);
stmt_t parse_stmt_if(tokenizer_t t);
stmt_t parse_stmt_while(tokenizer_t t);

expr_t parse_expr(tokenizer_t t);
expr_t parse_equality_expr(tokenizer_t t);
expr_t parse_relational_expr(tokenizer_t t);
expr_t parse_additive_expr(tokenizer_t t);
expr_t parse_multiplicative_expr(tokenizer_t t);
expr_t parse_unary_expr(tokenizer_t t);
expr_list_t parse_argument_expr_list(tokenizer_t t);

op_kind_t get_equality_op(token tok);
op_kind_t get_relational_op(token tok);
op_kind_t get_additive_op(token tok);
op_kind_t get_multiplicative_op(token tok);
op_kind_t get_unary_op(token tok);

void eat_it(tokenizer_t t, token_kind_t kind);
char* eat_id(tokenizer_t t);
char* eat_int_literal(tokenizer_t t);

