%option noyywrap
%option header-file="lexer.hh"
%option nounput

%{
#include <stdlib.h>
#include "node.hpp"
#include "parser.tab.hh"
#include "logger.hpp"
#include "lexer.hpp"

extern int __line__;

%}

E_DIGIT             [0-9]

E_INTEGER           {E_DIGIT}+
E_REAL_1            {E_DIGIT}+"f"
E_REAL_2            {E_DIGIT}+\.?{E_DIGIT}+"f"?
E_BAD_REAL_1        {E_DIGIT}+\.
E_BAD_REAL_2        \.{E_DIGIT}+
E_QSTRING           (\"(\\.|[^"])*\"|\'(\\.|[^'])*\')

E_SYMBOL            [_a-zA-Z][_a-zA-Z0-9]*
E_BAD_SYMBOL        [0-9][_a-zA-Z][_a-zA-Z0-9]

%%

">="            RET(T_GE);
"<="            RET(T_LE);
"=="            RET(T_EQ);
"!="            RET(T_NE);
"while"         RET(T_WHILE);
"if"            RET(T_COND);
"else"          RET(T_ELSE);
"outl"          RET(T_OUTL);
"out"           RET(T_OUT);

"::help\n"    {++__line__; return T_CONSOLE_HELP;}
"::h\n"       {++__line__; return T_CONSOLE_HELP;}
"::quit\n"    {++__line__; return T_CONSOLE_QUIT;}
"::q\n"       {++__line__; return T_CONSOLE_QUIT;}
"::inspect\n" {++__line__; return T_CONSOLE_INSPECT;}
"::i\n"       {++__line__; return T_CONSOLE_INSPECT;}
"::clean\n"   {++__line__; return T_CONSOLE_CLEAN;}
"::c\n"       {++__line__; return T_CONSOLE_CLEAN;}
"::free\n"    {++__line__; return T_CONSOLE_FREE;}
"::f\n"       {++__line__; return T_CONSOLE_FREE;}
"::bon\n"     {++__line__; return T_CONSOLE_EXEC_BUFFERED;}
"::boff\n"    {++__line__; return T_CONSOLE_EXEC_IMMEDIATE;}
"::run\n"     {++__line__; return T_CONSOLE_EXECUTE;}
"::r\n"       {++__line__; return T_CONSOLE_EXECUTE;}
"::lon\n"     {++__line__; return T_CONSOLE_LOGGING_ON;}
"::loff\n"    {++__line__; return T_CONSOLE_LOGGING_OFF;}
"::".*\n      {logger::warn("unrecognised command"); ++__line__;}

[ \t] ; /* skip whitespace */ 

\n {
    ++__line__;
}

"//".*\n {
    logger::debug("COMMENT");
    ++__line__;
}

"..." {
    RET(T_ELLIPSIS);
}

[-()<>=+*/;{}.,^] { // operators
    RET(*yytext);
}

{E_SYMBOL} { 
    SET(T_SYMBOL, symbol);
}

{E_INTEGER} {
    SET(T_NUMERIC, integer);
}

{E_REAL_1} {
    SET(T_NUMERIC, real);
}

{E_REAL_2} {
    SET(T_NUMERIC, real);
}

{E_QSTRING} {
    SET(T_STRING, quoted_string);
}
       
{E_BAD_REAL_1} {
    logger::error("invalid floating point formatting - use: 123f or 1.234 or 0.123");
}

{E_BAD_REAL_2} {
    logger::error("invalid floating point formatting - use: 123f or 1.234 or 0.123");
}

{E_BAD_SYMBOL} {
    logger::error("badly formed identifier");
}

. { /* anything else is an error */
    logger::error("invalid input");
}

%%

