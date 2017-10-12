#include <iostream>
#include "console.h"

extern FILE *yyin;
extern int yyparse (void);

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

        console::copyright();
    }

    do {
        yyparse();
    } while (!feof(yyin));

    cout << endl;
}
