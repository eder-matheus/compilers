// print how many numbers there are on each physical line
#include <stdlib.h>
#include <stdio.h>

int main()
{
  FILE *arq;
  arq = fopen("in2.txt", "rt");
  if (arq == NULL) {
    printf("Error: file in2.txt not opened\n");
    exit(1);
  }

  int lines = 0;
  char* result;
  char line[100];
  int buf;
  int offset;
  while (!feof(arq)) {
    int numbers_count = 0;
    result = fgets(line, 100, arq);
    if (result != NULL) {
      lines++;
    } else {
      break;
    }

    char* line_ptr = line;
    while (sscanf(line_ptr, "%d%n", &buf, &offset) == 1 && *line_ptr) {
      numbers_count++;
      line_ptr += offset;
    }
    printf("%d ", numbers_count);
  }

  printf("Num lines: %d\n", lines);

  fclose(arq);
  exit(0);
}