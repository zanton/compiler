#pragma once

typedef struct env * env_t;

typedef enum {
  id_kind_fun,
  id_kind_var,
} id_kind_t;

typedef struct id_list * id_list_t;
typedef struct id * id_t;

struct id
{
  id_kind_t kind; // function or variable
  char * name; // name
  char * addr; // memory address
  var_decl_list_t params;  // if function, need a param list
};

struct env
{
  id_list_t idl;
  env_t penv;
};

// make an empty environment object
env_t mk_env();

// make an environment object with given parent-environment-object
env_t mk_env_with_penv(env_t pe);

// add an variable (function) into env
void env_add(env_t e, char_buf_t * x); // also check identity

// search if there is variable x in env
int env_search(env_t e, char_buf_t * x);

// search x thoroughly through upper scopes
int env_search_thorough(env_t e, char_buf_t * x);

// get the address string of variable x from env
char * env_get_addr(env_t e, char_buf_t * x);

int env_set_params(var_decl_list_t params, env_t env);


/******************************************************/
/* ID List */

id_list_t mk_id_list();
void id_list_add(id_list_t sl, id_t s);
id_t id_list_get(id_list_t sl, int i);
int id_list_sz(id_list_t sl);


/***********************************************************/
/* Strorage Structure */

typedef enum {
  storage_kind_reg,
  storage_kind_mem,
} storage_kind_t;

typedef struct storage * storage_t;

struct storage {
  storage_kind_t kind; // 0:register, 1:memory
  char * base; // register name or base memory address
  int offset; // offset from base memory address
};

storage_t mk_storage(storage_kind_t kind, char * base, int offset);
storage_t mk_storage_offset(storage_t s, int offset); // add offset to current offset
storage_t mk_storage_next(storage_t s);
char * storage_get_str(storage_t s);


/***********************************************************/
/* Strorage List */

typedef struct storage_list * storage_list_t;

storage_list_t mk_storage_list();
void storage_list_add(storage_list_t sl, storage_t s);
storage_t storage_list_get(storage_list_t sl, int i);
int storage_list_sz(storage_list_t sl);
