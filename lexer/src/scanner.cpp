#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "Scanner.hpp"
#include <exception>
#include <cassert>
#include <cctype>
#include <fstream>

extern FILE* yyin;
extern char* _src_filename;
extern int yylex();

namespace lexer {

    Scanner::Scanner()
    {
        yyin = stdin;
    };

    Scanner::Scanner(std::string filename)
    {
#ifdef _MSC_VER
        auto err = fopen_s(&yyin, filename.c_str(), "r");
#       else
        yyin = fopen(filename, "r");
#       endif

        if (!yyin) {
            throw "bad file";
        }
    };

    void Scanner::TokenizeUntilEof() {
        do {
            Tokenize();
        } while (!feof(yyin));
    }

    void Scanner::Tokenize()
    {
        yylex();
    }
}
