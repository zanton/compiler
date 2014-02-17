#include <stdio.h>

#include "environment.h"

// code-generator for program_t
int cogen_program(FILE *fp, program_t p);

// code-generator for fun_def_t
int cogen_fun_def(FILE *fp, fun_def_t d, label_gen_t lg);

// code-generator for stmt
int cogen_stmt(FILE *fp, stmt_t s, label_gen_t lg, env_t env);
int cogen_stmt_return(FILE *fp, stmt_t s, label_gen_t lg, env_t env);
int cogen_stmt_empty(FILE *fp, stmt_t s, label_gen_t lg, env_t env);
int cogen_stmt_continue(FILE *fp, stmt_t s, label_gen_t lg, env_t env);
int cogen_stmt_break(FILE *fp, stmt_t s, label_gen_t lg, env_t env);
int cogen_stmt_expr(FILE *fp, stmt_t s, label_gen_t lg, env_t env);
int cogen_stmt_compound(FILE *fp, stmt_t s, label_gen_t lg, env_t env);
int cogen_stmt_if(FILE *fp, stmt_t s, label_gen_t lg, env_t env);
int cogen_stmt_while(FILE *fp, stmt_t s, label_gen_t lg, env_t env);

// code-generator for expression
int cogen_expr(FILE * fp, expr_t e, environment_t env);
void cogen_move(FILE * fp, storage_t s1, storage_t s2, storage_t reg);

// Allocate memory for variables and expression 
int cogen_alloc_storage_fun_def(fun_def_t d);
int cogen_alloc_storage_stmt(stmt_t stmt);
int cogen_alloc_storage_expr(expr_t e);

// Neccesary functions
void cogen_program_header(FILE *fp, char *filename);
void cogen_program_trailer(FILE *fp);
void fun_def_header(FILE *fp, char *func_name);
void fun_def_trailer(FILE *fp, char *func_name);

// Prologue and epilogue of a function
void cogen_prologue(FILE *fp, fun_def_t d, int frame_sz);
void cogen_epilogue(FILE *fp, fun_def_t d, label_gen_t lg);
