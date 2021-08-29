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

%{
int yyerror ();
%}

%left '|' '&'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/'
%left '~'

%%

program: declist
  ;

declist: data funclist
  |
  ;

data: KW_DATA '{' vardeclist '}'

vardeclist: vardec ';' vardeclist
  |
  ;

vardec: type ':' TK_IDENTIFIER '=' LIT_INTEGER
  | type ':' TK_IDENTIFIER '=' LIT_CHAR
  | vector
  ;

vector: type '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER
  | type '[' LIT_INTEGER OPERATOR_RANGE LIT_INTEGER ']' ':' TK_IDENTIFIER '=' vectordeclaration
  ;

vectordeclaration: LIT_INTEGER vectordeclaration
  | LIT_CHAR vectordeclaration
  |
  ;

funclist: func funclist
  |
  ;

func: type ':' TK_IDENTIFIER '(' ')' cmd_block
  | type ':' TK_IDENTIFIER '(' parameter parameterlist ')' cmd_block
  ;

parameterlist: ',' parameter parameterlist
  |
  ;

parameter: type ':' TK_IDENTIFIER

cmd_block: '{' cmd cmdlist '}'
  ;

cmdlist: ';' cmd cmdlist
  |
  ;

cmd: TK_IDENTIFIER 
  | TK_IDENTIFIER '=' expr
  | TK_IDENTIFIER '[' expr ']' '=' expr
  | KW_PRINT printelement printlist
  | KW_RETURN expr
  | KW_IF expr cmd_block
  | KW_IF expr cmd
  | KW_IF expr cmd_block KW_ELSE cmd_block
  | KW_IF expr cmd_block KW_ELSE cmd
  | KW_IF expr cmd KW_ELSE cmd
  | KW_UNTIL expr cmd_block
  | KW_UNTIL expr cmd
  | cmd_block
  | KW_COMEFROM ':' TK_IDENTIFIER
  |
  ;

printlist: ',' printelement printlist
  |
  ;

printelement: LIT_STRING
  | expr
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

funccall: TK_IDENTIFIER '(' ')'
  | TK_IDENTIFIER '(' expr exprlist ')'
  ;

exprlist: ',' expr exprlist
  |
  ;

type: KW_CHAR
  | KW_INT
  | KW_FLOAT
  ;

%%

int yyerror () {
  fprintf(stderr, "Syntax error at line %d.\n", getLineNumber());
  exit(3);
}
