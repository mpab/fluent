#ifndef __CMDSCANNER_HPP__
#define __CMDSCANNER_HPP__ 1

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "location.hh"
#include "parser.hh"

namespace CMD {

class Scanner : public yyFlexLexer {
public:
  Scanner(std::istream *in) : yyFlexLexer(in){};
  virtual ~Scanner(){};

  // get rid of override virtual function warning
  using FlexLexer::yylex;

  virtual int yylex(CMD::Parser::semantic_type *const lval,
                    CMD::Parser::location_type *location);
  // YY_DECL defined in mc_lexer.l
  // Method body created by flex in mc_lexer.yy.cc

private:
  /* yyval ptr */
  CMD::Parser::semantic_type *yylval = nullptr;
};

} /* end namespace CMD */

#endif /* END __CMDSCANNER_HPP__ */
