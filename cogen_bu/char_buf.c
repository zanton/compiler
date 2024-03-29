#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "char_buf.h"

char_buf_t mk_char_buf()
{
  char_buf_t t = (char_buf_t)malloc(sizeof(struct char_buf));
  t->str_len = 1;

  t->str = malloc(sizeof(char)*t->str_len);
  t->str[0] = '\0';

  return t;
}

void char_buf_addchar(char_buf_t t, char c)
{  
  char *temp_char;
  temp_char = malloc(sizeof(char)*t->str_len);

  int i;
  for(i=0; i < t->str_len-1; i++){
    temp_char[i] = t->str[i];
  }

  t->str_len++;
  free(t->str);
  t->str = malloc(sizeof(char)*t->str_len);

  for(i=0; i < t->str_len-2; i++){
    t->str[i] = temp_char[i];
  }
  
  t->str[t->str_len-2] = c;
  t->str[t->str_len-1] = '\0';
}

void char_buf_clear(char_buf_t t)
{
  free(t->str);
  t->str = malloc(sizeof(char)*1);

  t->str[0] = '\0';
  t->str_len = 1;
}

char* char_buf_getstr(char_buf_t t)
{
  return t->str;
}

void char_buf_cpy(char_buf_t t1, char_buf_t t2)
{
  if (t1 == NULL)
    t1 = mk_char_buf();
  else 
    free(t1->str);

  t1->str = malloc(sizeof(char)*t2->str_len);
  strcpy(t1->str, t2->str);
  t1->str_len = t2->str_len;
}

int char_buf_getlen(char_buf_t t)
{
  return t->str_len;
}
