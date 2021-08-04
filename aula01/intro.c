// print 10 lines, from 1 to 10, each with the numbers from one to the line number
#include <stdlib.h>
#include <stdio.h>

int main()
{
  int i, j;
  for (i = 1; i <= 10; i++) {
    for (j = 1; j <= i; j++) {
      printf("%d ", j);
    }
    printf("\n");
  }

  exit(0);
}
