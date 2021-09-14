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

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

AST *astCreate(int type, HASH_NODE *symbol, AST *s0, AST *s1, AST *s2,
               AST *s3) {
  AST *new_node = (AST *)calloc(1, sizeof(AST));
  new_node->type = type;
  new_node->symbol = symbol;
  new_node->son[0] = s0;
  new_node->son[1] = s1;
  new_node->son[2] = s2;
  new_node->son[3] = s3;
  return new_node;
}

AST *createSymbol(HASH_NODE * symbol) {
  return astCreate(AST_SYMBOL, symbol, 0, 0, 0, 0);
}

AST *createList(int type, AST *head, AST* tail) {
  return astCreate(type, 0, head, tail, 0, 0);
}

void astPrint(AST *node, int level) {
  int i = 0;
  if (node == 0) {
    return;
  }
  for (i = 0; i < level; i++) {
    fprintf(stderr, "  ");
  }
  fprintf(stderr, "ast(");
  switch (node->type) {
  case AST_SYMBOL:
    fprintf(stderr, "AST_SYMBOL");
    break;
  case AST_ADD:
    fprintf(stderr, "AST_ADD");
    break;
  case AST_SUB:
    fprintf(stderr, "AST_SUB");
    break;
  case AST_ATTR:
    fprintf(stderr, "AST_ATTR");
    break;
  case AST_CMDLIST:
    fprintf(stderr, "AST_CMDLIST");
    break;
  default:
    fprintf(stderr, "AST_UNKNOWN");
    break;
  }
  if (node->symbol != 0) {
    fprintf(stderr, ", %s\n", node->symbol->text);
  } else {
    fprintf(stderr, ", 0)\n");
  }

  for (i = 0; i < MAX_SONS; i++) {
    astPrint(node->son[i], level + 1);
  }
}
