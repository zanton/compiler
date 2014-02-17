typedef struct char_buf
{
  char *string;
  int count_string;
}* char_buf_t;

char_buf_t mk_char_buf();

void setChar(char_buf_t t, char c);
