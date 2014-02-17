#include <stdio.h>

#include "environment.h"
#include "util.h"
#include "list.h"
#include "syntree.h"

// make an empty environment object
env_t mk_env()
{
  env_t env = (env_t) safe_malloc( sizeof(struct env) );
  env->idl = mk_id_list();
  env->penv = NULL;
}

// make an environment object with given parent-environment-object
env_t mk_env_with_penv(env_t pe);

// add an variable (function) into env
int env_add(env_t env, id_kind_t kind, char * name, char * addr, var_decl_list_t params)
{
  // check identity
  id_list_t idl = env->idl;
  int i;
  for (i=0; i<id_list_sz(idl); i++) {
    id_t id = id_list_get(idl, i);
    if (strcmp(id->name, name) == 0)
      return -1;
  }
  // add
  id_t id = (id_t) safe_malloc( sizeof(struct id) );
  id->kind = kind;
  id->name = name;
  id->addr = addr;
  id->params = params;
  id_list_add(env->idl, id);
  return 1;
}

// search if there is variable x in env
int env_search(env_t e, char_buf_t * x);

// search x thoroughly through upper scopes
int env_search_thorough(env_t e, char_buf_t * x);

// get the address string of variable x from env
char * env_get_addr(env_t e, char_buf_t * x);

int env_set_params(var_decl_list_t params, env_t env)
{
  var_decl_t v;
  int i;
  for (i=0; i<var_decl_list_sz(params); i++) {
    v = var_decl_list_get(params, i);
    int error = env_add(env, id_kind_var, v->v, v->info->sto, NULL);
    if (error == -1)
      logic_error(v->filename, v->line, "Duplicate variable declaration.");
  }
}


/******************************************************/
/* ID List */

id_list_t mk_id_list()
{
  return (id_list_t) mk_list();
}

void id_list_add(id_list_t sl, id_t s)
{
  list_add((list_t) sl, (void *) s);
}

id_t id_list_get(id_list_t sl, int i)
{
  return (id_t) list_get((list_t) sl, i);
}

int id_list_sz(id_list_t sl)
{
  return list_sz((list_t) sl);
}


/******************************************************/
/* Storage Structure */

storage_t mk_storage(storage_kind_t kind, char * base, int offset)
{
  storage_t s = (storage_t) safe_malloc( sizeof(struct storage) );
  s->kind = kind;
  s->base = base;
  s->offset = offset;
  return s;
}

storage_t mk_storage_offset(storage_t s, int offset)
{
  storage_t new_s = (storage_t) safe_malloc( sizeof(struct storage) );
  new_s->kind = s->kind;
  new_s->base = safe_strdup(s->base);
  new_s->offset = s->offset + offset; // add offset to current offset
  return new_s;
}

storage_t mk_storage_next(storage_t s)
{
  storage_t new_s = (storage_t) safe_malloc( sizeof(struct storage) );
  new_s->kind = s->kind;
  new_s->base = safe_strdup(s->base);
  new_s->offset = s->offset - 4;
  return new_s;
}

char * storage_get_str(storage_t s)
{
  if (s->kind == storage_kind_reg)
    return s->base;
  else {
    char * str;
    sprintf(str, "%d(%s)", s->offset, s->base);
    return str;
  }
}


/******************************************************/
/* Storage List */

storage_list_t mk_storage_list()
{
  return (storage_list_t) mk_list();
}

void storage_list_add(storage_list_t sl, storage_t s)
{
  list_add((list_t) sl, (void *) s);
}

storage_t storage_list_get(storage_list_t sl, int i)
{
  return (storage_t) list_get((list_t) sl, i);
}

int storage_list_sz(storage_list_t sl)
{
  return list_sz((list_t) sl);
}


/******************************************************/
/* Error */

void logic_error(char * filename, int line, char * content)
{
  printf("Error at line %d in file %s\n%s\n", line, filename, content);
  exit(1);
}
