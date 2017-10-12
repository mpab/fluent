%{
#include <iostream>
using namespace std;

#include "logging.h"
#include "var.h"
#include "statement.h"
#include "console.h"

int yylex(void);
void yyerror(const char *);

%}
%define parse.error verbose

%union
{
    long n;
    double r;
    char s[100];
}

%token T_INT T_REAL T_ID T_STRING
%type<n> exp_int T_INT
%type<r> exp_real T_REAL
%type<s> T_ID
%type<s> T_STRING

%token T_ENDL T_EOF
%token T_ELLIPSIS
%token T_OUT T_OUTL
%token CONSOLE_QUIT CONSOLE_HELP CONSOLE_LIST

%left '+' '-'
%left '*' '/'
%precedence NEG
%right '^'

%%

program:
| program line
;

line: T_ENDL
    | stmt T_ENDL {
    }
    | CONSOLE_QUIT T_ENDL {
        console::quit();
    }
    | CONSOLE_HELP T_ENDL {
        console::help();
    }
    | CONSOLE_LIST T_ENDL {
        console::list();
    }
;

stmt:
    exp_int {
        console::echo($1);
    }
    | exp_real {
        console::echo($1);
    }
    | T_ID {
        console::echo($1);
    }
    | T_ID '=' T_ID {
        var::assign($1, $3);
    }
    | T_ID '=' exp_int {
        var::create<long>($1, $3);
    }
    | T_ID '=' exp_real {
        var::create<double>($1, $3);
    }
    | T_ID '=' T_STRING {
        var::create<string>($1, $3);
    }
    | T_ID '=' seq {
        var::create_seq($1);
    }
    | T_OUTL {
        statement::outl();
    }
    | T_OUT exp_int {
        statement::out($2);
    }
    | T_OUTL exp_int {
        statement::outl($2);
    }
    | T_OUT exp_real {
        statement::out($2);
    }
    | T_OUTL exp_real {
        statement::outl($2);
    }
    | T_OUT T_STRING {
        statement::out($2);
    }
    | T_OUTL T_STRING {
        statement::outl($2);
    }
    | T_OUT T_ID {
        statement::out($2);
    }
    | T_OUTL T_ID {
        statement::outl($2);
    }
;

exp_int:
    T_INT {
        $$ = $1;
    }
    | T_ID {
        $$ = var::value<long>($1, cout);
    }
    | exp_int '+' exp_int { $$ = statement::add($1, $3); }
    | exp_int '-' exp_int { $$ = statement::subtract($1, $3); }
    | exp_int '*' exp_int { $$ = statement::multiply($1, $3); }
    | exp_int '/' exp_int { $$ = statement::divide($1, $3); }
    | '-' exp_int %prec NEG { $$ = statement::negate($2); }
    | exp_int '^' exp_int { $$ = statement::power($1, $3); }
    | '(' exp_int ')' { $$ = $2; }
;

exp_real:
    T_REAL {
        $$ = $1;
    }
    | T_ID {
        $$ = var::value<double>($1, cout);
    }
    | exp_real '+' exp_real { $$ = statement::add($1, $3); }
    | exp_real '-' exp_real { $$ = statement::subtract($1, $3); }
    | exp_real '*' exp_real { $$ = statement::multiply($1, $3); }
    | exp_real '/' exp_real { $$ = statement::divide($1, $3); }
    | '-' exp_real %prec NEG { $$ = statement::negate($2); }
    | exp_real '^' exp_real { $$ = statement::power($1, $3); }
    | '(' exp_real ')' { $$ = $2; }

//exps:
//    T_QSTRING { $$ = $1; }
//    | exps '+' exps { $$ = $1 + $3; }
//    | '(' exps ')' { $$ = $2; }
//;

seq:
    '[' T_INT ',' T_ELLIPSIS ',' T_INT ']' {
        //cout << "seq(" << $2 << " to " << $6 << ")";
        var::push_seq($2, $6);
    }
    //| '[' T_INT T_ELLIPSIS T_INT ',' T_INT ']' {
    //    cout << "seq_skip(" << $2 << " to " << $4 << " skip every " << $6 << ")";
    //}
;

%%
