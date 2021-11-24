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

#include "semantic.h"
#include "ast.h"
#include "hash.h"
#include <math.h>

int semantic_errors_count = 0;

void semanticVerification(AST *node) {
  updateDeclarations(node);
  verifyOperand(node, 0);
  findUndeclaredIdentifiers();
}

void updateDeclarations(AST *node) {
  if (node == NULL) {
    return;
  }

  if (node->type == AST_VARDEC) { // check variable
    if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER) {
      fprintf(stderr, "[ERROR] Redeclaration of %s\n",
              node->son[1]->symbol->text);
      semantic_errors_count++;
    }

    node->son[1]->symbol->type = SYMBOL_VARIABLE;
    node->son[1]->symbol->data_type =
        detectDeclarationDataType(node->son[0]->symbol->type);
  } else if (node->type == AST_VECTOR_DECLARATION) { // check vector declaration
    if (node->son[2]->symbol->type != SYMBOL_IDENTIFIER) {
      fprintf(stderr, "[ERROR] Redeclaration of %s\n",
              node->son[2]->symbol->text);
      semantic_errors_count++;
    }

    node->son[2]->symbol->type = SYMBOL_VECTOR;
    node->son[2]->symbol->data_type =
        detectDeclarationDataType(node->son[0]->symbol->type);
  } else if (node->type == AST_VECTOR_INIT) { // check vector initialization
    if (node->son[2]->symbol->type != SYMBOL_IDENTIFIER) {
      fprintf(stderr, "[ERROR] Redeclaration of %s\n",
              node->son[2]->symbol->text);
      semantic_errors_count++;
    }

    node->son[2]->symbol->type = SYMBOL_VECTOR;
    node->son[2]->symbol->data_type =
        detectDeclarationDataType(node->son[0]->symbol->type);
  } else if (node->type == AST_FUNC_DEC) { // check function
    if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER) {
      fprintf(stderr, "[ERROR] Redeclaration of %s\n",
              node->son[1]->symbol->text);
      semantic_errors_count++;
    }

    node->son[1]->symbol->type = SYMBOL_FUNCTION;
    node->son[1]->symbol->data_type =
        detectDeclarationDataType(node->son[0]->symbol->type);
  } else if (node->type ==
             AST_FUNC_DEC_PARMS) { // check function with parameters
    if (node->son[1]->symbol->type != SYMBOL_IDENTIFIER) {
      fprintf(stderr, "[ERROR] Redeclaration of %s\n",
              node->son[1]->symbol->text);
      semantic_errors_count++;
    }

    node->son[1]->symbol->type = SYMBOL_FUNCTION;
    node->son[1]->symbol->data_type =
        detectDeclarationDataType(node->son[0]->symbol->type);

    // define the types of the function parameters
    AST *param_list = node->son[2];
    while (param_list != NULL) {
      AST *param = param_list->son[0];
      if (param->son[1]->symbol->type != SYMBOL_IDENTIFIER) {
        fprintf(stderr, "[ERROR] Redeclaration of %s\n",
                param->son[1]->symbol->text);
        semantic_errors_count++;
      }
      param->son[1]->symbol->type = SYMBOL_VARIABLE;
      param->son[1]->symbol->data_type =
          detectDeclarationDataType(param->son[0]->symbol->type);
      param_list = param_list->son[1];
      addParameter(node->son[1]->symbol, param->son[1]->symbol->type,
                   param->son[1]->symbol->data_type);
    }
  } else if (node->type == AST_COMEFROM) { // check comefrom
    if (node->son[0]->symbol->type != SYMBOL_IDENTIFIER) {
      fprintf(stderr, "[ERROR] Comefrom label \'%s\' used more than once\n",
              node->son[0]->symbol->text);
      semantic_errors_count++;
    }

    node->son[0]->symbol->type = SYMBOL_COMEFROM;
    node->son[0]->symbol->data_type = DATATYPE_COMEFROM;
  }

  int i;
  for (i = 0; i < MAX_SONS; i++) {
    updateDeclarations(node->son[i]);
  }
}

void findUndeclaredIdentifiers() {
  int i;
  HASH_NODE *node;
  for (i = 0; i < HASH_SIZE; i++) {
    node = Table[i];
    while (node) {
      if (node->type == SYMBOL_IDENTIFIER) {
        fprintf(stderr, "[ERROR] Undeclared %s identifier\n", node->text);
        semantic_errors_count++;
      }
      node = node->next;
    }
  }
}

void checkVariableDeclaration(AST *node) {
  int var_type = node->son[1]->symbol->data_type;
  int literal_type = detectLiteralDataType(node->son[2]->symbol->type);
  if (areTypesCompatible(var_type, literal_type) == 0) {
    fprintf(stderr, "[ERROR] Variable %s has invalid type declaration\n",
            node->son[1]->symbol->text);
    semantic_errors_count++;
  }
}

void checkVectorInitialization(AST *node) {
  int vector_type = node->son[2]->symbol->data_type;
  AST *range = node->son[1];
  int r0 = strtol(range->son[0]->symbol->text, NULL, 16);
  int r1 = strtol(range->son[1]->symbol->text, NULL, 16);
  int vector_range = abs(r1 - r0) + 1;

  int vector_size = 0;

  AST *vector_data = node->son[3];
  while (vector_data != NULL) {
    vector_size++;
    vector_data = vector_data->son[1];
  }

  if (vector_size > vector_range) {
    fprintf(stderr,
            "[ERROR] Vector %s initialized with %d elements, while vector size "
            "is %d\n",
            node->son[2]->symbol->text, vector_size, vector_range);
    semantic_errors_count++;
  }
}

void checkAttribution(AST *node) {
  int var_type = node->son[0]->symbol->type;
  if (var_type == SYMBOL_VARIABLE) {
    int var_data_type = node->son[0]->symbol->data_type;
    verifyOperand(node->son[1], 0);
    int expr_type = getType(node->son[1]);
    expr_type = (expr_type == 0) ? var_data_type : expr_type;

    if (areTypesCompatible(var_data_type, expr_type) == 0) {
      semantic_errors_count++;
      fprintf(stderr, "[ERROR] Invalid type for attribution to %s\n",
              node->son[0]->symbol->text);
    }
  } else if (var_type == SYMBOL_VECTOR) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] Invalid access to vector %s\n",
            node->son[0]->symbol->text);
  } else {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] %s is not a variable\n",
            node->son[0]->symbol->text);
  }
}

void checkVectorAttribution(AST *node) {
  int var_type = node->son[0]->symbol->type;
  if (var_type == SYMBOL_VECTOR) {
    verifyOperand(node->son[1], 0);
    verifyOperand(node->son[2], 0);

    int var_data_type = node->son[0]->symbol->data_type;
    int index_type = getType(node->son[1]);
    int expr_type = getType(node->son[2]);

    if (index_type != DATATYPE_INTEGER && index_type != DATATYPE_CHAR) {
      semantic_errors_count++;
      fprintf(stderr,
              "[ERROR] Invalid type %d for vector %s access. It needs to be "
              "integer\n",
              index_type, node->son[0]->symbol->text);
    } else if (areTypesCompatible(var_data_type, expr_type) == 0) {
      semantic_errors_count++;
      fprintf(stderr, "[ERROR] Incompatible types on vector %s attribution\n",
              node->son[0]->symbol->text);
    } else if (var_type == SYMBOL_VARIABLE) {
      semantic_errors_count++;
      fprintf(stderr, "[ERROR] Invalid access to variable %s\n",
              node->son[0]->symbol->text);
    }
  } else {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] %s is not a vector\n", node->son[0]->symbol->text);
  }
}

void checkNumericalAndComparatorOperator(AST *node) {
  verifyOperand(node->son[0], 0);
  verifyOperand(node->son[1], 0);

  int expr0_type = getType(node->son[0]);
  int expr1_type = getType(node->son[1]);

  int is_number0 = 0;
  int is_number1 = 0;
  if (expr0_type == DATATYPE_INTEGER || expr0_type == DATATYPE_CHAR ||
      expr0_type == DATATYPE_FLOAT) {
    is_number0 = 1;
  }

  if (expr1_type == DATATYPE_INTEGER || expr1_type == DATATYPE_CHAR ||
      expr1_type == DATATYPE_FLOAT) {
    is_number1 = 1;
  }

  if (is_number0 == 0 || is_number1 == 0) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] Invalid operands in numerical operator\n");
  }
}

void checkLogicalOperator(AST *node) {
  verifyOperand(node->son[0], 0);
  verifyOperand(node->son[1], 0);

  int expr0_type = getType(node->son[0]);
  int expr1_type = getType(node->son[1]);

  if (expr0_type != DATATYPE_BOOL || expr1_type != DATATYPE_BOOL) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] Invalid operands in logical operator\n");
  }
}

void checkNotOperator(AST *node) {
  verifyOperand(node->son[0], 0);
  int expr_type = getType(node->son[0]);

  if (expr_type != DATATYPE_BOOL) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] Invalid operand in NOT operator\n");
  }
}

void checkEqualDiffOperator(AST *node) {
  verifyOperand(node->son[0], 0);
  verifyOperand(node->son[1], 0);

  int expr0_type = getType(node->son[0]);
  int expr1_type = getType(node->son[1]);

  if (areTypesCompatible(expr0_type, expr1_type) == 0) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] Invalid operands in equal/diff operator\n");
  }
}

void checkVectorAccess(AST *node) {
  int type = node->son[0]->symbol->type;
  if (type != SYMBOL_VECTOR) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] %s is not a vector\n", node->son[0]->symbol->text);
  } else {
    int index_type = getType(node->son[1]);
    if (index_type != DATATYPE_INTEGER) {
      semantic_errors_count++;
      fprintf(
          stderr,
          "[ERROR] Invalid type for vector %s access. It needs to be integer\n",
          node->son[0]->symbol->text);
    }
  }
}

void checkIfElseUntil(AST *node) {
  verifyOperand(node->son[0], 0);
  int expr_type = getType(node->son[0]);

  if (expr_type != DATATYPE_BOOL) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] Invalid expression in if/else/until clausule. It "
                    "needs to be a bool\n");
  }
}

void checkFunctionCall(AST *node) {
  int type = node->son[0]->symbol->type;
  AST *func = node->son[0];
  int parms_cnt = 0;
  PARAMETER *param_list = func->symbol->parameters;
  while (param_list != NULL) {
    parms_cnt++;
    param_list = param_list->next;
  }

  if (type != SYMBOL_FUNCTION) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] %s is not a function\n",
            node->son[0]->symbol->text);
  } else if (node->son[1] != NULL) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] function %s does not expect parameters\n",
            node->son[0]->symbol->text);
  } else if (parms_cnt > 0) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] function %s expect %d parameters\n",
            node->son[0]->symbol->text, parms_cnt);
  }
}

void checkFunctionWithParamsCall(AST *node) {
  int type = node->son[0]->symbol->type;
  if (type != SYMBOL_FUNCTION) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] %s is not a function\n",
            node->son[0]->symbol->text);
    return;
  }

  AST *func = node->son[0];
  int parms_cnt = 0;
  PARAMETER *param_list = func->symbol->parameters;
  while (param_list != NULL) {
    parms_cnt++;
    param_list = param_list->next;
  }

  int args_cnt = 0;
  AST *arg = node->son[1];
  if (arg != NULL) {
    args_cnt++;
  }
  AST *arg_list = node->son[2];
  while (arg_list != NULL) {
    args_cnt++;
    arg_list = arg_list->son[1];
  }

  if (parms_cnt != args_cnt) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] Function %s expect %d parameters, given %d\n",
            node->son[0]->symbol->text, parms_cnt, args_cnt);
    return;
  }

  param_list = func->symbol->parameters;
  arg_list = node->son[2];
  arg = node->son[1];
  while (arg_list != NULL) {
    int param_type = param_list->data_type;
    int arg_type = getType(arg);
    if (areTypesCompatible(param_type, arg_type) == 0) {
      semantic_errors_count++;
      fprintf(stderr, "[ERROR] Invalid parameter type for function %s\n",
              node->son[0]->symbol->text);
    }
    param_list = param_list->next;
    arg = arg_list->son[0];
    arg_list = arg_list->son[1];
  }
}

void checkSymbol(AST *node) {
  if (node->symbol->type != SYMBOL_VARIABLE &&
      node->symbol->data_type != DATATYPE_CHAR &&
      node->symbol->data_type != DATATYPE_INTEGER &&
      node->symbol->data_type != DATATYPE_FLOAT &&
      node->symbol->data_type != DATATYPE_BOOL &&
      node->symbol->data_type != DATATYPE_STRING &&
      node->symbol->data_type != DATATYPE_COMEFROM &&
      detectDeclarationDataType(node->symbol->type) == 0 &&
      detectLiteralDataType(node->symbol->type) == 0) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] %s is not a variable neither a literal\n",
            node->symbol->text);
  }
}

void checkReturnOperator(AST *node) {
  int type = getType(node->son[0]);
  if (type == DATATYPE_BOOL) {
    semantic_errors_count++;
    fprintf(stderr, "[ERROR] Invalid return type\n");
  }
}

void verifyOperand(AST *node, int ignore) {
  if (node == NULL) {
    return;
  }

  if (node->type == AST_VARDEC) {
    checkVariableDeclaration(node);
  } else if (node->type == AST_VECTOR_INIT) {
    checkVectorInitialization(node);
  } else if (node->type == AST_ATTR) {
    checkAttribution(node);
  } else if (node->type == AST_ATTR_VEC) {
    checkVectorAttribution(node);
  } else if (node->type == AST_ADD || node->type == AST_SUB ||
             node->type == AST_DIV || node->type == AST_MULT ||
             node->type == AST_GREATER || node->type == AST_LESSER ||
             node->type == AST_GE || node->type == AST_LE) {
    checkNumericalAndComparatorOperator(node);
  } else if (node->type == AST_AND || node->type == AST_OR) {
    checkLogicalOperator(node);
  } else if (node->type == AST_NOT) {
    checkNotOperator(node);
  } else if (node->type == AST_EQ || node->type == AST_DIFF) {
    checkEqualDiffOperator(node);
  } else if (node->type == AST_VEC_ACCESS) {
    checkVectorAccess(node);
  } else if (node->type == AST_IF || node->type == AST_IF_ELSE ||
             node->type == AST_UNTIL) {
    checkIfElseUntil(node);
  } else if (node->type == AST_FUNC_CALL) {
    if (ignore != 0)
      checkFunctionCall(node);
  } else if (node->type == AST_FUNC_CALL_PARMS) {
    if (ignore != 0)
      checkFunctionWithParamsCall(node);
  } else if (node->type == AST_SYMBOL) {
    checkSymbol(node);
  } else if (node->type == AST_PARENTHESIS) {
    verifyOperand(node->son[0], 0);
  } else if (node->type == AST_RETURN) {
    checkReturnOperator(node);
  }

  int i;
  for (i = 0; i < MAX_SONS; i++) {
    verifyOperand(node->son[i], 1);
  }
}

int detectDeclarationDataType(int type) {
  if (type == SYMBOL_KW_INT) {
    return DATATYPE_INTEGER;
  }
  if (type == SYMBOL_KW_CHAR) {
    return DATATYPE_CHAR;
  }
  if (type == SYMBOL_KW_FLOAT) {
    return DATATYPE_FLOAT;
  }

  return 0;
}

int detectLiteralDataType(int type) {
  if (type == SYMBOL_LIT_INTEGER) {
    return DATATYPE_INTEGER;
  }
  if (type == SYMBOL_LIT_CHAR) {
    return DATATYPE_CHAR;
  }
  if (type == SYMBOL_LIT_STRING) {
    return DATATYPE_STRING;
  }

  return 0;
}

int areTypesCompatible(int type1, int type2) {
  int is_number1 = 0;
  int is_number2 = 0;
  if (type1 == DATATYPE_INTEGER || type1 == DATATYPE_CHAR ||
      type1 == DATATYPE_FLOAT) {
    is_number1 = 1;
  }

  if (type2 == DATATYPE_INTEGER || type2 == DATATYPE_CHAR ||
      type2 == DATATYPE_FLOAT) {
    is_number2 = 1;
  }

  if (is_number1 == is_number2) {
    return 1;
  }

  return 0;
}

int getSymbolType(AST *node) {
  int type = -1;
  if (node->symbol->type == SYMBOL_VARIABLE ||
      node->symbol->type == SYMBOL_VECTOR ||
      node->symbol->type == SYMBOL_FUNCTION) {
    type = node->symbol->data_type;
  } else if (node->symbol->type == SYMBOL_LIT_INTEGER ||
             node->symbol->type == SYMBOL_LIT_CHAR ||
             node->symbol->type == SYMBOL_LIT_STRING) {
    type = detectLiteralDataType(node->symbol->type);
  }

  if (type == -1) {
    fprintf(stderr, "[ERROR] Invalid type for %s\n", node->symbol->text);
    semantic_errors_count++;
  }

  return type;
}

int getNumericalOperatorsType(AST *node) {
  int expr0_type = getType(node->son[0]);
  int expr1_type = getType(node->son[1]);

  int is_number0 = 0;
  int is_number1 = 0;
  if (expr0_type == DATATYPE_INTEGER || expr0_type == DATATYPE_CHAR ||
      expr0_type == DATATYPE_FLOAT) {
    is_number0 = 1;
  }

  if (expr1_type == DATATYPE_INTEGER || expr1_type == DATATYPE_CHAR ||
      expr1_type == DATATYPE_FLOAT) {
    is_number1 = 1;
  }

  if (is_number0 == 1 && is_number1 == 1) {
    int type = (expr0_type > expr1_type) ? expr0_type : expr1_type;
    return type;
  }

  semantic_errors_count++;
  return -1;
}

int getLogicalOperatorsType(AST *node) {
  int expr0_type = getType(node->son[0]);
  int expr1_type = getType(node->son[1]);

  if (expr0_type == DATATYPE_BOOL && expr1_type == DATATYPE_BOOL) {
    return DATATYPE_BOOL;
  }

  semantic_errors_count++;
  return -1;
}

int getNotType(AST *node) {
  int expr_type = getType(node->son[0]);

  if (expr_type == DATATYPE_BOOL) {
    return DATATYPE_BOOL;
  }

  semantic_errors_count++;
  return -1;
}

int getComparatorOperatorsType(AST *node) {
  int expr0_type = getType(node->son[0]);
  int expr1_type = getType(node->son[1]);

  int is_number0 = 0;
  int is_number1 = 0;
  if (expr0_type == DATATYPE_INTEGER || expr0_type == DATATYPE_CHAR ||
      expr0_type == DATATYPE_FLOAT) {
    is_number0 = 1;
  }

  if (expr1_type == DATATYPE_INTEGER || expr1_type == DATATYPE_CHAR ||
      expr1_type == DATATYPE_FLOAT) {
    is_number1 = 1;
  }

  if (is_number0 == 1 && is_number1 == 1) {
    return DATATYPE_BOOL;
  }

  semantic_errors_count++;
  return -1;
}

int getEqualDiffType(AST *node) {
  int expr0_type = getType(node->son[0]);
  int expr1_type = getType(node->son[1]);

  if (areTypesCompatible(expr0_type, expr1_type) == 1) {
    return DATATYPE_BOOL;
  }

  semantic_errors_count++;
  return -1;
}

int getType(AST *node) {
  int type = -1;
  if (node->type == AST_SYMBOL) {
    type = getSymbolType(node);
  } else if (node->type == AST_ADD || node->type == AST_SUB ||
             node->type == AST_MULT || node->type == AST_DIV) {
    type = getNumericalOperatorsType(node);
  } else if (node->type == AST_OR || node->type == AST_AND) {
    type = getLogicalOperatorsType(node);
  } else if (node->type == AST_NOT) {
    type = getNotType(node);
  } else if (node->type == AST_LESSER || node->type == AST_GREATER ||
             node->type == AST_LE || node->type == AST_GE) {
    type = getComparatorOperatorsType(node);
  } else if (node->type == AST_EQ || node->type == AST_DIFF) {
    type = getEqualDiffType(node);
  } else if (node->type == AST_PARENTHESIS) {
    type = getType(node->son[0]);
  } else if (node->type == AST_VEC_ACCESS) {
    type = getType(node->son[0]);
  } else if (node->type == AST_FUNC_CALL || node->type == AST_FUNC_CALL_PARMS) {
    type = getType(node->son[0]);
  } else if (node->type == AST_READ) {
    return 0;
  }

  return type;
}
