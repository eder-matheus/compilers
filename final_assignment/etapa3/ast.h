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

#ifndef AST_HEADER
#define AST_HEADER

#define MAX_SONS 5

#define AST_SYMBOL 1

#define AST_ADD 2
#define AST_SUB 3
#define AST_MULT 4
#define AST_DIV 5

#define AST_OR 6
#define AST_AND 7
#define AST_NOT 8

#define AST_LESSER 9
#define AST_GREATER 10
#define AST_LE 11
#define AST_GE 12
#define AST_EQ 13
#define AST_DIFF 14

#define AST_RANGE 15

#define AST_ATTR 16

#define AST_PRINT 17
#define AST_RETURN 18
#define AST_IF 19
#define AST_IF_ELSE 20
#define AST_UNTIL 21
#define AST_COMEFROM 22
#define AST_READ 23

#define AST_KW_CHAR 24
#define AST_KW_INT 25
#define AST_KW_FLOAT 26

#define AST_PARENTHESIS 27
#define AST_BRACKETS 28
#define AST_KEYS 29

#define AST_VEC_ACCESS 30

#define AST_DECLIST 31
#define AST_CMDLIST 32
#define AST_EXPRLIST 33
#define AST_PRINTLIST 34
#define AST_PARAMETERLIST 35
#define AST_FUNCLIST 35
#define AST_VARDECLIST 36

#define AST_FUNC_CALL 40
#define AST_FUNC_CALL_PARMS 41
#define AST_FUNC_DEC 42
#define AST_FUNC_DEC_PARMS 43

#define AST_ATTR_VEC 44
#define AST_CMD_BLOCK 45
#define AST_VECTOR_DECLARATION 46
#define AST_VARDEC 47
#define AST_DATA 48


typedef struct ast_node {
  int type;
  HASH_NODE *symbol;
  struct ast_node *son[MAX_SONS];
} AST;

AST *astCreate(int type, HASH_NODE *symbol, AST *s0, AST *s1, AST *s2, AST *s3, AST *s4);
void astPrint(AST *node, int level);

#endif
