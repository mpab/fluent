%option noyywrap
%option nounistd
%option header-file="lexer.hh"
%option outfile="lexer.cc"
%option yylineno

%{
#include "lexer_macros.hpp"
%}

alpha               [a-zA-Z]
num                 [0-9]
alphanum            {alpha}|{num}

SYMBOL              [_]|alphanum*
BAD_SYMBOL          [0-9]SYMBOL

INT                 {num}+

REAL                {REAL_1}|{REAL_2}
REAL_1              {num}+"f"
REAL_2              {num}+\.?{num}+"f"?

BAD_REAL            {BAD_REAL_1}|{BAD_REAL_2}
BAD_REAL_1          {num}+\.
BAD_REAL_2          \.{num}+

QSTRING             (\"(\\.|[^"])*\"|\'(\\.|[^'])*\')

OPERATOR            [-()<>=+*/;{}.,^]

%%

"\/\/".*\n          SCAN_COMMENT();

"while"             SCAN_KEYWORD();
"case"              SCAN_KEYWORD();
"out"               SCAN_KEYWORD();

{OPERATOR}          SCAN_OPERATOR();

{SYMBOL}            SCAN_SYMBOL();

{BAD_SYMBOL}        SCAN_ERROR();

{INT}               SCAN_INT();

{REAL}              SCAN_REAL();

{BAD_REAL}          SCAN_ERROR();

{QSTRING}           SCAN_QSTRING();

"::abort\n"         SCAN_MODE(ABORT);
"::quit\n"          SCAN_MODE(ABORT);
"::q\n"             SCAN_MODE(ABORT);
"::exit\n"          SCAN_MODE(ABORT);

">="                SCAN_OPERATOR();
"<="                SCAN_OPERATOR();
"=="                SCAN_OPERATOR();
"!="                SCAN_OPERATOR();
          
\n                  SCAN_NEWLINE();
[[:space:]]         SCAN_IGNORE();
.                   SCAN_UNHANDLED();

%%
