typedef struct char_buf {
  char *str; //string
  int str_len; //string length including '\0'
}* char_buf_t;

char_buf_t mk_char_buf(); //contructor
void char_buf_addchar(char_buf_t t, char c); // add 1 charactor to string
void char_buf_clear(char_buf_t t); // clear string
char* char_buf_getstr(char_buf_t t); //return string
void char_buf_cpy(char_buf_t t1, char_buf_t t2); //copy char_buf_t from t2 to t1
int char_buf_getlen(char_buf_t t); //return str_len

