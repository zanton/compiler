#include <stdio.h>
#include <stdlib.h>

#include "char_buf.h"

char_buf_t mk_char_buf()
{
  char_buf_t t = (char_buf_t)malloc(sizeof(struct char_buf));
  t->count_string = 1;

  t->string = malloc(sizeof(char)*t->count_string);
  t->string[0] = '\0';

  return t;
}

void setChar(char_buf_t t, char c)
{  
  char *temp_char;
  temp_char = malloc(sizeof(char)*t->count_string);

  int i;
  for(i=0; i < t->count_string-1; i++){
    temp_char[i] = t->string[i];
  }

  t->count_string++;
  free(t->string);
  t->string = malloc(sizeof(char)*t->count_string);

  for(i=0; i < t->count_string-2; i++){
    t->string[i] = temp_char[i];
  }
  
  t->string[t->count_string-2] = c;
  t->string[t->count_string-1] = '\0';
}
