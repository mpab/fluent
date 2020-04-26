#ifndef FLUENT_CONSOLE_H_INCLUDED
#define FLUENT_CONSOLE_H_INCLUDED

//
// mpab 11.10.17.
//

#include "node.hpp"

namespace console {
    bool repl();
    void echo(node::Node* p);
    void help();
    void quit();
    void copyright();
}

#endif //FLUENT_CONSOLE_H_INCLUDED
