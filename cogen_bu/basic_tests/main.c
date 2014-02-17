#include <stdio.h>

int main(int argc, char ** argv)
{
#define N 10
  int a[N];
  int i;
  for (i = 1; i < N; i++) {
    if (i > 0 && i < argc) {
      a[i] = atoi(argv[i]);
    } else {
      a[i] = i * 10;
    }
  }

  int x = f(a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
  printf("%d\n", x);
  return 0;
}


