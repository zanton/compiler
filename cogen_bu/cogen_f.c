#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// code-generator for program_t
int cogen_program(FILE *fp, program_t p, label_gen_t lg)
{
  cogen_program_header(fp, p->filename);
 
  fun_def_list_t ds = p->fun_defs;

  int n = fun_def_list_sz(ds);
  int i;

  for(i=p; i<n; i++){
    fun_def_t d = fun_def_list_get(ds, i);
    cogen_fun_def(fp, d, lg);
  }

  cogen_program_trailer(fp);

  return 1;
}

// code-generator for fun_def_t
int cogen_fun_def(FILE *fp, fun_def_t d, label_gen_t lg)
{
  
  int frame_sz = cogen_alloc_storage_fun_def(d);

  env_t env = mk_env();
  env_set_params(d->params, env);

  cogen_fun_def_header(fp, d-f);

  cogen_prologue(fp, d, frame_sz);

  
  cogen_stmt(fp, d->body, lg, env);

  // gen label for return 
  char * l = lg_reset_return_label(lg);
  if (l != NULL)
    fprintf(fp, "%s:\n", l);
  
  cogen_epilogue(fp, d);

  cogen_fun_def_trailer(fp, d->f);

  return 1;
}

// code-generator for stmt_t
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
  expr_t e = s->u.e;
  storage_t eax = mk_storage(storage_kind_reg, "%eax", 0);
  cogen_expr(e);
  if (storage_is_different(e->info->sto, eax))
    cogen_move(e->info->sto, eax);
  fprintf(fp, "\tjmp\t%s\n", lg_get_return_label(lg));
  return 1;
}
int cogen_stmt_empty(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  //empty
  return 1;
}
int cogen_stmt_continue(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  
}
int cogen_stmt_break(FILE *fp, stmt_t s, label_gen_t lg, env_t env)
{
  
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
  case op_kind_lt: fprintf(fp, "\tlt\t%s\n", label1); break;
  case op_kind_gt: fprintf(fp, "\tgt\t%s\n", label1); break;
  case op_kind_le: fprintf(fp, "\tle\t%s\n", label1); break;
  case op_kind_ge: fprintf(fp, "\tge\t%s\n", label1); break;
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
  char* label2 = gen_label(lg);

  fprintf(fp, "\tjmp\t%s\n", label2);

  fprintf(fp, "%s:\n", label1);
  cogen_stmt(fp, s->w.body, lg, env);
  
  fprintf(fp, "%s:\n", label2);
  cogen_expr(fp, s->w.e, lg, env);
  
  switch(s->i.e->kind){
  case op_kind_lt: fprintf(fp, "\tlt\t%s\n", label1); break;
  case op_kind_gt: fprintf(fp, "\tgt\t%s\n", label1); break;
  case op_kind_le: fprintf(fp, "\tle\t%s\n", label1); break;
  case op_kind_ge: fprintf(fp, "\tge\t%s\n", label1); break;
  }  
}


/************************************************/
/* Code-generator for expression */

int cogen_expr(FILE * fp, expr_t e, environment_t env)
{
  if (e->kind == expr_kind_int_literal) {

    return 1;

  } else if (e->kind == expr_kind_id) {

    return 1;

  } else if (e->kind == expr_kind_paren) {

    expr_t pe = e->u.p;;
    switch (pe->kind) {
    case expr_kind_int_literal: 
      fprintf(fp, "\tmovl\t$%s, %s\n", pe->u.s, storage_get_str(e->info->sto));
      break;
    case expr_kind_id:
      // get var address
      storage_t addr = env_get_addr(pe->u.s);
      if (addr == NULL) 
	logic_error(pe->filename, pe->line, "Variable not declared yet.");
      // get a reg
      storage_t reg = storage_list_get(e->info->rgl, 0);
      // move
      cogen_move(fp, addr, e->info->sto, reg);
      break;
    case expr_kind_paren, expr_kind_app:
      cogen_expr(fp, pe, env);
      // get a reg
      storage_t reg = storage_list_get(e->info->rgl, 0);
      // move
      cogen_move(fp, pe->info->sto, e->info_sto, );
      break;
    }

  } else if (e->kind == expr_kind_app) {

    if (e->u.a.o == op_kind_fun) {

      expr_t x;
      int i;
      for (i=0; i<expr_list_sz(e->u.a.args); i++) {
	x = expr_list_get(e->u.a.args, i);
	cogen_expr(fp, x, env);
      }
      // call function
      storage_t reg = storage_list_get(e->info->rgl, 0);
      for (i=expr_list_sz(e->u.a.args)-1; i>=0; i--) {
	x = expr_list_get(e->u.a.args, i);
	fprintf(fp, "\tmovl\t%s, %s\n", storage_get_str(x->info->sto), reg->base);
	fprintf(fp, "\tpushl\t%s\n", reg->base);
      }
      fprintf(fp, "\tcall\t%s\n", e->u.a.f);
      cogen_move(fp, mk_storage(storage_kind_reg, "%eax", 0), e->info->sto, reg);

    } else {

      switch (e->u.a.o) {
      case op_kind_assign: 

	expr_t x1 = expr_list_get(e->u.a.args, 0);
	expr_t x2 = expr_list_get(e->u.a.args, 1);
	storage_t reg = storage_list_get(e->info->rgl, 0);
	// check error
	if (x1->kind != expr_kind_id)
	  logic_error(x1->filename, x1->line, "Variable ID expected.");
	// get address of x1
	storage_t addr = env_get_addr(env, x1->u.s);
	// cogen x2
	if (x2->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tmovl $%s, %s\n", x2->u.s, storage_get_str(addr));
	} else if (x2->kind == expr_kind_id) {
	  storage_t addr2 = env_get_addr(env, x2->u.s);
	  cogen_move(fp, addr2, addr, reg);
	} else {
	  x2->info->sto = addr;
	  cogen_expr(fp, x2, env);
	  //cogen_move(fp, x2->info->sto, addr, reg);
	}
	break;

      case op_kind_eq:
 
	expr_t x1 = expr_list_get(e->u.a.args, 0);
	expr_t x2 = expr_list_get(e->u.a.args, 1);
	storage_t reg;
	if (e->info->sto->kind == storage_kind_reg)
	  reg = e->info->sto;
	else 
	  reg = storage_list_get(e->info->rgl, 0);
	cogen_expr(fp, x1, env);
	cogen_expr(fp, x2, env);

	// move x1 to reg
	if (x1->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tmovl\t$%s, %s\n", x1->u.s, storage_get_str(reg));
	} else if (x1->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  cogen_move(fp, addr, reg, NULL);
	} else { 
	  cogen_move(fp, x1->info->sto, reg, NULL);
	}
	// compare
	if (x2->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tcmpl\t$%s, %s\n", x2->u.s, storage_get_str(reg));
	} else if (x2->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  fprintf(fp, "\tcmpl\t%s, %s\n", storage_get_str(addr), storage_get_str(reg));
	} else { 
	  fprintf(fp, "\tcmpl\t%s, %s\n", storage_get_str(x2->info->sto), storage_get_str(reg));
	}
	// compare tail
	if (e->info->sto != NULL) {
	  fprintf(fp, "\tsete\t%%al\n");
	  fprintf(fp, "\tmovzbl\t%%al, %s\n", storage_get_str(reg));
	  cogen_move(fp, reg, e->info->sto, NULL);
	}
	break;

      case op_kind_ge:
	expr_t x1 = expr_list_get(e->u.a.args, 0);
	expr_t x2 = expr_list_get(e->u.a.args, 1);
	storage_t reg;
	if (e->info->sto!=NULL && e->info->sto->kind==storage_kind_reg)
	  reg = e->info->sto;
	else 
	  reg = storage_list_get(e->info->rgl, 0);
	cogen_expr(fp, x1, env);
	cogen_expr(fp, x2, env);

	// move x1 to reg
	if (x1->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tmovl\t$%s, %s\n", x1->u.s, storage_get_str(reg));
	} else if (x1->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  cogen_move(fp, addr, reg, NULL);
	} else { 
	  cogen_move(fp, x1->info->sto, reg, NULL);
	}
	// compare
	if (x2->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tcmpl\t$%s, %s\n", x2->u.s, storage_get_str(reg));
	} else if (x2->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  fprintf(fp, "\tcmpl\t%s, %s\n", storage_get_str(addr), storage_get_str(reg));
	} else { 
	  fprintf(fp, "\tcmpl\t%s, %s\n", storage_get_str(x2->info->sto), storage_get_str(reg));
	}
	// compare tail
	if (e->info->sto != NULL) {
	  fprintf(fp, "\tmovzbl\t%%al, %s\n", storage_get_str(reg));
	  cogen_move(fp, reg, e->info->sto, NULL);
	}
	break;

      case op_kind_bin_plus: 
	expr_t x1 = expr_list_get(e->u.a.args, 0);
	expr_t x2 = expr_list_get(e->u.a.args, 1);
	storage_t reg;
	if (e->info->sto->kind == storage_kind_reg)
	  reg = e->info->sto;
	else 
	  reg = storage_list_get(e->info->rgl, 0);
	cogen_expr(fp, x1, env);
	cogen_expr(fp, x2, env);

	// exchange if x1 is number, to avoid loading constant to reg
	if (x1->kind == expr_kind_int_literal) {
	  expr_t temp = x1;
	  x1 = x2;
	  x2 = temp;
	}

	// move x1 to reg
	if (x1->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tmovl\t$%s, %s\n", x1->u.s, storage_get_str(reg));
	} else if (x1->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  cogen_move(fp, addr, reg, NULL);
	} else { 
	  cogen_move(fp, x1->info->sto, reg, NULL);
	}
	// plus
	if (x2->kind == expr_kind_int_literal) {
	  fprintf(fp, "\taddl\t$%s, %s\n", x2->u.s, storage_get_str(reg));
	} else if (x2->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  fprintf(fp, "\taddl\t%s, %s\n", storage_get_str(addr), storage_get_str(reg));
	} else { 
	  fprintf(fp, "\taddl\t%s, %s\n", storage_get_str(x2->info->sto), storage_get_str(reg));
	}
	// plus tail
	if (e->info->sto != NULL) {
	  cogen_move(fp, reg, e->info->sto, NULL);
	}
	break;
	
      case op_kind_bin_minus: 
	expr_t x1 = expr_list_get(e->u.a.args, 0);
	expr_t x2 = expr_list_get(e->u.a.args, 1);
	storage_t reg;
	if (e->info->sto->kind == storage_kind_reg)
	  reg = e->info->sto;
	else 
	  reg = storage_list_get(e->info->rgl, 0);
	cogen_expr(fp, x1, env);
	cogen_expr(fp, x2, env);

	// move x1 to reg
	if (x1->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tmovl\t$%s, %s\n", x1->u.s, storage_get_str(reg));
	} else if (x1->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  cogen_move(fp, addr, reg, NULL);
	} else { 
	  cogen_move(fp, x1->info->sto, reg, NULL);
	}
	// minus
	if (x2->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tsubl\t$%s, %s\n", x2->u.s, storage_get_str(reg));
	} else if (x2->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  fprintf(fp, "\tsubl\t%s, %s\n", storage_get_str(addr), storage_get_str(reg));
	} else { 
	  fprintf(fp, "\tsubl\t%s, %s\n", storage_get_str(x2->info->sto), storage_get_str(reg));
	}
	// minus tail
	if (e->info->sto != NULL) {
	  cogen_move(fp, reg, e->info->sto, NULL);
	}
	break;

      case op_kind_mult: 
	expr_t x1 = expr_list_get(e->u.a.args, 0);
	expr_t x2 = expr_list_get(e->u.a.args, 1);
	storage_t reg;
	if (e->info->sto->kind == storage_kind_reg)
	  reg = e->info->sto;
	else 
	  reg = storage_list_get(e->info->rgl, 0);
	cogen_expr(fp, x1, env);
	cogen_expr(fp, x2, env);
	
	// exchange if x1 is number, to avoid loading constant to reg
	if (x1->kind == expr_kind_int_literal) {
	  expr_t temp = x1;
	  x1 = x2;
	  x2 = temp;
	}

	// move x1 to reg
	if (x1->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tmovl\t$%s, %s\n", x1->u.s, storage_get_str(reg));
	} else if (x1->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  cogen_move(fp, addr, reg, NULL);
	} else { 
	  cogen_move(fp, x1->info->sto, reg, NULL);
	}
	// multiply
	if (x2->kind == expr_kind_int_literal) {
	  fprintf(fp, "\timull\t$%s, %s\n", x2->u.s, storage_get_str(reg));
	} else if (x2->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  fprintf(fp, "\timull\t%s, %s\n", storage_get_str(addr), storage_get_str(reg));
	} else { 
	  fprintf(fp, "\timull\t%s, %s\n", storage_get_str(x2->info->sto), storage_get_str(reg));
	}
	// multiply tail
	if (e->info->sto != NULL) {
	  cogen_move(fp, reg, e->info->sto, NULL);
	}
	break;

      case op_kind_div: 
	expr_t x1 = expr_list_get(e->u.a.args, 0);
	expr_t x2 = expr_list_get(e->u.a.args, 1);
	// get reg %eax and %edx
	storage_t eax, edx;
	int i;
	for (i=0; i<storage_list_sz(e->info->rgl); i++) {
	  storage_t s = storage_list_get(e->info->rgl, i);
	  if (strcmp(s->base, "%eax") == 0)
	    eax = s;
	  if (strcmp(s->base, "%edx") == 0)
	    edx = s;
	}
	// check if regs are available
	if (eax==NULL || edx==NULL)
	  logic_error(e->filename, e->line, "Register eax or edx unavailable.");
	
	cogen_expr(fp, x1, env);
	cogen_expr(fp, x2, env);

	// move x1 to eax
	if (x1->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tmovl\t$%s, %s\n", x1->u.s, storage_get_str(eax));
	} else if (x1->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  cogen_move(fp, addr, eax, NULL);
	} else { 
	  cogen_move(fp, x1->info->sto, eax, NULL);
	}
	// set edx
	fprintf(fp, "\tmovl\t%%eax, %%edx\n");
	fprintf(fp, "\tsarl\t$31, %%edx");

	// divide
	if (x2->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tidivl\t$%s\n", x2->u.s);
	} else if (x2->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  fprintf(fp, "\tidivl\t%s\n", storage_get_str(addr));
	} else { 
	  fprintf(fp, "\tidivl\t%s\n", storage_get_str(x2->info->sto));
	}
	// minus tail
	if (e->info->sto != NULL) {
	  cogen_move(fp, eax, e->info->sto, NULL);
	}
	break;

      case op_kind_rem: 
	expr_t x1 = expr_list_get(e->u.a.args, 0);
	expr_t x2 = expr_list_get(e->u.a.args, 1);
	// get reg %eax and %edx
	storage_t eax, edx;
	int i;
	for (i=0; i<storage_list_sz(e->info->rgl); i++) {
	  storage_t s = storage_list_get(e->info->rgl, i);
	  if (strcmp(s->base, "%eax") == 0)
	    eax = s;
	  if (strcmp(s->base, "%edx") == 0)
	    edx = s;
	}
	// check if regs are available
	if (eax==NULL || edx==NULL)
	  logic_error(e->filename, e->line, "Register eax or edx unavailable.");
	
	cogen_expr(fp, x1, env);
	cogen_expr(fp, x2, env);

	// move x1 to eax
	if (x1->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tmovl\t$%s, %s\n", x1->u.s, storage_get_str(eax));
	} else if (x1->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  cogen_move(fp, addr, eax, NULL);
	} else { 
	  cogen_move(fp, x1->info->sto, eax, NULL);
	}
	// set edx
	fprintf(fp, "\tmovl\t%%eax, %%edx\n");
	fprintf(fp, "\tsarl\t$31, %%edx");

	// divide
	if (x2->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tidivl\t$%s\n", x2->u.s);
	} else if (x2->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  fprintf(fp, "\tidivl\t%s\n", storage_get_str(addr));
	} else { 
	  fprintf(fp, "\tidivl\t%s\n", storage_get_str(x2->info->sto));
	}
	// minus tail
	if (e->info->sto != NULL) {
	  cogen_move(fp, edx, e->info->sto, NULL);
	}
	break;

      case op_kind_un_plus: 
	expr_t x = expr_list_get(e->u.a.args, 0);
	if (e->info->sto != NULL) 
	  x->info->sto = e->info->sto;
	cogen_expr(fp, x, env);
	break;

      case op_kind_un_minus: 
	expr_t x = expr_list_get(e->u.a.args, 0);
	storage_t reg;
	if (e->info->sto->kind == storage_kind_reg)
	  reg = e->info->sto;
	else 
	  reg = storage_list_get(e->info->rgl, 0);
	
	cogen_expr(fp, x, env);

	// move x to reg
	if (x->kind == expr_kind_int_literal) {
	  fprintf(fp, "\tmovl\t$%s, %s\n", x1->u.s, storage_get_str(reg));
	} else if (x1->kind == expr_kind_id) {
	  storage_t addr = env_get_addr(env, x1->u.s);
	  cogen_move(fp, addr, reg, NULL);
	} else { 
	  cogen_move(fp, x1->info->sto, reg, NULL);
	}
	// minus
	fprintf(fp, "\tnegl\t%s\n", storage_get_str(reg));
	
	// minus tail
	if (e->info->sto != NULL) {
	  cogen_move(fp, reg, e->info->sto, NULL);
	}
	break;

      case op_kind_logneg: 
	expr_t x = expr_list_get(e->u.a.args, 0);
	storage_t reg;
	if (e->info->sto->kind == storage_kind_reg)
	  reg = e->info->sto;
	else 
	  reg = storage_list_get(e->info->rgl, 0);
	
	x->info->sto = reg;
	cogen_expr(fp, x, env);

	// logneg
	fprintf(fp, "\tcmpl\t$0, %s\n", storage_get_str(reg));
	
	// logneg tail
	if (e->info->sto != NULL) {
	  fprintf(fp, "\tsete\t%%al\n");
	  fprintf(fp, "\tmovzbl\t%%al, %s\n", storage_get_str(reg));
	  cogen_move(fp, reg, e->info->sto, NULL);
	}
	break;

      } // switch
    } // else
  } // else if

  return 1;
}

void cogen_move(FILE * fp, storage_t s1, storage_t s2, storage_t reg)
{
  if (s1==NULL || s2==NULL)
    return ;
  if (!storage_is_different(s1, s2))
    return ;
  if (s1->kind==storage_kind_mem && s2->kind==storage_kind_mem) {
    fprintf(fp, "\tmovl\t%s, %s\n", storage_get_str(s1), storage_get_str(reg));
    fprintf(fp, "\tmovl\t%s, %s\n", storage_get_str(reg), storage_get_str(s2));
  } else {
    fprintf(fp, "\tmovl\t%s, %s\n", storage_get_str(s1), storage_get_str(s2));
  }
}

/************************************************/
/* Allocate memory for variables and expression */

int cogen_alloc_storage_fun_def(fun_def_t d)
{
  int flag = 0;
  storage_t anchor = mk_storage(storage_kind_mem, "%%ebp", flag);

  // assign memory for parameters
  var_decl_list_t params = d->params;
  var_decl_t v;
  int i;
  for (i=0; i<var_decl_list_sz(params); i++) {
    v = var_decl_list_get(params, i);
    v->info->sto = mk_storage_offset(anchor, 8 + 4*i);
  }

  // adjust anchor to allocate space for backing up any neccessary registers

  // deal with stmt_t
  d->body->info->anc = anchor;
  d->body->info->sto = NULL;
  // make list of available regs
  storage_list_t reg_list = mk_storage_list();
  storage_list_add(reg_list, mk_storage(storage_kind_reg, "%%eax", 0));
  storage_list_add(reg_list, mk_storage(storage_kind_reg, "%%ecx", 0));
  storage_list_add(reg_list, mk_storage(storage_kind_reg, "%%edx", 0));
  d->body->info->rgl = reg_list;

  int frame_sz = cogen_alloc_storage_stmt(d->body);

  return 4 * frame_sz;
}

int cogen_alloc_storage_stmt(stmt_t stmt)
{
  int offset;
  switch (stmt->kind) {
  case stmt_kind_empty: 
    offset = 0; 
    break;
  case stmt_kind_continue: 
    offset = 0;
    break;
  case stmt_kind_break: 
    offset = 0;
    break;

  case stmt_kind_return: 
    expr_t e = stmt->u.e;
    e->info->anc = stmt->info->anc;
    e->info->sto = mk_storage(storage_kind_reg, "%%eax", 0);
    e->info->rgl = stmt->info->rgl;
    offset = cogen_alloc_storage_expr(e); 
    break; 

  case stmt_kind_expr: 
    expr_t e = stmt->u.e;
    e->info->anc = stmt->info->anc;
    e->info->sto = NULL;
    e->info->rgl = stmt->info->rgl;
    offset = cogen_alloc_storage_expr(e); 
    break;

  case stmt_kind_compound: 
    storage_t anchor = stmt->info->anc;
    var_decl_list_t vars = stmt->u.c.decls;
    stmt_list_t compound = stmt->u.c.body;
    int max = 0;
    offset = 0;
    int i;
    // assign storage for all variables
    for (i=0; i<var_decl_list_sz(vars); i++) {
      var_decl_t var = var_decl_list_get(vars, i);
      anchor = mk_storage_next(anchor);
      offset++;
      var->info->anc = NULL;
      var->info->sto = anchor;
    }
    // assign same anchor, and NULL sto, and same reg_list  for all statements
    for (i=0; i<stmt_list_sz(compound); i++) {
      stmt_t stmt1 = stmt_list_get(compound, i);
      stmt1->info->anc = anchor;
      stmt1->info->sto = NULL;
      stmt1->info->rgl = stmt->info->rgl;
    }
    // continue recursive cogen
    for (i=0; i<stmt_list_sz(compound); i++) {
      stmt_t stmt = stmt_list_get(compound, i);
      int n = cogen_alloc_storage_stmt(stmt);
      if (n > max) max = n;
    }
    offset += max;
    break;
		
  case stmt_kind_if: 
    storage_t anchor = stmt->info->anc;
    // assign anchor, storage, reg_list
    //expression
    stmt->u.i.e->info->anc = anchor;
    stmt->u.i.e->info->sto = NULL;
    stmt->u.i.e->info->rgl = stmt->info->rgl;
    //then statement
    stmt->u.i.th->info->anc = anchor;
    stmt->u.i.th->info->sto = NULL;
    stmt->u.i.th->info->rgl = stmt->info->rgl;
    //else statement
    if (stmt->u.i.el != NULL) {
      stmt->u.i.el->info->anc = anchor;
      stmt->u.i.el->info->sto = NULL;
      stmt->u.i.el->info->rgl = stmt->info->rgl;
    }
    // continue recursive cogen
    int max = 0;
    int n;
    n = cogen_alloc_storage_expr(stmt->u.i.e);
    if (n > max) max = n;
    n = cogen_alloc_storage_stmt(stmt->u.i.th);
    if (n > max) max = n;
    if (stmt->u.i.el != NULL) {
      n = cogen_alloc_storage_stmt(stmt->u.i.el);
      if (n > max) max = n;
    }

    offset = max;
    break;	

  case stmt_kind_while: 
    storage_t anchor = stmt->info->anc;
    expr_t e = stmt->u.w.e;
    stmt_t body = stmt->u.w.body;

    // assign anchor, storage, reg_list
    e->info->anc = anchor;
    e->info->sto = NULL;
    e->info->rgl = stmt->info->rgl;
    body->info->anc = anchor;
    body->info->sto = NULL;
    body->info->rgl = stmt->info->rgl;

    // continue recursive cogen
    int max = 0;
    int n;
    n = cogen_alloc_storage_expr(e);
    if (n > max) max = n;
    n = cogen_alloc_storage_stmt(body);
    if (n > max) max = n;
    
    offset = max;
    break;
  }

  return offset;
}

int cogen_alloc_storage_expr(expr_t e)
{
  int offset = 0;

  switch (e->kind) {

  case expr_kind_int_literal: 
    break;

  case expr_kind_id: 
    break;

  case expr_kind_paren: 
    e->u.p->info = e->info;
    offset = cogen_alloc_storage_expr(e->u.p);
    break;

  case expr_kind_app:
    offset = 0;
    expr_list_t args = e->u.a.args;
    storage_t anchor = e->info->anc;
    int i;
    // loop for each sub-expression
    for (i=0; i<expr_list_sz(args); i++) {
      // get sub-expr
      expr_t e1 = expr_list_get(args, i);
      if (e1->kind != expr_int_literal && e1->kind != expr_id) {
	// set anc, sto, and rgl for sub-expr
	anchor = mk_storage_next(anchor);
	e1->info->anc = anchor;
	e1->info->sto = anchor;
	e1->info->rgl = e->info->rgl;
	// call recursively
	int n = cogen_alloc_storage_expr(e1);
	// move anchor for next sub-expr
	anchor = mk_storage_offset(anchor, -4*n);
	offset += n;
      }
    }
    break;
  }
  return offset;
}


/***********************************************/
/* Neccesary functions */

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
/* Prologue and epilogue of a function */

// print function prologue
void cogen_prologue(FILE *fp, fun_def_t d, int frame_sz)
{
  fprintf(fp, "\tpushl\t%%ebp\n");
  fprintf(fp, "\tmovl\t%%esp, %%ebp\n");
  if (frame_sz > 0)
    fprintf(fp, "\tsubl\t$%d, %%esp\n", frame_sz);

  // back up registers
  storage_list_t reg_list = d->body->info->rgl;
  int i;
  for (i=0; i<storage_list_sz(reg_list); i++) {
    storage_t reg = storage_list_get(rgl, i);
    if (strcmp(reg->base, "%ebx") == 0)
      fprintf(fp, "\tmovl\t%%ebx, -4(%%ebp)\n");
    else if (strcmp(reg->base, "%esi") == 0)
      fprintf(fp, "\tmovl\t%%esi, -8(%%ebp)\n");
    else if (strcmp(reg->base, "%edi") == 0)
      fprintf(fp, "\tmovl\t%%edi, -12(%%ebp)\n");
  }
}

// print function epilogue
void cogen_epilogue(FILE *fp, fun_def_t d, int make_label, char * label)
{
  if (make_label)
    fprintf(fp, "%s:\n", label);

  // recover registers
  storage_list_t reg_list = d->body->info->rgl;
  int i;
  for (i=0; i<storage_list_sz(reg_list); i++) {
    storage_t reg = storage_list_get(rgl, i);
    if (strcmp(reg->base, "%ebx") == 0)
      fprintf(fp, "\tmovl\t-4(%%ebp), %%ebx\n");
    else if (strcmp(reg->base, "%esi") == 0)
      fprintf(fp, "\tmovl\t-8(%%ebp), %%esi\n");
    else if (strcmp(reg->base, "%edi") == 0)
      fprintf(fp, "\tmovl\t-12(%%ebp), %%edi\n");
  }

  fprintf(fp, "\tleave\n");
  fprintf(fp, "\tret\n");
}

/************************************************/
/* Label generator functions */

label_gen_t mk_label_gen()
{
  label_gen_t l = (label_gen_t)safe_malloc(sizeof(struct label_gen));

  l->count = 0;
  
  return l;
}

char* label_gen_generate(label_gen_t lg)
{
  int digit = 0;
  int temp = lg->count
  
  //count digit
  while(temp > 0){
    digit++;
    temp /= 10;
  }

  char label[digit+3];

  //change int to char*
  sprintf(label, ".L%d", lg->count);

  return label;
}
