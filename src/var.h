//
// Created by mick on 03.10.17.
//

#ifndef FLUENT_VAR_H
#define FLUENT_VAR_H

#include <string>
#include <ostream>

namespace var {   
    void assign(std::string k1, std::string k2);
    template <typename T> void create(std::string k, T v);
    template <typename T> T value(std::string k, std::ostream& err);

    void push_seq(long start, long end);
    void create_seq(std::string k);
}

#endif //FLUENT_VAR_H
