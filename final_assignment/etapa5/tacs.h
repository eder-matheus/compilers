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

#ifndef TACS_HEADER
#define TACS_HEADER

#include "ast.h"
#include "hash.h"

#define TAC_SYMBOL 1

#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5

#define TAC_OR 6
#define TAC_AND 7
#define TAC_NOT 8

#define TAC_GREATER 9
#define TAC_LESSER 10
#define TAC_LE 11
#define TAC_GE 12
#define TAC_EQ 13
#define TAC_DIFF 14

#define TAC_COPY 15
#define TAC_COPY_VEC 16

#define TAC_PRINT 17
#define TAC_RETURN 18
#define TAC_IF 19
#define TAC_IF_ELSE 20
#define TAC_UNTIL 21
#define TAC_READ 23

#define TAC_JUMP 24
#define TAC_JUMP_FALSE 25
#define TAC_JUMP_TRUE 26
#define TAC_LABEL 27
#define TAC_BEGINFUN 28
#define TAC_ENDFUN 29
#define TAC_FUNCALL 30
#define TAC_FUNC_ARGS 31
#define TAC_VEC_ACCESS 32

typedef struct tac_node {
  int type;
  HASH_NODE *res;
  HASH_NODE *op1;
  HASH_NODE *op2;
  struct tac_node *prev;
  struct tac_node *next;
} TAC;

TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
TAC *tacJoin(TAC *l1, TAC *l2);
void tacPrint(TAC *tac);
void tacPrintBackwards(TAC *tac);

// code generation
TAC *generateCode(AST *node);

#endif
