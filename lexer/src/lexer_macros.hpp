#ifndef __LEXER_DEPENDENCIES_HPP__INCLUDED__
#define __LEXER_DEPENDENCIES_HPP__INCLUDED__

#include <string>
#include <iostream>

extern int __line__;

//#undef  YY_DECL
//#define YY_DECL int lr::Scanner::yylex( lr::Parser::semantic_type* const lval, lr::Parser::location_type* loc )
/* typedef to make the returns for the tokens shorter */
//using tok = lr::Parser::token_type;

///* define yyterminate as this instead of NULL */
//#define yyterminate() return( tok::END )

/* update location on matching */

#define YY_STEP loc.step()
#define YY_USER_ACTION

// utility macros to simplify the actions

#define SCAN_INT()				{ auto val = std::atoi(yytext); std::cout << "integer: " << val; }
#define SCAN_REAL()				{ auto val = std::atof(yytext); std::cout << "real: " << val; }
#define SCAN_QSTRING()			{ auto val = std::string(yytext, yyleng); std::cout << "string: " << val; }

#define SCAN_SYMBOL()			{ std::cout << "symbol: " << yytext; }
#define SCAN_KEYWORD()			{ std::cout << "keyword: " << yytext; }
#define SCAN_MODE(mode)			{ std::cout << "mode: " << #mode; }
#define SCAN_OPERATOR()			{ std::cout << "operator: " << yytext; }
#define SCAN_COMMENT()			{ std::cout << "comment: " << yytext; }

#define SCAN_ERROR()			{ std::cout << "error: " << yytext; }
#define SCAN_UNHANDLED()        { std::cout << "unhandled" << yytext; }

#define SCAN_IGNORE()			{ std::cout << "--" << yytext << "--"; }

#define SCAN_NEWLINE()			{ std::cout << std::endl; }

#endif //__LEXER_DEPENDENCIES_HPP__INCLUDED__