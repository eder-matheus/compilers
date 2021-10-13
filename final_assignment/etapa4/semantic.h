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

#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

int semantic_errors_count;

void semanticVerification(AST *node);

void updateDeclarations(AST *node);

void findUndeclaredIdentifiers();

void checkVariableDeclaration(AST *node);

void checkVectorInitialization(AST *node);

void checkAttribution(AST *node);

void checkVectorAttribution(AST *node);

void checkNumericalAndComparatorOperator(AST *node);

void checkLogicalOperator(AST *node);

void checkNotOperator(AST *node);

void checkEqualDiffOperator(AST *node);

void checkVectorAccess(AST *node);

void checkIfElseUntil(AST *node);

void checkFunctionCall(AST *node);

void checkFunctionWithParamsCall(AST *node);

void checkSymbol(AST *node);

void checkReturnOperator(AST *node);

void verifyOperand(AST *node, int ignore);

int detectDeclarationDataType(int type);

int detectLiteralDataType(int type);

int areTypesCompatible(int type1, int type2);

int getSymbolType(AST *node);

int getNumericalOperatorsType(AST *node);

int getLogicalOperatorsType(AST *node);

int getNotType(AST *node);

int getComparatorOperatorsType(AST *node);

int getEqualDiffType(AST *node);

int getType(AST *node);

#endif
