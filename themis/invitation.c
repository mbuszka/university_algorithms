/*
  Maciej Buszka
  279129
  KLO
*/

#include <stdio.h>

int main() {
  long a, b;
  scanf("%ld %ld", &a, &b);
  long i;
  for (i=a; i<=b; i++) {
    if (i&1) printf("%ld ", i);
  }
  return 0;
}

