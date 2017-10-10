%{
#include <iostream>
#include <math.h>
using namespace std;

#include "logging.h"
#include "var.h"

int yylex(void);
void yyerror(const char *);
void man();
extern char *__file__;


%}
%define parse.error verbose

%union
{
    long i;
    float f;
    char s[100];
}

%token T_INT T_FLOAT T_ID T_STRING
%type<i> expi T_INT
%type<f> expf T_FLOAT
%type<s> T_ID
%type<s> T_STRING

%token T_ENDL T_EOF
%token T_ELLIPSIS
%token T_OUT T_OUTL
%token T_EXIT T_HELP T_LIST

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
    //| seq T_ENDL {
    //}
    | T_EXIT T_ENDL {
        exit(0);
    }
    | T_HELP T_ENDL {
        man();
    }
    | T_LIST T_ENDL {
        var::list_vars();
    }
;

stmt:
    expi {
        if (!__file__) {
        cout << $1 << endl;
        }
    }
    | expf {
        if (!__file__) {
            cout << $1 << endl;
        }
    }
    | T_ID {
        if (!__file__) {
            var::outl($1);
        }
    }
    | T_ID '=' T_ID {
        var::assign($1, $3);
    }
    | T_ID '=' expi {
        var::create<long>($1, $3);
    }
    | T_ID '=' expf {
        var::create<double>($1, $3);
    }
    | T_ID '=' T_STRING {
        var::create<string>($1, $3);
    }
    | T_ID '=' seq {
        //cout << "::" << $1 << endl;
        var::create_seq($1);
    }
    //| '(' T_ID '=' seq ')' '{' stmt '}' {
    //    cout << ":=" << sym[$2] << endl;
    //    cout << "::=" << ('a' + $1) << endl;
    //}
    | T_OUTL {
        cout << endl;
    }
    | T_OUT expi {
        cout << $2;
    }
    | T_OUTL expi {
        cout << $2 << endl;
    }
    | T_OUT expf {
        cout << $2;
    }
    | T_OUTL expf {
        cout << $2 << endl;
    }
    | T_OUT T_STRING {
        cout << $2;
    }
    | T_OUTL T_STRING {
        cout << $2 << endl;
    }
    | T_OUT T_ID {
        var::out($2);
    }
    | T_OUTL T_ID {
        var::outl($2);
    }
;

expi:
    T_INT { $$ = $1; }
    | T_ID {
        $$ = var::value<long>($1, cout);
    }
    | expi '+' expi { $$ = $1 + $3; }
    | expi '-' expi { $$ = $1 - $3; }
    | expi '*' expi { $$ = $1 * $3; }
    | expi '/' expi { $$ = $1 / $3; }
    | '-' expi %prec NEG { $$=-$2; }
    | expi '^' expi { $$ = pow($1, $3); }
    | '(' expi ')' { $$ = $2; }
;

expf:
    T_FLOAT { $$ = $1; }
    | T_ID {
        $$ = var::value<double>($1, cout);
    }
    | expf '+' expf { $$ = $1 + $3; }
    | expf '-' expf { $$ = $1 - $3; }
    | expf '*' expf { $$ = $1 * $3; }
    | expf '/' expf { $$ = $1 / $3; }
    | '-' expf %prec NEG { $$=-$2; }
    | expf '^' expf { $$ = pow($1, $3); }
    | '(' expf ')' { $$ = $2; }
;

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
