// print how many numbers there are on each logical line ending with ;
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
  FILE *arq;
  arq = fopen("in2.txt", "rt");
  if (arq == NULL) {
    printf("Error: file in2.txt not opened\n");
    exit(1);
  }

  char* result;
  char line[100];
  int buf;
  int offset;
  int numbers_count = 0;
  while (!feof(arq)) {
    result = fgets(line, 100, arq);
    if (result == NULL) {
      break;
    }

    char* line_ptr = line;
    while (sscanf(line_ptr, "%d%n", &buf, &offset) == 1 && *line_ptr) {
      numbers_count++;
      line_ptr += offset;
    }
    if (strchr(line, ';') != NULL) {
      printf("%d ", numbers_count);
      numbers_count = 0;
    }
  }
  printf("\n");

  fclose(arq);
  exit(0);
}