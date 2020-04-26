
#ifndef __CMDLEXEME_HPP__
#define __CMDLEXEME_HPP__ 1

#include "parser.hh"
#include <string>

namespace CMD {

struct lexeme {
  lexeme(Parser::token_type token) : type(TOKEN), token(token) {}

  lexeme(std::string text) : type(TEXT), text(text) {}

  enum { TOKEN, TEXT } type;

  CMD::Parser::token_type token;
  std::string text;
};

} // namespace CMD

#endif /* END __CMDLEXEME_HPP__ */
