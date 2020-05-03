 #ifndef __SCANNER_HPP__INCLUDED__
 #define __SCANNER_HPP__INCLUDED__

namespace lexer {

class Scanner {

public:
    Scanner();
    Scanner(std::string filename);

    void TokenizeUntilEof();
    int Tokenize();

//   //Scanner(std::istream *in, std::ostream *out) : yyFlexLexer(in){};
//   virtual ~Scanner(){};

//   // get rid of override virtual function warning
//   using FlexLexer::yylex;

//   virtual int yylex(Parser::semantic_type *const lval,
//                     Parser::location_type *loc);
//   // YY_DECL defined in lexer.l
//   // Method body created by flex in lexer.yy.cc

// private:
//   /* yyval ptr */
//   Parser::semantic_type *lval = nullptr;
//   Parser::location_type *loc = nullptr;
};

} // namespace

 #endif /* END __SCANNER_HPP__INCLUDED__ */
