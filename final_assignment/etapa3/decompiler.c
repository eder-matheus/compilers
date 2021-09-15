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

#include "decompiler.h"

#include <stdio.h>
#include <stdlib.h>

void decompileSYMBOL(AST* tree_node, FILE *output) {
  fprintf(output, "%s", tree_node->symbol->text);
}

void decompileADD(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " + ");
  decompile(tree_node->son[1], output);
}

void decompileSUB(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " - ");
  decompile(tree_node->son[1], output);
}

void decompileMULT(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " *  ");
  decompile(tree_node->son[1], output);
}

void decompileDIV(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " / ");
  decompile(tree_node->son[1], output);
}

void decompileOR(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " | ");
  decompile(tree_node->son[1], output);
}

void decompileAND(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " & ");
  decompile(tree_node->son[1], output);
}

void decompileNOT(AST* tree_node, FILE *output) {
  fprintf(output, "~");
  decompile(tree_node->son[0], output);
}

void decompileLESSER(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " < ");
  decompile(tree_node->son[1], output);
}

void decompileGREATER(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " > ");
  decompile(tree_node->son[1], output);
}

void decompileLE(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " <= ");
  decompile(tree_node->son[1], output);
}

void decompileGE(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " >= ");
  decompile(tree_node->son[1], output);
}

void decompileEQ(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " == ");
  decompile(tree_node->son[1], output);
}

void decompileDIFF(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " != ");
  decompile(tree_node->son[1], output);
}

void decompileRANGE(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, "..");
  decompile(tree_node->son[1], output);
}

void decompileATTR(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, " = ");
  decompile(tree_node->son[1], output);
}

void decompilePRINT(AST* tree_node, FILE *output) {
  fprintf(output, "print ");
  decompile(tree_node->son[0], output);
  decompile(tree_node->son[1], output);
}

void decompileRETURN(AST* tree_node, FILE *output) {
  fprintf(output, "return ");
  decompile(tree_node->son[0], output);
}

void decompileIF(AST* tree_node, FILE *output) {
  fprintf(output, "if ");
  decompile(tree_node->son[0], output);
  fprintf(output, "\n");
  decompile(tree_node->son[1], output);
}

void decompileIF_ELSE(AST* tree_node, FILE *output) {
  fprintf(output, "if ");
  decompile(tree_node->son[0], output);
  fprintf(output, "\n");
  decompile(tree_node->son[1], output);
  fprintf(output, "\n");
  fprintf(output, "else ");
  fprintf(output, "\n");
  decompile(tree_node->son[2], output);
}

void decompileUNTIL(AST* tree_node, FILE *output) {
  fprintf(output, "until ");
  decompile(tree_node->son[0], output);
  fprintf(output, "\n");
  decompile(tree_node->son[1], output);
}

void decompileCOMEFROM(AST* tree_node, FILE *output) {
  fprintf(output, "comefrom: ");
  decompile(tree_node->son[0], output);
}

void decompileREAD(AST* tree_node, FILE *output) {
  fprintf(output, "read");
}

void decompilePARENTHESIS(AST* tree_node, FILE *output) {
  fprintf(output, "(");
  decompile(tree_node->son[0], output);
  fprintf(output, ")");
}

void decompileVEC_ACCESS(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, "[");
  decompile(tree_node->son[1], output);
  fprintf(output, "]");
}

void decompileDECLIST(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, "\n");
  decompile(tree_node->son[1], output);
}

void decompileCMDLIST(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, ";\n");
  decompile(tree_node->son[1], output);
}

void decompileEXPRLIST(AST* tree_node, FILE *output) {
  fprintf(output, ", ");
  decompile(tree_node->son[0], output);
  decompile(tree_node->son[1], output);
}

void decompilePRINTLIST(AST* tree_node, FILE *output) {
  fprintf(output, ", ");
  decompile(tree_node->son[0], output);
  decompile(tree_node->son[1], output);
}

void decompilePARAMETERLIST(AST* tree_node, FILE *output) {
  fprintf(output, ", ");
  decompile(tree_node->son[0], output);
  decompile(tree_node->son[1], output);
}

void decompileFUNCLIST(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, "\n");
  decompile(tree_node->son[1], output);
}

void decompileVARDECLIST(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  decompile(tree_node->son[1], output);
}

void decompileVECTOR_INIT_LIST(AST* tree_node, FILE *output) {
  fprintf(output, " ");
  decompile(tree_node->son[0], output);
  decompile(tree_node->son[1], output);
}

void decompileFUNC_CALL(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, "()");
}

void decompileFUNC_CALL_PARMS(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, "(");
  decompile(tree_node->son[1], output);
  decompile(tree_node->son[2], output);
  fprintf(output, ")");
}

void decompileFUNC_DEC(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, ": ");
  decompile(tree_node->son[1], output);
  fprintf(output, "() ");
  fprintf(output, "\n");
  decompile(tree_node->son[2], output);
}

void decompileFUNC_DEC_PARMS(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, ": ");
  decompile(tree_node->son[1], output);
  fprintf(output, "(");
  decompile(tree_node->son[2]->son[0], output);
  decompile(tree_node->son[2]->son[1], output);
  fprintf(output, ")");
  fprintf(output, "\n");
  decompile(tree_node->son[3], output);
}

void decompileATTR_VEC(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, "[");
  decompile(tree_node->son[1], output);
  fprintf(output, "] = ");
  decompile(tree_node->son[2], output);
}

void decompileCMD_BLOCK(AST* tree_node, FILE *output) {
  fprintf(output, "{\n");
  decompile(tree_node->son[0], output);
  fprintf(output, "}");
}

void decompileVECTOR_DECLARATION(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, "[");
  decompile(tree_node->son[1], output);
  fprintf(output, "]: ");
  decompile(tree_node->son[2], output);
  fprintf(output, ";\n");
}

void decompileVECTOR_INIT(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, "[");
  decompile(tree_node->son[1], output);
  fprintf(output, "]: ");
  decompile(tree_node->son[2], output);
  fprintf(output, " = ");
  decompile(tree_node->son[3]->son[0], output);
  decompile(tree_node->son[3]->son[1], output);
  fprintf(output, ";\n");
}

void decompileVARDEC(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, ": ");
  decompile(tree_node->son[1], output);
  fprintf(output, " = ");
  decompile(tree_node->son[2], output);
  fprintf(output, ";\n");
}

void decompileDATA(AST* tree_node, FILE *output) {
  fprintf(output, "data {\n");
  decompile(tree_node->son[0], output);
  fprintf(output, "}");
}

void decompilePARAMETER(AST* tree_node, FILE *output) {
  decompile(tree_node->son[0], output);
  fprintf(output, ": ");
  decompile(tree_node->son[1], output);
}

void decompile(AST* tree_node, FILE *output) {
  if (!tree_node) {
    return;
  }

  switch (tree_node->type) {
    case AST_SYMBOL:
      decompileSYMBOL(tree_node, output);
      break;
    case AST_ADD:
      decompileADD(tree_node, output);
      break;
    case AST_SUB:
      decompileSUB(tree_node, output);
      break;
    case AST_MULT:
      decompileMULT(tree_node, output);
      break;
    case AST_DIV:
      decompileDIV(tree_node, output);
      break;
    case AST_OR:
      decompileOR(tree_node, output);
      break;
    case AST_AND:
      decompileAND(tree_node, output);
      break;
    case AST_NOT:
      decompileNOT(tree_node, output);
      break;
    case AST_LESSER:
      decompileLESSER(tree_node, output);
      break;
    case AST_GREATER:
      decompileGREATER(tree_node, output);
      break;
    case AST_LE:
      decompileLE(tree_node, output);
      break;
    case AST_GE:
      decompileGE(tree_node, output);
      break;
    case AST_EQ:
      decompileEQ(tree_node, output);
      break;
    case AST_DIFF:
      decompileDIFF(tree_node, output);
      break;
    case AST_RANGE:
      decompileRANGE(tree_node, output);
      break;
    case AST_ATTR:
      decompileATTR(tree_node, output);
      break;
    case AST_PRINT:
      decompilePRINT(tree_node, output);
      break;
    case AST_RETURN:
      decompileRETURN(tree_node, output);
      break;
    case AST_IF:
      decompileIF(tree_node, output);
      break;
    case AST_IF_ELSE:
      decompileIF_ELSE(tree_node, output);
      break;
    case AST_UNTIL:
      decompileUNTIL(tree_node, output);
      break;
    case AST_COMEFROM:
      decompileCOMEFROM(tree_node, output);
      break;
    case AST_READ:
      decompileREAD(tree_node, output);
      break;
    case AST_PARENTHESIS:
      decompilePARENTHESIS(tree_node, output);
      break;
    case AST_VEC_ACCESS:
      decompileVEC_ACCESS(tree_node, output);
      break;
    case AST_DECLIST:
      decompileDECLIST(tree_node, output);
      break;
    case AST_CMDLIST:
      decompileCMDLIST(tree_node, output);
      break;
    case AST_EXPRLIST:
      decompileEXPRLIST(tree_node, output);
      break;
    case AST_PRINTLIST:
      decompilePRINTLIST(tree_node, output);
      break;
    case AST_PARAMETERLIST:
      decompilePARAMETERLIST(tree_node, output);
      break;
    case AST_FUNCLIST:
      decompileFUNCLIST(tree_node, output);
      break;
    case AST_VARDECLIST:
      decompileVARDECLIST(tree_node, output);
      break;
    case AST_VECTOR_INIT_LIST:
      decompileVECTOR_INIT_LIST(tree_node, output);
      break;
    case AST_FUNC_CALL:
      decompileFUNC_CALL(tree_node, output);
      break;
    case AST_FUNC_CALL_PARMS:
      decompileFUNC_CALL_PARMS(tree_node, output);
      break;
    case AST_FUNC_DEC:
      decompileFUNC_DEC(tree_node, output);
      break;
    case AST_FUNC_DEC_PARMS:
      decompileFUNC_DEC_PARMS(tree_node, output);
      break;
    case AST_ATTR_VEC:
      decompileATTR_VEC(tree_node, output);
      break;
    case AST_CMD_BLOCK:
      decompileCMD_BLOCK(tree_node, output);
      break;
    case AST_VECTOR_DECLARATION:
      decompileVECTOR_DECLARATION(tree_node, output);
      break;
    case AST_VECTOR_INIT:
      decompileVECTOR_INIT(tree_node, output);
      break;
    case AST_VARDEC:
      decompileVARDEC(tree_node, output);
      break;
    case AST_DATA:
      decompileDATA(tree_node, output);
      break;
    case AST_PARAMETER:
      decompilePARAMETER(tree_node, output);
      break;
    default:
      break;
  }
	return;
}