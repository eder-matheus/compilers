#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 997 // requires a prime number
#define SYMBOL_LIT_INTEGER 1
#define SYMBOL_LIT_CHAR 2
#define SYMBOL_LIT_STRING 3
#define SYMBOL_IDENTIFIER 7

typedef struct hash_node {
  int type;
  char *text;
  struct hash_node *next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);

#endif
