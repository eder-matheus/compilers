%{
  #include "hash.h"
  #include "ast.h"
  int yyerror ();
  int getLineNumber();
  int yylex();
%}

%union
{
  HASH_NODE *symbol;
  AST *ast;
}

%token<symbol> KW_CHAR
%token<symbol> KW_INT
%token<symbol> KW_FLOAT
%token<symbol> KW_DATA

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

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%token TOKEN_ERROR

%type<ast> expr
%type<ast> cmd
%type<ast> cmdlist
%type<ast> funccall
%type<ast> type
%type<ast> exprlist
%type<ast> printlist
%type<ast> printelement
%type<ast> parameter
%type<ast> parameterlist
%type<ast> func
%type<ast> funclist
%type<ast> cmd_block
%type<ast> vector
%type<ast> vectordeclaration
%type<ast> vardec
%type<ast> vardeclist
%type<ast> data
%type<ast> declist
%type<ast> program

%left '|' '&'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'
%left '~'

%%

program: declist                               { $$ = $1; astPrint($1, 0); }
  ;

declist: data funclist                         { $$ = createList(AST_DECLIST, $1, $2); }
  |                                            { $$ = 0; }
  ;

data: KW_DATA '{' vardeclist '}'               { $$ = astCreate(AST_DATA, 0, $3, 0, 0, 0); }

vardeclist: vardec ';' vardeclist              { $$ = createList(AST_VARDECLIST, $1, $3); }
  |                                            { $$ = 0; }
  ;

vardec: type ':' TK_IDENTIFIER '=' LIT_INTEGER { $$ = astCreate(AST_VARDEC, 0, $1, (createSymbol($3)), (createSymbol($5)), 0); }
  | type ':' TK_IDENTIFIER '=' LIT_CHAR        { $$ = astCreate(AST_VARDEC, 0, $1, (createSymbol($3)), (createSymbol($5)), 0); }
  | vector                                     { $$ = astCreate(AST_VARDEC, 0, $1, 0, 0, 0); }
  ;

vector: type '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER   { 
            $$ = astCreate(AST_VECTOR_DECLARATION, 0, $1, (
            astCreate(AST_RANGE, 0, (
              createSymbol($3)), (
              createSymbol($5)), 0, 0)), 
          (createSymbol($8)), 0);
        }
  | type '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER '=' vectordeclaration  { 
        $$ = astCreate(AST_VECTOR_DECLARATION, 0, $1, (
        astCreate(AST_RANGE, 0, (
          createSymbol($3)), (
          createSymbol($5)), 0, 0)), 
      (createSymbol($8)), $10);
    }
  ;

vectordeclaration: LIT_INTEGER vectordeclaration  { $$ = astCreate(AST_VECTOR_DECLARATION, 0, (createSymbol($1)), $2, 0, 0); }
  | LIT_CHAR vectordeclaration                    { $$ = astCreate(AST_VECTOR_DECLARATION, 0, (createSymbol($1)), $2, 0, 0); }
  |                                               { $$ = 0; }
  ;

funclist: func funclist                     { $$ = createList(AST_FUNCLIST, $1, $2); }
  |                                         { $$ = 0; }
  ;

func: type ':' TK_IDENTIFIER '(' ')' cmd_block                        { $$ = astCreate(AST_FUNC_DEC, 0, $1, (createSymbol($3)), $6, 0); }
  | type ':' TK_IDENTIFIER '(' parameter parameterlist ')' cmd_block  {
      $$ = astCreate(AST_FUNC_DEC_PARMS, 0, $1, (
        createSymbol($3)), 
      createList(AST_PARAMETERLIST, $5, $6), $8); }
  ;

parameterlist: ',' parameter parameterlist { $$ = createList(AST_PARAMETERLIST, $2, $3); }
  |                                        { $$ = 0; }
  ;

parameter: type ':' TK_IDENTIFIER          { $$ = astCreate(AST_PARAMETERLIST, 0, $1, (createSymbol($3)), 0, 0); }

cmd_block: '{' cmdlist '}'                { $$ = astCreate(AST_CMD_BLOCK, 0, $2, 0, 0, 0); }
  ;

cmdlist: cmd ';' cmdlist                  { $$ = createList(AST_CMDLIST, $1, $3); }
  |                                       { $$ = 0; }
  ;

cmd: TK_IDENTIFIER                        { $$ = createSymbol($1); }
  | TK_IDENTIFIER '=' expr                { $$ = astCreate(AST_ATTR, 0, (createSymbol($1)), $3, 0, 0); }
  | TK_IDENTIFIER '[' expr ']' '=' expr   { $$ = astCreate(AST_ATTR_VEC, 0, (createSymbol($1)), $3, $6, 0); }
  | KW_PRINT printelement printlist       { $$ = astCreate(AST_PRINT, 0, $2, $3, 0, 0); }
  | KW_RETURN expr                        { $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0); }
  | KW_IF expr cmd                        { $$ = astCreate(AST_IF, 0, $2, $3, 0, 0); }
  | KW_IF expr cmd KW_ELSE cmd            { $$ = astCreate(AST_IF_ELSE, 0, $2, $3, $5, 0); }
  | KW_UNTIL expr cmd                     { $$ = astCreate(AST_UNTIL, 0, $2, $3, 0, 0); }
  | cmd_block                             { $$ = $1; }
  | KW_COMEFROM ':' TK_IDENTIFIER         { $$ = astCreate(AST_COMEFROM, 0, (createSymbol($3)), 0, 0, 0); }
  |                                       { $$ = 0; }
  ;

printlist: ',' printelement printlist   { $$ = createList(AST_PRINTLIST, $2, $3); }
  |                                     { $$ = 0; }
  ;

printelement: LIT_STRING          { $$ = createSymbol($1); }
  | expr                          { $$ = astCreate(AST_PRINTLIST, 0, $1, 0, 0, 0); }
  ;

expr: LIT_INTEGER                 { $$ = createSymbol($1); }
  | LIT_CHAR                      { $$ = createSymbol($1); }
  | TK_IDENTIFIER                 { $$ = createSymbol($1); }
  | TK_IDENTIFIER '[' expr ']'    { $$ = astCreate(AST_VEC_ACCESS, 0, (createSymbol($1)), $3, 0, 0); }
  | expr '+' expr                 { $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0); }
  | expr '-' expr                 { $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0); }
  | expr '*' expr                 { $$ = astCreate(AST_MULT, 0, $1, $3, 0, 0); }
  | expr '/' expr                 { $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0); }
  | expr '<' expr                 { $$ = astCreate(AST_LESSER, 0, $1, $3, 0, 0); }
  | expr '>' expr                 { $$ = astCreate(AST_GREATER, 0, $1, $3, 0, 0); }
  | expr '|' expr                 { $$ = astCreate(AST_OR, 0, $1, $3, 0, 0); }
  | expr '&' expr                 { $$ = astCreate(AST_AND, 0, $1, $3, 0, 0); }
  | '~' expr                      { $$ = astCreate(AST_NOT, 0, $2, 0, 0, 0); }
  | expr OPERATOR_LE expr         { $$ = astCreate(AST_LE, 0, $1, $3, 0, 0); }
  | expr OPERATOR_GE expr         { $$ = astCreate(AST_GE, 0, $1, $3, 0, 0); }
  | expr OPERATOR_EQ expr         { $$ = astCreate(AST_EQ, 0, $1, $3, 0, 0); }
  | expr OPERATOR_DIF expr        { $$ = astCreate(AST_DIFF, 0, $1, $3, 0, 0); }
  | '(' expr ')'                  { $$ = astCreate(AST_PARENTHESIS, 0, $2, 0, 0, 0); }
  | KW_READ                       { $$ = astCreate(AST_READ, 0, 0, 0, 0, 0); }
  | funccall                      { $$ = $1; }
  ;

funccall: TK_IDENTIFIER '(' ')'         { $$ = astCreate(AST_FUNC_CALL, 0, (createSymbol($1)), 0, 0, 0); }
  | TK_IDENTIFIER '(' expr exprlist ')' { $$ = astCreate(AST_FUNC_CALL_PARMS, 0, (createSymbol($1)), $3, $4, 0); }
  ;

exprlist: ',' expr exprlist       { $$ = createList(AST_EXPRLIST, $2, $3); }
  |                               { $$ = 0; }
  ;

type: KW_CHAR                     { $$ = createSymbol($1); }
  | KW_INT                        { $$ = createSymbol($1); }
  | KW_FLOAT                      { $$ = createSymbol($1); }
  ;

%%

int yyerror () {
  fprintf(stderr, "Syntax error at line %d.\n", getLineNumber());
  exit(3);
}
