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

#define yytextspc yytext << std::endl;

#define SCAN_INT()				{ auto val = std::atoi(yytext); std::cout << "integer " << val << " "; }
#define SCAN_REAL()				{ auto val = std::atof(yytext); std::cout << "real " << val << " "; }
#define SCAN_QSTRING()			{ auto val = std::string(yytext, yyleng); std::cout << "string " << val << " "; }

#define SCAN_SYMBOL()			{ std::cout << "symbol " << yytextspc; }
#define SCAN_KEYWORD()			{ std::cout << "keyword " << yytextspc; }
#define SCAN_MODE_ABORT()		{ yyterminate(); }
#define SCAN_OPERATOR()			{ std::cout << "operator " << yytextspc; }
#define SCAN_COMMENT()			{ std::cout << "comment " << yytextspc; }

#define SCAN_ERROR()			{ std::cout << "error " << yytextspc; }
#define SCAN_UNHANDLED()        { std::cout << "unhandled" << yytextspc; }

#define SCAN_IGNORE()			{} /*{ std::cout << "."; }*/

#define SCAN_NEWLINE()			{} /*{ std::cout << "NL" << std::endl; }*/

#endif //__LEXER_DEPENDENCIES_HPP__INCLUDED__