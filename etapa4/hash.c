// BSD 3-Clause License
//
// Copyright (c) 2021, Eder Monteiro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "hash.h"

HASH_NODE *Table[HASH_SIZE];

void hashInit(void) {
  int i;
  for (i = 0; i < HASH_SIZE; i++) {
    Table[i] = 0;
  }
}

int hashAddress(char *text) {
  int addr = 1;
  int i;
  for (i = 0; i < strlen(text); i++) {
    addr = (addr * text[i]) % HASH_SIZE + 1;
  }

  return addr - 1;
}

HASH_NODE *hashFind(char *text) {
  HASH_NODE *node;
  int addr = hashAddress(text);
  for (node = Table[addr]; node; node = node->next) {
    if (strcmp(node->text, text) == 0) {
      return node;
    }
  }
  return 0;
}

HASH_NODE *hashInsert(char *text, int type) {
  int addr = hashAddress(text);
  HASH_NODE *node = hashFind(text);

  // if element already existis in the table, return it
  if (node != 0) {
    return node;
  }

  node = (HASH_NODE *)calloc(1, sizeof(HASH_NODE));
  node->type = type;
  node->text = (char *)calloc(strlen(text) + 1, sizeof(char));
  strcpy(node->text, text);
  node->parameters = NULL;

  // The new node will be the head of the linked list
  node->next = Table[addr];
  Table[addr] = node;
  return node;
}

void addParameter(HASH_NODE *node, int type, int data_type) {
  PARAMETER *param;
  param = (PARAMETER *)calloc(1, sizeof(PARAMETER));
  param->type = type;
  param->data_type = data_type;
  if (node->parameters != NULL) {
    PARAMETER *param_list = node->parameters;
    while (param_list->next != NULL) {
      param_list = param_list->next;
    }

    param_list->next = param;
  } else {
    node->parameters = param;
  }
}

void hashPrint(void) {
  int i;
  HASH_NODE *node;
  for (i = 0; i < HASH_SIZE; i++) {
    for (node = Table[i]; node; node = node->next) {
      printf("Table[%d] has (%s, type %i)\n", i, node->text, node->type);
    }
  }
}