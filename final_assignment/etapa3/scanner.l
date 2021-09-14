%{

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

"char"                          { return KW_CHAR; }
"int"                           { return KW_INT; }
"float"                         { return KW_FLOAT; }
"data"                          { return KW_DATA; }

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
