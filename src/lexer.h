#include "logger.h"

#define RET(val) \
{ std::string txt = #val; if (txt.compare("*yytext")) logger::debug() << FMT16 << #val << std::endl; else logger::debug() << FMT16 << val << std::endl; } \
return val

#define SET(TOK, TYPE) \
logger::debug() << FMT16 << #TOK << yytext << std::endl; \
yylval.node = node::create_ ## TYPE (yytext); \
return TOK
