#include <stdio.h>
#include <string.h>

#include "util2.h"


/******************************************************/
/* var List */


void var_list_add(var_list_t sl, var_t s)
{
  list_add((list_t) sl, (void *) s);
}

var_t var_list_get(var_list_t sl, int i)
{
  return (var_t) list_get((list_t) sl, i);
}

int var_list_sz(var_list_t sl)
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
    str = (char *) safe_malloc( sizeof(char) * MAX_BASE_LENGTH);
    sprintf(str, "%d(%s)", s->offset, s->base);
    return str;
  }
}

int storage_is_different(storage_t s1, storage_t s2)
{
  if (s1->kind != s2->kind) return 1;
  if (strcmp(s1->base, s2->base) != 0) return 1;
  if (s1->kind == storage_kind_mem) {
    if (s1->offset != s2->offset) return 1;
  }
  return 0;
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


/***********************************************************/
/* Label Generator */

label_gen_t mk_label_gen()
{
  label_gen_t l = (label_gen_t)malloc(sizeof(struct label_gen));

  l->count = 0;
  l->label = (char *) safe_malloc( sizeof(char) );
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
  lg->label = (char *) safe_malloc( sizeof(char) * (digit+3) );

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


/******************************************************/
/* Error */

void logic_error(char * filename, int line, char * content)
{
  printf("Error at line %d in file %s\n%s\n", line, filename, content);
  exit(1);
}
