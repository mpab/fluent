%option noyywrap

%{
#include <stdlib.h>
#include "y.tab.h"

#include "logging.h"

extern void yyerror(const char *);

extern int __line__;

%}

DIGIT       [0-9]
ALPHA       [a-zA-z]
ALPHANUM    [a-zA-z0-9]
IDSTART     [_a-zA-z]

DQSTRING    \"(\\.|[^"])*\"
SQSTRING    \'(\\.|[^'])*\'

INT         {DIGIT}+
REAL_1      {DIGIT}+"f"
REAL_2      {DIGIT}+\.?{DIGIT}+"f"?
BAD_REAL_1  {DIGIT}+\.
BAD_REAL_2  \.{DIGIT}+
QSTRING     (\"(\\.|[^"])*\"|\'(\\.|[^'])*\')

ID          [_a-zA-Z][_a-zA-Z0-9]*
BADID       [0-9][_a-zA-Z][_a-zA-Z0-9]

%%

[ \t] ; /* skip whitespace */ 

\n {
    DP("T_ENDL:\n");
    ++__line__;
    return T_ENDL;
}

"//".*\n {
    DP("COMMENT\n");
    ++__line__;
}

(out) {
    DP("OUT ");
    RET(T_OUT);
}

(outl) {
    DP("OUTL ");
    RET(T_OUTL);
}

"..." {
    DP("T_ELLIPSIS ");
    RET(T_ELLIPSIS);
}

[-+=^*/] { // operators
    DP("ARITH_OP: %s ", yytext);
    RET(*yytext);
}

[\(\)\{\}\[\]] { // operators
    DP("BRACE: %s ", yytext);
    RET(*yytext);
}

"," {
    DP("COMMA ");
    RET(*yytext);
}

{ID} { // variable identifiers
    strcpy(yylval.s, yytext);    
    DP("ID: %s ", yylval.s);
    RET(T_ID);
}

{INT} { // integers
    yylval.n = atoi(yytext);
    DP("INT: %ld ", yylval.n);
    RET(T_INT);
}

{REAL_1} {
    yylval.r = atof(yytext);
    DP("REAL_1: %f ", yylval.r);
    RET(T_REAL);
}

{REAL_2} {
    yylval.r = atof(yytext);
    DP("REAL_2: %f ", yylval.r);
    RET(T_REAL);
}

{QSTRING} { // string
    //strcpy(yylval.s, yytext);

    memcpy(yylval.s, yytext + 1, yyleng - 2); // copy all but quotes
    yylval.s[yyleng - 2] = 0;                 // terminator

    DP("QSTRING: %s ", yylval.s);
    RET(T_STRING);
}
       
"::help"    {return CONSOLE_HELP;}
"::h"       {return CONSOLE_HELP;}
"::quit"    {return CONSOLE_QUIT;}
"::q"       {return CONSOLE_QUIT;}
"::list"    {return CONSOLE_LIST;}
"::l"       {return CONSOLE_LIST;}

{BAD_REAL_1} {
    DP("BAD_REAL_1 ");
    yyerror("invalid floating point formatting - expected: 0-9f, 0-9.0-9, 0-9.0-9f");
}

{BAD_REAL_2} {
    DP("BAD_REAL_2 ");
    yyerror("invalid floating point formatting - expected: 0-9f, 0-9.0-9, 0-9.0-9f");
}

{BADID} {
    yyerror("invalid identifier");
}

. { /* anything else is an error */
    //yyerror("ERR");
    yyerror("invalid syntax");
}

%%

