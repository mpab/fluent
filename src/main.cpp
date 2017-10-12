#include <iostream>
#include <string.h>
#include "build_date.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

extern FILE *yyin;
extern int yyparse (void);

int __line__ = 1;
int __column__ = 1;
char *__file__ = 0;

using namespace std;

int main (int argc, char **argv) {

    ++argv, --argc;  /* skip over program name */
    if (argc > 0) {
        yyin = fopen( argv[0], "r" );
        __file__ = argv[0];
    } else {
        yyin = stdin;
        __file__ = 0;
        cout << "Fluent v0.0.1, (c) 2017- Michael Alderson-Bythell" << endl;
        cout << "built: " << build_date << endl;
        cout << "type ::help if you need it" << endl;
    }

    do {
        yyparse();
    } while (!feof(yyin));

    printf("\n");
}

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

