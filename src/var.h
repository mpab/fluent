//
// Created by mick on 03.10.17.
//

#ifndef FLUENT_VAR_H
#define FLUENT_VAR_H

#include <string>

namespace var {   
    using namespace std;
    void out(string k);
    void outl(string k);
    void assign(string k1, string k2);
    template <typename T> void create(string k, T v);
    template <typename T> T value(string k, ostream& err);
    void list_vars();

    void push_seq(long start, long end);
    void create_seq(string k);
}

#endif //FLUENT_VAR_H
