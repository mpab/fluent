#include <iostream>
#include "console.h"
#include "context.h"
#include "logger.h"

extern FILE *yyin;
extern int yyparse (void);

int __column__ = 1;
extern char *_src_filename;

using namespace std;

int main (int argc, char **argv) {

    ++argv, --argc;  /* skip over program name */
    if (argc > 0) {
        _src_filename = argv[0];
#ifdef _MSC_VER
        auto err = fopen_s(&yyin, _src_filename, "r");
#       else
        yyin = fopen(_src_filename, "r" );
#       endif

        if (!yyin) {
            logger::error() << "source file: " << _src_filename << " not found." << endl;
        }

    } else {
        yyin = stdin;
        console::copyright();
    }

    do {
        yyparse();
    } while (!feof(yyin));

    cout << endl;
}
