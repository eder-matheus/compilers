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

#include "tacs.h"

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2) {
  TAC *new_tac = NULL;
  new_tac = (TAC *)calloc(1, sizeof(TAC));
  new_tac->type = type;
  new_tac->res = res;
  new_tac->op1 = op1;
  new_tac->op2 = op2;
  new_tac->prev = NULL;
  new_tac->next = NULL;

  return new_tac;
}

void tacPrint(TAC *tac) {
  if (!tac) {
    return;
  }

  if (tac->type == TAC_SYMBOL) {
    return;
  }

  printf("TAC(");
  switch (tac->type) {
  case TAC_ADD:
    printf("TAC_ADD");
    break;
  case TAC_SUB:
    printf("TAC_SUB");
    break;
  case TAC_MUL:
    printf("TAC_MUL");
    break;
  case TAC_DIV:
    printf("TAC_DIV");
    break;

  case TAC_OR:
    printf("TAC_OR");
    break;
  case TAC_AND:
    printf("TAC_AND");
    break;
  case TAC_NOT:
    printf("TAC_NOT");
    break;

  case TAC_GREATER:
    printf("TAC_GREATER");
    break;
  case TAC_LESSER:
    printf("TAC_LESSER");
    break;
  case TAC_LE:
    printf("TAC_LE");
    break;
  case TAC_GE:
    printf("TAC_GE");
    break;
  case TAC_EQ:
    printf("TAC_EQ");
    break;
  case TAC_DIFF:
    printf("TAC_DIFF");
    break;

  case TAC_COPY:
    printf("TAC_COPY");
    break;
  case TAC_COPY_VEC:
    printf("TAC_COPY_VEC");
    break;

  case TAC_PRINT:
    printf("TAC_PRINT");
    break;
  case TAC_RETURN:
    printf("TAC_RETURN");
    break;
  case TAC_READ:
    printf("TAC_READ");
    break;

  case TAC_JUMP:
    printf("TAC_JUMP");
    break;
  case TAC_JUMP_FALSE:
    printf("TAC_JUMP_FALSE");
    break;
  case TAC_JUMP_TRUE:
    printf("TAC_JUMP_TRUE");
    break;
  case TAC_LABEL:
    printf("TAC_LABEL");
    break;
  case TAC_BEGINFUN:
    printf("TAC_BEGINFUN");
    break;
  case TAC_ENDFUN:
    printf("TAC_ENDFUN");
    break;
  case TAC_FUNCALL:
    printf("TAC_FUNCALL");
    break;
  case TAC_FUNC_ARGS:
    printf("TAC_FUNC_ARGS");
    break;
  case TAC_VEC_ACCESS:
    printf("TAC_VEC_ACCESS");
    break;
  default:
    printf("TAC_UNKNOWN");
    break;
  }
  printf(", %s", (tac->res) ? tac->res->text : "0");
  printf(", %s", (tac->op1) ? tac->op1->text : "0");
  printf(", %s", (tac->op2) ? tac->op2->text : "0");
  printf(");\n");
}
void tacPrintBackwards(TAC *tac) {
  if (!tac) {
    return;
  }

  tacPrintBackwards(tac->prev);
  tacPrint(tac);
}

TAC *tacJoin(TAC *l1, TAC *l2) {
  TAC *point;
  if (!l1)
    return l2;
  if (!l2)
    return l1;

  for (point = l2; point->prev != 0; point = point->prev) {
    ;
  }

  point->prev = l1;
  return l2;
}

// code generation - auxiliar functions

TAC *makeFunction(HASH_NODE *symbol, TAC *code) {
  return tacJoin(tacJoin(tacCreate(TAC_BEGINFUN, symbol, 0, 0), code),
                 tacCreate(TAC_ENDFUN, symbol, 0, 0));
}

TAC *makeBinaryOperands(int tac_op, TAC *code0, TAC *code1) {
  return tacJoin(tacJoin(code0, code1),
                 tacCreate(tac_op, makeTemp(), code0 ? code0->res : NULL,
                           code1 ? code1->res : NULL));
}

TAC *makeUnaryOperands(int tac_op, TAC *code0) {
  return tacJoin(
      code0, tacCreate(tac_op, makeTemp(), code0 ? code0->res : NULL, NULL));
}

TAC *makeIf(TAC *code0, TAC *code1) {
  TAC *jump_false_tac = 0;
  TAC *label_tac = 0;
  HASH_NODE *new_label = 0;
  new_label = makeLabel();

  jump_false_tac = tacCreate(TAC_JUMP_FALSE, new_label, code0->res, NULL);
  jump_false_tac->prev = code0;

  label_tac = tacCreate(TAC_LABEL, new_label, NULL, NULL);
  label_tac->prev = code1;

  return tacJoin(jump_false_tac, label_tac);
}

TAC *makeIfElse(TAC *code0, TAC *code1, TAC *code2) {
  TAC *jump_false_tac = 0;
  TAC *jump_abs_tac = 0;
  TAC *label_false_tac = 0;
  TAC *label_end_tac = 0;

  HASH_NODE *else_label = 0;
  else_label = makeLabel();

  jump_false_tac = tacCreate(TAC_JUMP_FALSE, else_label, code0->res, NULL);
  jump_false_tac->prev = code0;

  HASH_NODE *end_label = 0;
  end_label = makeLabel();

  jump_abs_tac = tacCreate(TAC_JUMP, end_label, NULL, NULL);
  jump_abs_tac->prev = code1;

  label_false_tac = tacCreate(TAC_LABEL, else_label, NULL, NULL);
  label_false_tac->prev = jump_abs_tac;

  label_end_tac = tacCreate(TAC_LABEL, end_label, NULL, NULL);
  label_end_tac->prev = code2;

  return tacJoin(jump_false_tac, tacJoin(label_false_tac, label_end_tac));
}

TAC *makeUntil(TAC *code0, TAC *code1) {
  TAC *jump_start_tac = 0;
  TAC *jump_true_tac = 0;

  TAC *label_begin_tac = 0;
  TAC *label_end_tac = 0;

  HASH_NODE *start_label = 0;
  start_label = makeLabel();

  label_begin_tac = tacCreate(TAC_LABEL, start_label, NULL, NULL);

  jump_start_tac = tacCreate(TAC_JUMP, start_label, NULL, NULL);
  jump_start_tac->prev = code1;

  HASH_NODE *end_label = 0;
  end_label = makeLabel();

  jump_true_tac = tacCreate(TAC_JUMP_TRUE, end_label, code0->res, NULL);
  jump_true_tac->prev = code0;

  label_end_tac = tacCreate(TAC_LABEL, end_label, NULL, NULL);
  label_end_tac->prev = jump_start_tac;

  return tacJoin(label_begin_tac, tacJoin(jump_true_tac, label_end_tac));
}

TAC *makeComefrom(TAC *code0) {
  TAC *jump_tac = 0;
  HASH_NODE *new_label = 0;
  new_label = makeLabel();

  jump_tac = tacCreate(TAC_JUMP, new_label, NULL, NULL);

  return tacJoin(code0, jump_tac);
}

// code generation
TAC *generateCode(AST *node) {
  if (!node)
    return NULL;

  int i;
  TAC *code[MAX_SONS];
  for (i = 0; i < MAX_SONS; i++) {
    code[i] = generateCode(node->son[i]);
  }

  TAC *res = NULL;

  switch (node->type) {
  case AST_SYMBOL:
    res = tacCreate(TAC_SYMBOL, node->symbol, NULL, NULL);
    break;

  // binary expressions
  case AST_ADD:
    res = makeBinaryOperands(TAC_ADD, code[0], code[1]);
    break;
  case AST_SUB:
    res = makeBinaryOperands(TAC_SUB, code[0], code[1]);
    break;
  case AST_MULT:
    res = makeBinaryOperands(TAC_MUL, code[0], code[1]);
    break;
  case AST_DIV:
    res = makeBinaryOperands(TAC_DIV, code[0], code[1]);
    break;
  case AST_OR:
    res = makeBinaryOperands(TAC_OR, code[0], code[1]);
    break;
  case AST_AND:
    res = makeBinaryOperands(TAC_AND, code[0], code[1]);
    break;
  case AST_LESSER:
    res = makeBinaryOperands(TAC_LESSER, code[0], code[1]);
    break;
  case AST_GREATER:
    res = makeBinaryOperands(TAC_GREATER, code[0], code[1]);
    break;
  case AST_LE:
    res = makeBinaryOperands(TAC_LE, code[0], code[1]);
    break;
  case AST_GE:
    res = makeBinaryOperands(TAC_GE, code[0], code[1]);
    break;
  case AST_EQ:
    res = makeBinaryOperands(TAC_EQ, code[0], code[1]);
    break;
  case AST_DIFF:
    res = makeBinaryOperands(TAC_DIFF, code[0], code[1]);
    break;

  // not expression
  case AST_NOT:
    res = makeUnaryOperands(TAC_NOT, code[0]);
    break;

  // attributions
  case AST_ATTR:
    res = tacJoin(code[1], tacCreate(TAC_COPY, code[0] ? code[0]->res : NULL,
                                     code[1] ? code[1]->res : NULL, NULL));
    break;
  case AST_ATTR_VEC:
    res = tacJoin(
        code[1],
        tacJoin(code[2], tacCreate(TAC_COPY_VEC, code[0] ? code[0]->res : NULL,
                                   code[1] ? code[1]->res : NULL,
                                   code[2] ? code[2]->res : NULL)));
    break;

  // print and return
  case AST_PRINT:
  case AST_PRINTLIST:
    res = tacJoin(
        tacJoin(code[0], tacCreate(TAC_PRINT, code[0] ? code[0]->res : NULL,
                                   NULL, NULL)),
        code[1]);
    break;
  case AST_RETURN:
    res = tacJoin(code[0], tacCreate(TAC_RETURN, code[0] ? code[0]->res : NULL,
                                     NULL, NULL));
    break;

  // flow control
  case AST_IF:
    res = makeIf(code[0], code[1]);
    break;
  case AST_IF_ELSE:
    res = makeIfElse(code[0], code[1], code[2]);
    break;
  case AST_UNTIL:
    res = makeUntil(code[0], code[1]);
    break;
  case AST_READ:
    res = tacCreate(TAC_READ, NULL, NULL, NULL);
    break;

  // function declaration
  case AST_FUNC_DEC:
    res = makeFunction(node->son[1]->symbol, code[2]);
    break;
  case AST_FUNC_DEC_PARMS:
    res = makeFunction(node->son[1]->symbol, code[3]);
    break;

  // function call
  case AST_FUNC_CALL:
    res =
        tacCreate(TAC_FUNCALL, makeTemp(), code[0] ? code[0]->res : NULL, NULL);
    break;
  case AST_FUNC_CALL_PARMS:;
    TAC *tac_func_call = tacCreate(TAC_FUNCALL, makeTemp(), code[0]->res, NULL);
    res = tacJoin(
        tacJoin(tacJoin(code[1],
                        tacCreate(TAC_FUNC_ARGS, code[1] ? code[1]->res : NULL,
                                  NULL, NULL)),
                code[2]),
        tac_func_call);
    break;
  case AST_EXPRLIST:
    res = tacJoin(
        tacJoin(code[0], tacCreate(TAC_FUNC_ARGS, code[0] ? code[0]->res : NULL,
                                   NULL, NULL)),
        code[1]);
    break;

  // vector access
  case AST_VEC_ACCESS:
    res = tacJoin(code[1], tacCreate(TAC_VEC_ACCESS, makeTemp(),
                                     code[0] ? code[0]->res : NULL,
                                     code[1] ? code[1]->res : NULL));
    break;

  default:
    res = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
    break;
  }

  return res;
}
