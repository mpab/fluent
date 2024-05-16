// copyright (c) 2017- mpab

#include "logger.hpp"

#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <sstream>

#include "console.hpp"
#include "context.hpp"

using namespace std;

int __line__ = 1;
char* _src_filename = nullptr;

#ifndef DEBUG_BUILD
bool disable_logging = true;
#else
bool disable_logging = false;
#endif

std::stringstream nullstream;

#ifdef _MSC_VER

// https://developercommunity.visualstudio.com/t/error-c2872-byte-ambiguous-symbol/93889
#define byte BYTE
#include <windows.h>

struct WinConsole {
  const HANDLE hout;
  WORD orig_col;

  WinConsole() : hout(GetStdHandle(STD_OUTPUT_HANDLE)) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hout, &csbi);
    orig_col = csbi.wAttributes;

    // cout << "saved orig_col " << orig_col << endl;

    // for (auto i = 0; i != 256; ++i) {
    //     SetConsoleTextAttribute(hout, i);
    //     cout << "i = " << i << ", col = " << i << endl;
    //     reset_col();
    // }
  }

  ~WinConsole() { reset_col(); }

  void set_fg_col(WORD c) {
    c &= 0x0F;
    c += orig_col & 0xF0;
    SetConsoleTextAttribute(hout, c);
    // cout << "set_fg_col() = " << c << endl;
  }

  void reset_col() {
    SetConsoleTextAttribute(hout, orig_col);
    // cout << "reset_col()" << endl;
  }
};

WinConsole win_console;

ostream& operator<<(ostream& o, const console_fg_col& fgc) {
  switch (fgc.c) {
    case console_fg_col::RED:
      win_console.set_fg_col(4);
      break;
    case console_fg_col::GREEN:
      win_console.set_fg_col(10);
      break;
    case console_fg_col::YELLOW:
      win_console.set_fg_col(14);
      break;
    case console_fg_col::BLUE:
      win_console.set_fg_col(9);
      break;
    case console_fg_col::MAGENTA:
      win_console.set_fg_col(13);
      break;
    case console_fg_col::CYAN:
      win_console.set_fg_col(11);
      break;
    default:
      win_console.reset_col();
      break;
  }
  return o;
}

#else

ostream& operator<<(ostream& o, const console_fg_col& fgc) {
  switch (fgc.c) {
    case console_fg_col::RED:
      o << "\x1b[31m";
      break;
    case console_fg_col::GREEN:
      o << "\x1b[32m";
      break;
    case console_fg_col::YELLOW:
      o << "\x1b[33m";
      break;
    case console_fg_col::BLUE:
      o << "\x1b[34m";
      break;
    case console_fg_col::MAGENTA:
      o << "\x1b[35m";
      break;
    case console_fg_col::CYAN:
      o << "\x1b[36m";
      break;
    default:
      o << "\x1b[0m";
      break;
  }
  return o;
}

#endif

class ResetColorStream : public std::ostream {
  // Write a stream buffer that resets the colour when sync'(d)
  class StreamBuf : public std::stringbuf {
    std::ostream& o;

   public:
    explicit StreamBuf(std::ostream& str) : o(str) {}

    virtual int sync() {
      o << str() << ANSI_COLOR_RESET;
      str("");
      o.flush();
      return 0;
    }
  };

  StreamBuf buffer;

 public:
  explicit ResetColorStream(std::ostream& str)
      : std::ostream(&buffer), buffer(str) {}
};

ResetColorStream rcs(std::cout);

namespace logger {

void on() {
  rcs << ANSI_COLOR_MAGENTA << "verbose logging=on" << endl;
  disable_logging = false;
}

void off() {
  rcs << ANSI_COLOR_MAGENTA << "verbose logging=off" << endl;
  disable_logging = true;
}

std::ostream& debug() {
  if (disable_logging) {
    nullstream.clear();
    return nullstream;
  }
  return rcs << ANSI_COLOR_CYAN;
}

std::ostream& warn() { return rcs << ANSI_COLOR_YELLOW; }

std::ostream& error() { return rcs << ANSI_COLOR_RED; }

std::ostream& info() { return rcs << ANSI_COLOR_RESET; }

void debug(const char* s) {
  if (disable_logging || !s) return;
  debug() << s << endl;
}

void debug_if(bool b, const char* s) {
  if (disable_logging || !s || !b) return;
  debug(s);
}

void info(const char* s) {
  if (!s) return;
  info() << s << endl;
}

void parser_info(const char* s) {
  if (!s) return;
  info() << s << " (" << _src_filename << ", line: " << __line__ << ")" << endl;
}

void msg(const char* s, std::ostream& o, bool inspect_and_exit) {
  if (!s) return;

  o << s;

  if (!console::repl()) {
    o << ", in file: " << _src_filename << ", at line " << __line__;
  }

  o << endl;

  if (inspect_and_exit) {
    context::inspect();
    exit(-1);
  }
}

void warn(const char* s) {
  if (console::repl()) {
    msg(s, warn(), false);
  } else {
    // elevate to error
    error(s);
  }
}

void warn_if(bool b, const char* s) {
  if (!s || !b) return;
  warn(s);
}

void error(const char* s) { msg(s, error(), true); }

void error_if(bool b, const char* s) {
  if (!s || !b) return;
  error(s);
}
}  // namespace logger

void yyerror(const char* s) {
  if (!s) return;

  if (0 == strcmp("syntax error, unexpected $end, expecting T_ENDL", s)) {
    logger::warn("fluent source file must end with a new line");
    return;
  }

  // auto bad_token = context::current_node();

  if (0 == strcmp("syntax error, unexpected T_SYMBOL, expecting '('", s)) {
    logger::error("syntax error, missing '('");
    return;
  }

  if (0 == strcmp("syntax error, unexpected T_SYMBOL", s)) {
    logger::error("syntax error, unexpected symbol");
    return;
  }

  logger::error(s);
}
