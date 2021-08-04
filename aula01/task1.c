// count how many numbers exists in in1.txt
#include <stdlib.h>
#include <stdio.h>

int main()
{
  FILE *arq;
  arq = fopen("in1.txt", "rt");
  if (arq == NULL) {
    printf("Error: file in1.txt not opened\n");
    exit(1);
  }

  int numbers = 0;
  int num;
  while (fscanf(arq, "%d", &num) == 1) {
    numbers++;
  }

  printf("Total numbers: %d\n", numbers);

  fclose(arq);
  exit(0);
}