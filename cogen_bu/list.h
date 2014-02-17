/* 
 * list.h
 */
/* 注: これは同じヘッダファイルが2度includeされたとき,
   2回目以降は読み込まれないようにするためのおまじない
   (コンパイラへの指示) */
#pragma once

/* void* のリスト */
typedef struct list * list_t;

list_t mk_list();
void list_add(list_t l, void * x);
void * list_get(list_t l, int i);
int list_sz(list_t l);


