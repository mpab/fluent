%{

/* This should suppress the macro redefinition warnings in stdint.h... but it doesn't */
#define YY_NO_UNISTD_H

/* Implementation of yyFlexScanner */ 
#include "scanner.hpp"

#undef  YY_DECL
#define YY_DECL int CMD::Scanner::yylex( CMD::Parser::semantic_type * const lval, CMD::Parser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using tok = CMD::Parser::token_type;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( tok::END )

/* update location on matching */
#define YY_USEr_aCTION loc->step(); loc->columns(yyleng);

%}

%option debug
%option nodefault
%option yyclass="CMD::Scanner"
%option noyywrap
%option c++

DIGIT             [0-9]
INTEGER           {DIGIT}+
REAL_1            {DIGIT}+"f"
REAL_2            {DIGIT}+\.?{DIGIT}+"f"?
BAD_REAL_1        {DIGIT}+\.
BAD_REAL_2        \.{DIGIT}+
QSTRING           (\"(\\.|[^"])*\"|\'(\\.|[^'])*\')

SYMBOL            [_a-zA-Z][_a-zA-Z0-9]*
BAD_SYMBOL        [0-9][_a-zA-Z][_a-zA-Z0-9]

%%

%{          /** Code executed at the beginning of yylex **/
            yylval = lval;
%}

"while"     return tok::WHILE;
"case"      return tok::CASE;
"out"       return tok::OUT;

[a-zA-Z]+   {
               /**
                * Section 10.1.5.1 of the 3.0.2 Bison Manual says the 
                * following should work:
                * yylval.build( yytext ;
                * but it doesn't.
                * ref: http://goo.gl/KLn0w2
                */
               yylval->build<std::string>(yytext);
               return tok::TEXT;
            }

[-()<>=+*/;{}.,^]   {
                        yylval->build<std::string>(yytext);
                        return tok::OPERATOR;
                    }

{SYMBOL}            { 
                        yylval->build<std::string>(yytext);
                        return tok::SYMBOL;
                    }

{BAD_SYMBOL}        { 
                        yylval->build<std::string>(yytext);
                        return tok::BAD_SYMBOL;
                    }

{INTEGER}           { 
                        yylval->build<std::string>(yytext);
                        return tok::INTEGER;
                    }

{REAL_1}            { 
                        yylval->build<std::string>(yytext);
                        return tok::REAL;
                    }

{REAL_2}            { 
                        yylval->build<std::string>(yytext);
                        return tok::REAL;
                    }

{BAD_REAL_1}        { 
                        yylval->build<std::string>(yytext);
                        return tok::BAD_REAL;
                    }

{BAD_REAL_2}        { 
                        yylval->build<std::string>(yytext);
                        return tok::BAD_REAL;
                    }

{QSTRING}           { 
                        yylval->build<std::string>(yytext);
                        return tok::QSTRING;
                    }



"::abort\n" return tok::ABORT;
"::quit\n"  return tok::ABORT;
"::q\n"     return tok::ABORT;
"::exit\n"  return tok::ABORT;

">="        return tok::GEQ;
"<="        return tok::LEQ;
"=="        return tok::ISEQ;
"!="        return tok::NEQ;
          
"\/\/".*\n  return tok::COMMENT;

\n          return tok::NL;
[[:space:]] return tok::SPC;
.           return tok::INVALID;

%%
