#pragma once

#include <stdio.h>

#include "list.h"
#include "util.h"

#define MAX_BASE_LENGTH 11

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
int storage_is_different(storage_t s1, storage_t s2);

/******************************************************/
/* VAR List */

typedef struct var_list * var_list_t;
typedef struct var * var_t;

struct var
{
  char * name; // name
  storage_t addr; // memory address
};

var_list_t mk_var_list();
void var_list_add(var_list_t sl, var_t s);
var_t var_list_get(var_list_t sl, int i);
int var_list_sz(var_list_t sl);


/***********************************************************/
/* Strorage List */

typedef struct storage_list * storage_list_t;

storage_list_t mk_storage_list();
void storage_list_add(storage_list_t sl, storage_t s);
storage_t storage_list_get(storage_list_t sl, int i);
int storage_list_sz(storage_list_t sl);


/***********************************************************/
/* Label Generator */

typedef struct label_gen * label_gen_t;

struct label_gen
{
  int count;
  char * label;
  char * return_label;
  char * break_label;
  char * continue_label;
};

label_gen_t mk_label_gen();
char * gen_label(label_gen_t lg);

char * lg_get_break_label(label_gen_t lg);
char * lg_get_return_label(label_gen_t lg);
char * lg_get_continue_label(label_gen_t lg);

char * lg_reset_break_label(label_gen_t lg);
char * lg_reset_return_label(label_gen_t lg);
char * lg_reset_continue_label(label_gen_t lg);


/****************************************************************/
/* Error */

void logic_error(char * filename, int line, char * content);

