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

        logger::info() << n << endl;
    }

/*
=======================================================================================================================
*/

    void help() {
        if (!repl()) {
            return;
        }

        cout << "============================== FLUENT HELP ====================================" << endl;
        cout << "Variables:     integer, floating point and string variables can be used." << endl;
        cout << "               valid names start with a letter or underscore (_)." << endl;
        cout << "Arithmetic:    + / - + ^ operations are supported with numeric types." << endl;
        cout << "Examples:      You can also check out the samples folder." << endl;
        cout << "               Press ctrl+d if you want to leave fluent." << endl;
        cout << ":: commands:" << endl;
        cout << "               ::q/::quit      - exit Fluent." << endl;
        cout << "               ::h/::help      - help." << endl;
        cout << "               ::i/::inspect   - inspect environment." << endl;
        cout << "               ::f/::free      - free unused environment data (use in buffered mode)." << endl;
        cout << "               ::c/::clean     - clean all environment data (including symbols)." << endl;
        cout << "               ::bon           - buffered mode on (default is off)." << endl;
        cout << "               ::boff          - buffered mode off." << endl;
        cout << "               ::r/::run       - run program (only if buffered mode on)." << endl;
        cout << "               ::lon           - debug logging on (default is off)." << endl;
        cout << "               ::loff          - debug logging off." << endl;
        cout << "===============================================================================" << endl;
    }

    void quit() {
        if (!repl()) {
            return;
        }

        exit(0);
    }

    void copyright() {
        cout << "Fluent v0.0.3, (c) 2017- Michael Alderson-Bythell" << endl;
        cout << "built: " << build_date << endl;
        cout << "type ::help if you need it" << endl;
    }
  
}
