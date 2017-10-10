%option noyywrap

%{
#include <stdlib.h>
#include "y.tab.h"

#include "../src/logging.h"

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
FLOAT_1     {DIGIT}+"f"
FLOAT_2     {DIGIT}+\.?{DIGIT}+"f"?
BAD_FLOAT_1 {DIGIT}+\.
BAD_FLOAT_2 \.{DIGIT}+
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
    yylval.i = atoi(yytext);
    DP("INT: %ld ", yylval.i);
    RET(T_INT);
}

{FLOAT_1} {
    yylval.f = atof(yytext);
    DP("FLOAT_1: %f ", yylval.f);
    RET(T_FLOAT);
}

{FLOAT_2} {
    yylval.f = atof(yytext);
    DP("FLOAT_2: %f ", yylval.f);
    RET(T_FLOAT);
}

{QSTRING} { // string
    //strcpy(yylval.s, yytext);

    memcpy(yylval.s, yytext + 1, yyleng - 2); // copy all but quotes
    yylval.s[yyleng - 2] = 0;                 // terminator

    DP("QSTRING: %s ", yylval.s);
    RET(T_STRING);
}
       
"::help"    {return T_HELP;}
"::h"       {return T_HELP;}
"::quit"    {return T_EXIT;}
"::q"       {return T_EXIT;}
"::list"    {return T_LIST;}
"::l"       {return T_LIST;}

{BAD_FLOAT_1} {
    DP("BAD_FLOAT_1 ");
    yyerror("invalid floating point formatting - expected: 0-9f, 0-9.0-9, 0-9.0-9f");
}

{BAD_FLOAT_2} {
    DP("BAD_FLOAT_2 ");
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

