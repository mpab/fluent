//
// Created by mick on 12.10.17.
//

#include <iostream>
#include <string.h>

using namespace std;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int __line__ = 1;
extern char *__file__;

void yyerror(const char *s) {
    if (0 == strcmp("syntax error, unexpected $end, expecting T_ENDL", s)) {
        cout << endl << ANSI_COLOR_YELLOW << "WARNING: fluent source file must end with a new line" << ANSI_COLOR_RESET << endl;
    } else {
        cout << endl << ANSI_COLOR_RED << s << ANSI_COLOR_RESET << endl;
    }

    if (__file__) {
        cout << __file__ << ", at line " << __line__ << endl;
        exit(-1);
    }
}
