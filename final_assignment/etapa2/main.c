#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  initMe();
  if (argc < 2) {
    fprintf(stderr, "Call: etapa1 fileName\n");
    exit(1);
  }

  yyin = fopen(argv[1], "r");
  if (yyin == 0) {
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    exit(2);
  }

  yyparse();

  hashPrint();
  exit(0);
}
