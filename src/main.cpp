#include <iostream>
#include "console.h"
#include "context.h"

extern FILE *yyin;
extern int yyparse (void);

int __column__ = 1;
extern char *_src_filename;

using namespace std;

int main (int argc, char **argv) {

    ++argv, --argc;  /* skip over program name */
    if (argc > 0) {
        _src_filename = argv[0];
        yyin = fopen(_src_filename, "r" );
    } else {
        yyin = stdin;
        console::copyright();
    }

    do {
        yyparse();
    } while (!feof(yyin));

    cout << endl;
}
