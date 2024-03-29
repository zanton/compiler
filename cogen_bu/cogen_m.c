#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int cogen_program(FILE *fp, program_t p, label_gen_t lg)
{
  cogen_program_header(fp, p->filename);
 
  fun_def_list_t ds = p->fun_defs;

  int n = fun_def_list_sz(ds);
  int i;

  for(i=0; i<n; i++){
    fun_def_t d = fun_def_list_get(ds, i);
    cogen_fun_def(fp, d, lg);
  }

  cogen_program_trailer(fp);

  return 0;
}

int cogen_fun_def(FILE *fp, fun_def_t d, label_gen_t lg)
{
  int frame_sz = cogen_alloc_storage_fun_def(d, );
  env_t env = mk_env();

  env = set_env_params(d->params, env);

  cogen_fun_def_header(fp, d-f);

  cogen_prologue(fp, d, );

  
  cogen_stmt(fp, d->body, lg, env);


  cogen_epilogue(fp, d, );

  cogen_fun_def_trailer(fp, d->f);

  return 0;
}

int cogen_stmt(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  switch(s->kind){
  case stmt_kind_return:
    return cogen_stmt_return(fp, s, lg, env);
  case stmt_kind_empty:
    return cogen_stmt_empty(fp, s, lg, env);
  case stmt_kind_coninue:
    return cogen_stmt_continue(fp, s, lg, env);
  case stmt_kind_break:
    return cogen_stmt_break(fp, s, lg, env);
  case stmt_kind_expr:
    return cogen_stmt_expr(fp, s, lg, env);
  case stmt_kind_compound:
    return cogen_stmt_compound(fp, s, lg, env);
  case stmt_kind_if:
    return cogen_stmt_if(fp, s, lg, env);
  case stmt_kind_while:
    return cogen_stmt_while(fp, s, lg, env);
  default:
    assert(0);
  }
}

int cogen_stmt_return(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  switch(s->e->kind){
  case expr_kind_int_literal:
    
    break;
  case expr_kind_id:
    
    break;
  case expr_kind_paren:
    
    break;
  case expr_kind_app:
    
    break;
  }
}
int cogen_stmt_empty(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  //empty
}
int cogen_stmt_continue(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  fprintf(fp, "\tjmp\t%s\n", lg_get_continue_label(lg));
}
int cogen_stmt_break(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  fprintf(fp, "\tjmp\t%s\n", lg_get_break_label(lg));
}
int cogen_stmt_expr(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  cogen_expr(fp, s->u.e, evn);
}
int cogen_stmt_compound(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  env_t env_c = mk_env_with_penv(env);
  int i;

  env_set_params(s->u.c.decls, env_c);

  for(i=0; i<stmt_list_sz(e->u.c.body); i++){
    cogen_expr(fp, stmt_list_get(e->u.c.body, i), env_c);
  }
}
int cogen_stmt_if(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  cogen_expr(fp, s->i.e, env);
  
  char* label1 = gen_label(lg);
  char* label2 = gen_label(lg);

  switch(s->i.e->kind){
  case op_kind_lt: fprintf(fp, "\tjl\t%s\n", label1); break;
  case op_kind_gt: fprintf(fp, "\tjg\t%s\n", label1); break;
  case op_kind_le: fprintf(fp, "\tjle\t%s\n", label1); break;
  case op_kind_ge: fprintf(fp, "\tjge\t%s\n", label1); break;
  case op_kind_eq : fprintf(fp, "\tje\t%s\n", label1); break;
  case op_kind_neq : fprintf(fp, "\tjne\t%s\n", label1); break;
  }
 
  cogen_expr(fp, s->i.th, env);

  fprintf(fp, "\tjmp\t%s\n", label2);

  fprintf(fp, "%s:\n", label1);

  if(s->i.el != NULL){
    cogen_expr(fp, s->i.el, env);
  }

  fpritnf(fp, "%s:\n", label2);
}
int cogen_stmt_while(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  char* label1 = gen_label(lg);
  char* label2 = lg_reset_continue_label(lg);

  fprintf(fp, "\tjmp\t%s\n", label2);

  fprintf(fp, "%s:\n", label1);
  cogen_stmt(fp, s->w.body, lg, env);
  
  fprintf(fp, "%s:\n", label2);
  cogen_expr(fp, s->w.e, lg, env);
  
  switch(s->i.e->kind){
  case op_kind_lt: fprintf(fp, "\tjl\t%s\n", label1); break;
  case op_kind_gt: fprintf(fp, "\tjg\t%s\n", label1); break;
  case op_kind_le: fprintf(fp, "\tjle\t%s\n", label1); break;
  case op_kind_ge: fprintf(fp, "\tjge\t%s\n", label1); break;
  case op_kind_eq : fprintf(fp, "\tje\t%s\n", label1); break;
  case op_kind_neq : fprintf(fp, "\tjne\t%s\n", label1); break;
  }  

  // gen label for break
  char * l = lg_reset_break_label(lg);
  if (l != NULL)
    fprintf(fp, "%s:\n", l);
}

/************************************************/

// print program header
void cogen_program_header(FILE *fp, char *filename)
{
  fprintf(fp, "\t.file\t\"%s\"\n", p->filename);
  fprintf(fp, "\t.text\n");
}

// print program trailer
void cogen_program_trailer(FILE *fp)
{
  fprintf(fp, "\t.ident\t\"GCC: (Ubuntu/Linaro 4.4.4-14ubuntu5) 4.4.5\"\n");
  fprintf(fp, "\t.section\t.note.GNU-stack,\"\",@progbits\n");
}

// print function header
void fun_def_header(FILE *fp, char *func_name)
{
  fprintf(fp, ".globl %s\n", func_name);
  fprintf(fp, "\t.type\t%s, @function\n", func_name);
  fprintf(fp, "%s:\n", func_name);
}

// print function trailer
void fun_def_trailer(FILE *fp, char *func_name)
{
  fprintf(fp, "\t.size\t%s, .-%s\n", func_name, func_name);
}

/************************************************/

label_gen_t mk_label_gen()
{
  label_gen_t l = (label_gen_t)malloc(sizeof(struct label_gen));

  l->count = 0;
  l->label = malloc(sizeof(char*));
  l->label[0] = '\0';

  return l;
}

char* gen_label(label_gen_t lg)
{
  int digit = 0;
  int temp = lg->count;
  
  //count digit
  while(temp > 0){
    digit++;
    temp /= 10;
  }
  
  free(lg->label);
  lg->label = malloc(sizeof(char*)*(digit+3));

  //change int to char*
  sprintf(lg->label, ".L%d", lg->count++);

  return lg->label;
}

char * lg_get_break_label(label_gen_t lg)
{
  if (lg->break_label == NULL)
    lg->break_label = gen_label(lg);
  return lg->break_label;
}

char * lg_reset_break_label(label_gen_t lg)
{
  char * l = lg->break_label;
  lg->break_label = NULL;
  return l;
}

char * lg_get_return_label(label_gen_t lg)
{
  if (lg->return_label == NULL)
    lg->return_label = gen_label(lg);
  return lg->return_label;
}

char * lg_reset_return_label(label_gen_t lg)
{
  char * l = lg->return_label;
  lg->return_label = NULL;
  return l;
}
char * lg_get_continue_label(label_gen_t lg)
{
  if (lg->continue_label == NULL)
    lg->continue_label = gen_label(lg);
  return lg->continue_label;
}

char * lg_reset_continue_label(label_gen_t lg)
{
  char * l = lg->continue_label;
  lg->continue_label = NULL;
  return l;
}
