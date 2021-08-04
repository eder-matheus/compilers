// print a function with n int parms
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
  int i, parms;

  if (argc < 2) {
    exit(1);
  }

  parms = atoi(argv[1]);

  printf("func (");
  if (parms > 0)
    printf("int p0");
  for (i = 1; i < parms; i++) {
    printf(", int p%d", i);
  }
  printf(")\n");
  printf("{\n\n}\n");

  exit(0);
}