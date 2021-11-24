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
    exit(1);
  }

  int token;
  while (isRunning()) {
    token = yylex();
    if (!isRunning()) {
      break;
    }

    switch(token) {
      case KW_CHAR:       printf("Palavra reservada char encontrada\n"); break;
      case KW_INT:        printf("Palavra reservada int encontrada\n"); break;
      case KW_FLOAT:      printf("Palavra reservada float encontrada\n"); break;
      case KW_DATA:       printf("Palavra reservada data encontrada\n"); break;

      case KW_IF:         printf("Palavra reservada if encontrada\n"); break;
      case KW_ELSE:       printf("Palavra reservada else encontrada\n"); break;
      case KW_UNTIL:      printf("Palavra reservada until encontrada\n"); break;
      case KW_COMEFROM:   printf("Palavra reservada comefrom encontrada\n"); break;
      case KW_READ:       printf("Palavra reservada read encontrada\n"); break;
      case KW_PRINT:      printf("Palavra reservada print encontrada\n"); break;
      case KW_RETURN:     printf("Palavra reservada return encontrada\n"); break;

      case OPERATOR_LE:   printf("Operador <= encontrado\n"); break;
      case OPERATOR_GE:   printf("Operador >= encontrado\n"); break;
      case OPERATOR_EQ:   printf("Operador == encontrado\n"); break;
      case OPERATOR_DIF:  printf("Operador != encontrado\n"); break;
      case TK_IDENTIFIER: printf("Identificador %s encontrado na linha %d\n", yytext, getLineNumber()); break;

      case LIT_INTEGER:   printf("Literal inteiro %s encontrado na linha %d\n", yytext, getLineNumber()); break;
      case LIT_CHAR:      printf("Literal char %s encontrado na linha %d\n", yytext, getLineNumber()); break;
      case LIT_STRING:    printf("Literal string %s encontrado na linha %d\n", yytext, getLineNumber()); break;

      case TOKEN_ERROR: printf("ERRO: entrada %s nao identificada\n", yytext); break;
      default: printf("Simbolo %s encontrado na linha %d\n", yytext, getLineNumber()); break;
    }
  }

  hashPrint();
  exit(0);
}