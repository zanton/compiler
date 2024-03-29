#include <stdio.h>
#include <string.h>

#include "environment.h"
#include "util.h"
#include "list.h"

/*******************************************************/
/* Environment Struct */

// make an empty environment object
env_t mk_env()
{
  env_t env = (env_t) safe_malloc( sizeof(struct env) );
  env->varl = mk_var_list();
  env->penv = NULL;
  return env;
}

// make an environment object with given parent-environment-object
env_t mk_env_with_penv(env_t pe)
{
  env_t env = (env_t) safe_malloc( sizeof(struct env) );
  env->varl = mk_var_list();
  env->penv = pe;
  return env;
}

// add an variable (function) into env
int env_add(env_t env, char * name, storage_t addr)
{
  // check identity
  var_list_t varl = env->varl;
  int i;
  for (i=0; i<var_list_sz(varl); i++) {
    var_t var = var_list_get(varl, i);
    if (strcmp(var->name, name) == 0)
      return -1;
  }
  // add
  var_t var = (var_t) safe_malloc( sizeof(struct var) );
  var->name = name;
  var->addr = addr;
  var_list_add(env->varl, var);
  return 1;
}

// get the address string of variable x from env
storage_t env_get_addr(env_t env, char * x)
{
  int i;
  while (env != NULL) {
    for (i=0; i<var_list_sz(env->varl); i++) {
      var_t var = var_list_get(env->varl, i);
      if (strcmp(var->name, x) == 0)
	return var->addr;
    }
    env = env->penv;
  }
  return NULL;
}

int env_set_params(var_decl_list_t params, env_t env)
{
  var_decl_t v;
  int i;
  for (i=0; i<var_decl_list_sz(params); i++) {
    v = var_decl_list_get(params, i);
    int error = env_add(env, v->v, v->info->sto);
    if (error == -1)
      logic_error(v->filename, v->line, "Duplicate variable declaration.");
  }
  return 1;
}

