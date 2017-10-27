//
// Created by mick on 11.10.17.
//

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include "context.h"
#include "build_date.h"
#include "logger.h"

#include "console.h"

/*
=======================================================================================================================
*/
#define IOUT logger::info()
extern char *_src_filename;
/*
=======================================================================================================================
*/

namespace console {
    using namespace std;

    bool repl() {
        return _src_filename == nullptr;
    }

    void echo(node::Node* n) {
        if (!repl())
            return;

        IOUT << n << endl;
    }

/*
=======================================================================================================================
*/



    void help() {
        if (!repl()) {
            return;
        }

        IOUT << "============================== FLUENT HELP ====================================" << endl;
        IOUT << "Variables:     integer, floating point and string variables can be used." << endl;
        IOUT << "               valid names start with a letter or underscore (_)." << endl;
        IOUT << "Arithmetic:    + / - + ^ operations are supported with numeric types." << endl;
        IOUT << "Examples:      You can also check IOUT the samples folder." << endl;
        IOUT << "               Press ctrl+d if you want to leave fluent." << endl;
        IOUT << ":: commands:" << endl;
        IOUT << "               ::q/::quit      - exit Fluent." << endl;
        IOUT << "               ::h/::help      - help." << endl;
        IOUT << "               ::i/::inspect   - inspect environment." << endl;
        IOUT << "               ::f/::free      - free unused environment data (use in buffered mode)." << endl;
        IOUT << "               ::c/::clean     - clean all environment data (including symbols)." << endl;
        IOUT << "               ::bon           - buffered mode on (default is off)." << endl;
        IOUT << "               ::boff          - buffered mode off." << endl;
        IOUT << "               ::r/::run       - run program (only if buffered mode on)." << endl;
        IOUT << "               ::lon           - verbose logging on (default is off)." << endl;
        IOUT << "               ::loff          - verbose logging off." << endl;
        IOUT << "===============================================================================" << endl;
    }

    void quit() {
        if (!repl()) {
            return;
        }

        exit(0);
    }

    void copyright() {
        IOUT << "Fluent v0.0.3, (c) 2017- Michael Alderson-Bythell" << endl;
        IOUT << "built: " << build_date << endl;
        IOUT << "type ::help if you need it" << endl;
    }
  
}
