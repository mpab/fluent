//
// Created by mick on 11.10.17.
//

#ifndef FLUENT_CONSOLE_H
#define FLUENT_CONSOLE_H

#include "node.h"

namespace console {
    using namespace node;

    bool repl();

    void echo(Node* p);
    void help();
    void quit();
    void copyright();
}

#endif //FLUENT_CONSOLE_H
