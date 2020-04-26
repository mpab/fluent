%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {CMD}
%define api.parser.class {Parser}

%code requires{
   namespace CMD {
      class Driver;
      class Scanner;
   }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "driver.hpp"

  #undef yylex
  #define yylex scanner.yylex

  using T = CMD::Parser::token_type;
}

%define api.value.type variant
%define parse.assert

%token END    0     "end of file"
%token NL
%token SPC
%token ABORT

%token <std::string> TEXT
%token <std::string> REAL
%token <std::string> QSTRING

%token COMMENT

%token <node> CASE
%token <node> DO
%token <node> WHILE
%token <node> OUT
%type <node> expression

%token OPERATOR

%token <node> SYMBOL
%token BAD_SYMBOL
%token INTEGER
%token BAD_REAL

%token GEQ
%token LEQ
%token ISEQ
%token NEQ

%left GEQ LEQ ISEQ NEQ '>' '<'
%left '+' '-'
%left '*' '/'
%left '^'
%precedence NEG
%precedence SYMBOL

%locations

%%

command: END | list END;

list
: item
| list item
;

item
:
| NL        { driver.newline(); }
| SPC       { driver.space(); }
| TEXT      { driver.text($1); }
| ABORT     { driver.abort(); }
;

assignment
: SYMBOL '=' SYMBOL ';'         { $$ = addi('=', 2, $1, $3); }
| SYMBOL '=' expression ';'     { $$ = addi('=', 2, $1, $3); }
| SYMBOL '=' statements ';'     { $$ = addi('=', 2, $1, $3); }
;

group
: statements
| CASE evaluation scope         { $$ = addi(CASE, 2, $2, $3); }
| WHILE evaluation scope        { $$ = addi(WHILE, 2, $2, $3); }
| DO scope WHILE evaluation     { $$ = addi(DO, 2, $2, $4); }
;

scope
: '{' statements '}'
;

evaluation
: '(' expression ')'            { $$ = $2; } 
;

statements
: statement                     { $$ = $1; }
| statements statement          { $$ = addi(';', 2, $1, $2); }
;

statement
: ';'                           { $$ = addi(';', 2, 0, 0); }
| expression ';'                { $$ = $1; }
| OUT expression ';'            { $$ = addi(OUT, 1, $2); }
;

expression
: '-' expression %prec NEG      { $$ = addi(NEG, 1, $2); }
| expression '+' expression     { $$ = addi('+', 2, $1, $3); }
| expression '-' expression     { $$ = addi('-', 2, $1, $3); }
| expression '*' expression     { $$ = addi('*', 2, $1, $3); }
| expression '/' expression     { $$ = addi('/', 2, $1, $3); }
| expression '<' expression     { $$ = addi('<', 2, $1, $3); }
| expression '>' expression     { $$ = addi('>', 2, $1, $3); }
| expression GEQ expression     { $$ = addi(GEQ, 2, $1, $3); }
| expression LEQ expression     { $$ = addi(LEQ, 2, $1, $3); }
| expression NEQ expression     { $$ = addi(NEQ, 2, $1, $3); }
| expression ISEQ expression    { $$ = addi(ISEQ, 2, $1, $3); }
| expression '^' expression     { $$ = addi('^', 2, $1, $3); }
;

%%

void CMD::Parser::error(const location_type &l, const std::string &err_message) {
   std::cerr << err_message << std::endl;
}
