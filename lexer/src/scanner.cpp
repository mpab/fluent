#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <cassert>
#include <cctype>
#include <exception>
#include <fstream>

#include "Scanner.hpp"
#include "lexer.hh"

namespace lexer {

Scanner::Scanner() { yyin = stdin; };

Scanner::Scanner(std::string filename) {
#ifdef _MSC_VER
  auto err = fopen_s(&yyin, filename.c_str(), "r");
#else
  yyin = fopen(filename, "r");
#endif

  if (!yyin) {
    throw "bad file";
  }
};

void Scanner::TokenizeUntilEof() {
  int keep_scanning;
  do {
    keep_scanning = Tokenize();
  } while (keep_scanning && !feof(yyin));
}

int Scanner::Tokenize() { return yylex(); }
} // namespace lexer
