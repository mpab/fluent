%{

#include "node.h"
#include "context.h"
#include "console.h"
#include "logger.h"

int yylex(void);

%}

%define parse.error verbose

%union {
    node::Node* node;
};

%token <node> T_SYMBOL
%token <node> T_NUMERIC
%token <node> T_STRING

%token T_CONSOLE_QUIT T_CONSOLE_HELP T_CONSOLE_INSPECT
%token T_CONSOLE_CLEAN T_CONSOLE_FREE T_CONSOLE_EXECUTE
%token T_CONSOLE_LOGGING_ON T_CONSOLE_LOGGING_OFF T_CONSOLE_EXEC_BUFFERED T_CONSOLE_EXEC_IMMEDIATE

%token T_ENDL T_EOF T_ELLIPSIS
%token T_WHILE T_COND T_OUT T_OUTL
%nonassoc T_CONDX
%nonassoc T_ELSE

%type <node> stmt expr stmt_list

%left T_GE T_LE T_EQ T_NE '>' '<'
%left '+' '-'
%left '*' '/'
%left '^'
%precedence T_NEG

%%

program:
        function                { exit(0); }
        ;

function:
          function stmt         { exec($2); }
        | function console      {}
        | /* nullptr */
        ;

console:
        T_CONSOLE_QUIT {
                console::quit();
        }
        | T_CONSOLE_HELP {
                console::help();
        }
        | T_CONSOLE_EXEC_BUFFERED {
                context::set_execute_mode(context::BUFFERED);
        }
        | T_CONSOLE_EXEC_IMMEDIATE {
                context::set_execute_mode(context::IMMEDIATE);
        }
        | T_CONSOLE_INSPECT {
                console::inspect();
        }
        | T_CONSOLE_CLEAN {
                context::free_all_nodes();
        }
        | T_CONSOLE_FREE {
                context::free_unused_nodes();
        }
        | T_CONSOLE_EXECUTE {
                context::execute_blocks();
        }
        | T_CONSOLE_LOGGING_ON {
                logger::on();
        }
        | T_CONSOLE_LOGGING_OFF {
                logger::off();
        }
        ;

stmt:
        ';'                                     { $$ = addi(';', 2, 0, 0); }
        | expr ';'                              { $$ = $1; }
        | T_OUT expr ';'                        { $$ = addi(T_OUT, 1, $2); }
        | T_OUTL expr ';'                       { $$ = addi(T_OUTL, 1, $2); }
        | T_SYMBOL '=' expr ';'                 { $$ = addi('=', 2, $1, $3); }
        | T_WHILE '(' expr ')' stmt             { $$ = addi(T_WHILE, 2, $3, $5); }
        | T_COND '(' expr ')' stmt %prec T_CONDX{ $$ = addi(T_COND, 2, $3, $5); }
        | T_COND '(' expr ')' stmt T_ELSE stmt  { $$ = addi(T_COND, 3, $3, $5, $7); }
        | '{' stmt_list '}'                     { $$ = $2; }
        ;

stmt_list:
          stmt                                  { $$ = $1; }
        | stmt_list stmt                        { $$ = addi(';', 2, $1, $2); }
        ;

expr:
          T_STRING                              { $$ = $1; }
        | T_NUMERIC                             { $$ = $1; }   
        | T_SYMBOL                              { $$ = $1; }
        | '-' expr %prec T_NEG                  { $$ = addi(T_NEG, 1, $2); }
        | expr '+' expr                         { $$ = addi('+', 2, $1, $3); }
        | expr '-' expr                         { $$ = addi('-', 2, $1, $3); }
        | expr '*' expr                         { $$ = addi('*', 2, $1, $3); }
        | expr '/' expr                         { $$ = addi('/', 2, $1, $3); }
        | expr '<' expr                         { $$ = addi('<', 2, $1, $3); }
        | expr '>' expr                         { $$ = addi('>', 2, $1, $3); }
        | expr T_GE expr                        { $$ = addi(T_GE, 2, $1, $3); }
        | expr T_LE expr                        { $$ = addi(T_LE, 2, $1, $3); }
        | expr T_NE expr                        { $$ = addi(T_NE, 2, $1, $3); }
        | expr T_EQ expr                        { $$ = addi(T_EQ, 2, $1, $3); }
        | expr '^' expr                         { $$ = addi('^', 2, $1, $3); }
        | '(' expr ')'                          { $$ = $2; }
        ;
%%
