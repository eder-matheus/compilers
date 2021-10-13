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
#include "hash.h"
#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin;
extern AST *root;
int yyparse();
void initMe();

int main(int argc, char **argv) {
  initMe();
  if (argc < 3) {
    fprintf(stderr, "Call: etapa4 inFileName outFileName\n");
    exit(1);
  }

  yyin = fopen(argv[1], "r");
  if (yyin == 0) {
    fprintf(stderr, "Cannot open file %s\n", argv[1]);
    exit(2);
  }

  FILE *out = fopen(argv[2], "w");
  if (out == 0) {
    fprintf(stderr, "Cannot open file %s\n", argv[2]);
    exit(2);
  }

  yyparse();

  hashPrint();
  decompile(root, out);

  if (semantic_errors_count > 0) {
    fprintf(stderr, "Compilation ended with %d semantic errors\n",
            semantic_errors_count);
    exit(4);
  }

  printf("Compilation successfull \n");
  return 0;
}
