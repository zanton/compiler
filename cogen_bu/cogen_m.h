#include <stdio.h>

typedef struct label_gen *label_gen_t;

struct label_gen
{
  int count;
  char * label;
  char * return_label;
  char * break_label;
  char * continue_label;
};



/************************************************/
label_gen_t mk_label_gen();
char* gen_label(label_gen_t lg);

char * lg_get_break_label(label_gen_t lg);
void lg_reset_break_label(label_gen_t lg);
char * lg_get_return_label(label_gen_t lg);
void lg_reset_return_label(label_gen_t lg);
char * lg_get_continue_label(label_gen_t lg);
void lg_reset_continue_label(label_gen_t lg);
