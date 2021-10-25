%{
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
  #include "ast.h"
  #include "y.tab.h"
  int Running = 1;
  int lineNumber =  1;

  int isRunning(void) {
    return Running;
  }

  int getLineNumber(void) {
    return lineNumber;
  }

  void initMe(void) {
    hashInit();
  }
%}

%x COMMENT

%%

[,;:()\[\]{}+-]|[\*]|[/<>|&~=]  { return yytext[0]; }

"char"                          { yylval.symbol = hashInsert(yytext, SYMBOL_KW_CHAR);
                                  return KW_CHAR; }
"int"                           { yylval.symbol = hashInsert(yytext, SYMBOL_KW_INT);
                                  return KW_INT; }
"float"                         { yylval.symbol = hashInsert(yytext, SYMBOL_KW_FLOAT);
                                  return KW_FLOAT; }
"data"                          { yylval.symbol = hashInsert(yytext, SYMBOL_KW_DATA);
                                  return KW_DATA; }

"if"                            { return KW_IF; }
"else"                          { return KW_ELSE; }
"until"                         { return KW_UNTIL; }
"comefrom"                      { return KW_COMEFROM; }
"read"                          { return KW_READ; }
"print"                         { return KW_PRINT; }
"return"                        { return KW_RETURN; }


"<="                            { return OPERATOR_LE; }
">="                            { return OPERATOR_GE; }
"=="                            { return OPERATOR_EQ; }
"!="                            { return OPERATOR_DIF; }
".."                            { return OPERATOR_RANGE; }

[a-zA-Z]+[a-zA-Z0-9 ]*[a-zA-Z0-9]+|[a-zA-Z]+  { yylval.symbol = hashInsert(yytext, SYMBOL_IDENTIFIER);
                                                return TK_IDENTIFIER;
                                              }

[0-9]+[0-9]*                    { yylval.symbol = hashInsert(yytext, SYMBOL_LIT_INTEGER);
                                  return LIT_INTEGER;
                                }
"\'"."\'"                       { yylval.symbol = hashInsert(yytext, SYMBOL_LIT_CHAR);
                                  return LIT_CHAR;
                                }
\"([^\\\"\n]|\\.)*\"              { yylval.symbol = hashInsert(yytext, SYMBOL_LIT_STRING);
                                    return LIT_STRING;
                                  }



[ \t]
"\n"  lineNumber++;
"##".*  
"#/"  BEGIN(COMMENT);

.     { return TOKEN_ERROR; }

<COMMENT>.
<COMMENT>"\n" lineNumber++;
<COMMENT>"/#" BEGIN(INITIAL);

%%

int yywrap() {
  Running = 0;
  return 1;
}
