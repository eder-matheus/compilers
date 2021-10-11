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

AST *createSymbol(HASH_NODE *symbol) {
  return astCreate(AST_SYMBOL, symbol, 0, 0, 0, 0);
}

AST *createList(int type, AST *head, AST *tail) {
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
  case AST_MULT:
    fprintf(stderr, "AST_MULT");
    break;
  case AST_DIV:
    fprintf(stderr, "AST_DIV");
    break;
  case AST_OR:
    fprintf(stderr, "AST_OR");
    break;
  case AST_AND:
    fprintf(stderr, "AST_AND");
    break;
  case AST_NOT:
    fprintf(stderr, "AST_NOT");
    break;
  case AST_LESSER:
    fprintf(stderr, "AST_LESSER");
    break;
  case AST_GREATER:
    fprintf(stderr, "AST_GREATER");
    break;
  case AST_LE:
    fprintf(stderr, "AST_LE");
    break;
  case AST_GE:
    fprintf(stderr, "AST_GE");
    break;
  case AST_EQ:
    fprintf(stderr, "AST_EQ");
    break;
  case AST_DIFF:
    fprintf(stderr, "AST_DIFF");
    break;
  case AST_RANGE:
    fprintf(stderr, "AST_RANGE");
    break;
  case AST_ATTR:
    fprintf(stderr, "AST_ATTR");
    break;
  case AST_PRINT:
    fprintf(stderr, "AST_PRINT");
    break;
  case AST_RETURN:
    fprintf(stderr, "AST_RETURN");
    break;
  case AST_IF:
    fprintf(stderr, "AST_IF");
    break;
  case AST_IF_ELSE:
    fprintf(stderr, "AST_IF_ELSE");
    break;
  case AST_UNTIL:
    fprintf(stderr, "AST_UNTIL");
    break;
  case AST_COMEFROM:
    fprintf(stderr, "AST_COMEFROM");
    break;
  case AST_READ:
    fprintf(stderr, "AST_READ");
    break;
  case AST_PARENTHESIS:
    fprintf(stderr, "AST_PARENTHESIS");
    break;
  case AST_VEC_ACCESS:
    fprintf(stderr, "AST_VEC_ACCESS");
    break;
  case AST_DECLIST:
    fprintf(stderr, "AST_DECLIST");
    break;
  case AST_CMDLIST:
    fprintf(stderr, "AST_CMDLIST");
    break;
  case AST_EXPRLIST:
    fprintf(stderr, "AST_EXPRLIST");
    break;
  case AST_PRINTLIST:
    fprintf(stderr, "AST_PRINTLIST");
    break;
  case AST_PARAMETERLIST:
    fprintf(stderr, "AST_PARAMETERLIST");
    break;
  case AST_FUNCLIST:
    fprintf(stderr, "AST_FUNCLIST");
    break;
  case AST_VARDECLIST:
    fprintf(stderr, "AST_VARDECLIST");
    break;
  case AST_FUNC_CALL:
    fprintf(stderr, "AST_FUNC_CALL");
    break;
  case AST_FUNC_CALL_PARMS:
    fprintf(stderr, "AST_FUNC_CALL_PARMS");
    break;
  case AST_FUNC_DEC:
    fprintf(stderr, "AST_FUNC_DEC");
    break;
  case AST_FUNC_DEC_PARMS:
    fprintf(stderr, "AST_FUNC_DEC_PARMS");
    break;
  case AST_ATTR_VEC:
    fprintf(stderr, "AST_ATTR_VEC");
    break;
  case AST_CMD_BLOCK:
    fprintf(stderr, "AST_CMD_BLOCK");
    break;
  case AST_VECTOR_DECLARATION:
    fprintf(stderr, "AST_VECTOR_DECLARATION");
    break;
  case AST_VECTOR_INIT:
    fprintf(stderr, "AST_VECTOR_INIT");
    break;
  case AST_VARDEC:
    fprintf(stderr, "AST_VARDEC");
    break;
  case AST_DATA:
    fprintf(stderr, "AST_DATA");
    break;
  case AST_PARAMETER:
    fprintf(stderr, "AST_PARAMETER");
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
