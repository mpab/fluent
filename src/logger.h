//
// Created by mick on 11.10.17.
//

#ifndef FLUENT_LOGGING_H_INCLUDED
#define FLUENT_LOGGING_H_INCLUDED

#include <ostream>
#include <iomanip>

#define FMT8  std::left << std::setw(8)  << std::setfill(' ')
#define FMT12 std::left << std::setw(12) << std::setfill(' ')
#define FMT16 std::left << std::setw(16) << std::setfill(' ')
#define FMT24 std::left << std::setw(20) << std::setfill(' ')

struct console_fg_col {
    enum col{ RESET, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN };
    const col c;
    console_fg_col(const col c) : c(c){}
};

std::ostream& operator << (std::ostream&, const console_fg_col&);

#define ANSI_COLOR_RED     console_fg_col(console_fg_col::RED)
#define ANSI_COLOR_GREEN   console_fg_col(console_fg_col::GREEN)
#define ANSI_COLOR_YELLOW  console_fg_col(console_fg_col::YELLOW)
#define ANSI_COLOR_BLUE    console_fg_col(console_fg_col::BLUE)
#define ANSI_COLOR_MAGENTA console_fg_col(console_fg_col::MAGENTA)
#define ANSI_COLOR_CYAN    console_fg_col(console_fg_col::CYAN)
#define ANSI_COLOR_RESET   console_fg_col(console_fg_col::RESET)

void yyerror(const char *s);
const char* opcode_name(int id);

namespace logger {
    void on(); // TODO - use level filter
    void off();
    
    std::ostream& debug();
    void debug(const char *s);
    void debug_if(bool b, const char *s);

    std::ostream& info();
    void info(const char *s);
    void parser_info(const char *s);

    std::ostream& warn();
    void warn(const char *s);
    void warn_if(bool b, const char *s);
    
    std::ostream& error();
    void error(const char *s);
    void error_if(bool b, const char *s);
}

#endif //FLUENT_LOGGING_H_INCLUDED
