//
// Created by mick on 11.10.17.
//

#ifndef FLUENT_LOGGING_H_INCLUDED
#define FLUENT_LOGGING_H_INCLUDED

#include <ostream>
#include <iomanip>

#define FMT8 std::left << std::setw(8) << std::setfill(' ')
#define FMT12 std::left << std::setw(12) << std::setfill(' ')
#define FMT16 std::left << std::setw(16) << std::setfill(' ')
#define FMT24 std::left << std::setw(20) << std::setfill(' ')

#ifndef _MSC_VER
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#else
#define ANSI_COLOR_RED     ""
#define ANSI_COLOR_GREEN   ""
#define ANSI_COLOR_YELLOW  ""
#define ANSI_COLOR_BLUE    ""
#define ANSI_COLOR_MAGENTA ""
#define ANSI_COLOR_CYAN    ""
#define ANSI_COLOR_RESET   ""
#endif

void yyerror(const char *s);
const char* opcode_name(int id);

namespace logger {
    void on();
    void off();
    
    std::ostream& debug();
    void debug(const char *s);
    void debug_if(bool b, const char *s);

    std::ostream& info();
    void info(const char *s);

    std::ostream& warn();
    void warn(const char *s);
    void warn_if(bool b, const char *s);
    
    std::ostream& error();
    void error(const char *s);
    void error_if(bool b, const char *s);
}

#endif //FLUENT_LOGGING_H_INCLUDED
