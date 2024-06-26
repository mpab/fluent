%{
#if _MSC_VER
#pragma warning(disable: 4505)  // 'yysymbol_name': unreferenced function with internal linkage has been removed
#pragma warning(disable: 4702)  // unreachable code
#endif

#include "context.hpp"
#include "console.hpp"
#include "logger.hpp"

int yylex(void);

%}

%define parse.error verbose
%define api.header.include {"parser.tab.hh"}

%code requires {
#include "node.hpp" 
}

%union {
    node::Node* node;
};

%token <node> T_SYMBOL
%token <node> T_NUMERIC
%token <node> T_STRING
%token <node> T_COND
%token <node> T_LOOP

%token T_CONSOLE_QUIT T_CONSOLE_HELP T_CONSOLE_INSPECT
%token T_CONSOLE_CLEAN T_CONSOLE_FREE T_CONSOLE_EXECUTE
%token T_CONSOLE_LOGGING_ON T_CONSOLE_LOGGING_OFF T_CONSOLE_EXEC_BUFFERED T_CONSOLE_EXEC_IMMEDIATE

%token T_OUT T_OUTL
//%token T_STREAMOP
%token T_ENDL T_EOF T_ELLIPSIS
%token T_WHILE T_IF TOK_IF_EXPLICIT_ELSE TOK_IF_IMPLIED_ELSE
%nonassoc T_IFX
%nonassoc T_ELSE

%token  T_IFNDEF T_UNDEFINED T_EXIT

%type <node> stmt
%type <node> expr
%type <node> stmts
%type <node> conds
%type <node> loops
//%type <node> prints

%left T_GE T_LE T_EQ T_NE '>' '<'
%left '+' '-' '%'
%left '*' '/'
%left '^'
%precedence T_NEG
%precedence T_SYMBOLS

%%

program:
        function                        { exit(0); }
        ;

function:
          function stmt                 { exec($2); }
        | function console              {}
        | %empty /* nullptr */
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
                // TODO - disable when source file is active
                context::inspect();
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

stmt:     ';'                                   { $$ = addi(';', 2, 0, 0); }
        | expr ';'                              { $$ = $1; }
        | conds                                 { $$ = $1; }
        | loops                                 { $$ = $1; }
        | T_EXIT ';'                            { $$ = addi(T_EXIT, 1, 0, 0); }
        | T_OUT expr ';'                        { $$ = addi(T_OUT, 1, $2); }
        | T_OUTL expr ';'                       { $$ = addi(T_OUTL, 1, $2); }
        | T_SYMBOL '=' expr ';'                 { $$ = addi('=', 2, $1, $3); }
        | '{' stmts '}'                         { $$ = $2; }
        ;

// prints:   expr                                 { $$ = $1; }
//         | prints T_STREAMOP expr               { $$ = addi(';', 2, $1, $2); }
//         ;

stmts:    stmt                                  { $$ = $1; }
        | stmts stmt                            { $$ = addi(';', 2, $1, $2); }
        ;

conds:    T_COND                                { $$ = $1; }
        | T_IF '(' expr ')' stmt T_ELSE stmt    { $$ = addi(TOK_IF_EXPLICIT_ELSE, 3, $3, $5, $7); }
        | T_IF '(' expr ')' stmt %prec T_IFX    { $$ = addi(T_IF, 2, $3, $5); }


        | T_IF '(' T_UNDEFINED '(' T_SYMBOL ')' ')' stmt %prec T_IFX {
                                                  $$ = addi(T_IFNDEF, 2, $5, $8); }
        | T_IF '(' T_UNDEFINED '(' T_SYMBOL ')' ')' stmt T_ELSE stmt  {
                                                  $$ = addi(T_IFNDEF, 3, $5, $8, $10); }
        ;

loops:    T_LOOP                                { $$ = $1; }
        | T_WHILE '(' expr ')' stmt             { $$ = addi(T_WHILE, 2, $3, $5); }
        ;

expr:     T_STRING                              { $$ = $1; }
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
        | expr '%' expr                         { $$ = addi('%', 2, $1, $3); }
        | '(' expr ')'                          { $$ = $2; }
        ;

%%
