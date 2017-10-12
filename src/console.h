//
// Created by mick on 11.10.17.
//

#ifndef FLUENT_CONSOLE_H
#define FLUENT_CONSOLE_H

namespace console {
    template <typename T> void echo(T v);
    void echo(char* k);
    void list();
    void help();
    void quit();
    void copyright();
}

#endif //FLUENT_CONSOLE_H
