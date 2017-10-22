#ifndef FLUENT_LOGGING_H
#define FLUENT_LOGGING_H

#include <ostream>
#include <iomanip>

#define FMT8 std::left << std::setw(8) << std::setfill(' ')
#define FMT12 std::left << std::setw(12) << std::setfill(' ')
#define FMT16 std::left << std::setw(16) << std::setfill(' ')

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void yyerror(const char *s);
const char* opcode_name(int id);

namespace logger {
    void on();
    void off();
    
    std::ostream& debug();
    void debug(const char *s);
    void debug_if(bool b, const char *s);

    std::ostream& info();

    std::ostream& warn();
    void warn(const char *s);
    void warn_if(bool b, const char *s);
    
    std::ostream& error();
    void error(const char *s);
    void error_if(bool b, const char *s);
}

#endif //FLUENT_LOGGING_H
