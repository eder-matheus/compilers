%{
int yyerror ();
%}

%token KW_CHAR
%token KW_INT
%token KW_FLOAT
%token KW_DATA

%token KW_IF
%token KW_ELSE
%token KW_UNTIL
%token KW_COMEFROM
%token KW_READ
%token KW_PRINT
%token KW_RETURN

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF
%token OPERATOR_RANGE

%token TK_IDENTIFIER

%token LIT_INTEGER
%token LIT_CHAR
%token LIT_STRING

%token TOKEN_ERROR

%%

programa: decl
  ;

decl: dec rest
  |
  ;

rest: ',' dec rest 
  |
  ;

dec: KW_INT TK_IDENTIFIER
  | KW_INT TK_IDENTIFIER '(' ')' body
  ;

body: '{' lcmd '}'
  ;

lcmd: cmd lcmd
  |
  ;

cmd: TK_IDENTIFIER '=' expr
  ;

expr: LIT_INTEGER
  | LIT_CHAR
  | TK_IDENTIFIER
  | TK_IDENTIFIER '[' expr ']'
  | expr '+' expr
  | expr '-' expr
  | expr '*' expr
  | expr '/' expr
  | expr '<' expr
  | expr '>' expr
  | expr '|' expr
  | expr '&' expr
  | '~' expr
  | expr OPERATOR_LE expr
  | expr OPERATOR_GE expr
  | expr OPERATOR_EQ expr
  | expr OPERATOR_DIF expr
  | '(' expr ')'
  | KW_READ
  | funccall
  ;

funccall: returntype ':' TK_IDENTIFIER '(' ')'
  | returntype ':' TK_IDENTIFIER '(' ')' expr exprlist
  ;

exprlist: ',' expr exprlist
  |
  ;

%%

int yyerror () {
  fprintf(stderr, "Syntax error at line %d.\n", getLineNumber());
  exit(3);
}
