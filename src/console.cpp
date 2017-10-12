//
// Created by mick on 11.10.17.
//

#include <unordered_map>
#include <iostream>
#include "var.h"
#include "build_date.h"

#include "console.h"

namespace var {
    using namespace std;
    void list_vars();
    void stream(string k);
}

extern char *__file__;

namespace console {
    using namespace std;

    template void echo<long>(long v);
    template void echo<double>(double v);
    //template void echo<char*>(char* v);
    template <typename T> void echo(T v) {
        if (__file__) {
            return;
        }

        cout << v << endl;
    }

    void echo(char* k) {
        if (__file__) {
            return;
        }

        var::stream(k);
        cout << endl;
    }

    void list() {
        if (__file__) {
            return;
        }

        var::list_vars();
    }

    void help() {
        if (__file__) {
            return;
        }

        cout << "============================== FLUENT HELP ====================================" << endl;
        cout << "Variables:     integer, floating point and string variables can be used." << endl;
        cout << "               valid names start with a letter or underscore (_)." << endl;
        cout << "Arithmetic:    + / - + ^ operations are supported with numeric types." << endl;
        cout << "Examples:      You can also check out the samples folder." << endl;
        cout << "               Press ctrl+d if you want to leave fluent." << endl;
        cout << ":: commands:" << endl;
        cout << "               ::q/::quit - exit Fluent." << endl;
        cout << "               ::h/::help - help." << endl;
        cout << "               ::l/::list - list all defined variables." << endl;
        cout << "===============================================================================" << endl;
    }

    void quit() {
        if (__file__) {
            return;
        }

        exit(0);
    }

    void copyright() {
        cout << "Fluent v0.0.1, (c) 2017- Michael Alderson-Bythell" << endl;
        cout << "built: " << build_date << endl;
        cout << "type ::help if you need it" << endl;
    }
  
}
