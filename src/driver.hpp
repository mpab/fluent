#ifndef __CMDDRIVER_HPP__
#define __CMDDRIVER_HPP__ 1

#include <cstddef>
#include <istream>
#include <list>
#include <string>

#include "lexeme.hpp"
#include "parser.hh"
#include "scanner.hpp"

namespace CMD {

class Driver {
public:
  enum Mode { REPL, STREAM };
  Driver(Mode mode = Mode::STREAM);

  virtual ~Driver();

  /**
   * parse - parse from a file
   * @param filename - valid string with input file
   */
  void parse(const char *const filename);
  /**
   * parse - parse from a c++ input stream
   * @param is - std::istream&, valid input stream
   */
  void parse(std::istream &iss);

  std::ostream &eval(std::ostream &stream);

  void text(const std::string &word);
  void token(CMD::Parser::token_type t);

  void chr();
  void newline();
  void space();
  void abort();
  void xpr(void*);

  std::string evaluate();

private:
  void prompt();

  Mode mode;

  std::list<CMD::lexeme> lexemes;

  void parse_helper(std::istream &stream);

  CMD::Parser *parser = nullptr;
  CMD::Scanner *scanner = nullptr;
};

} /* end namespace CMD */
#endif /* END __CMDDRIVER_HPP__ */
