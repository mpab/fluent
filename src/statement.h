//
// Created by mick on 11.10.17.
//

#ifndef FLUENT_STATEMENT_H
#define FLUENT_STATEMENT_H

#include <string>

namespace statement {  
    template <typename T> T negate(T n);

    template <typename T> T add(T n, T m);
    template <typename T> T subtract(T a, T b);
    template <typename T> T multiply(T n, T m);
    template <typename T> T divide(T n, T m);
    template <typename T> T power(T n, T m);

    template <typename T> void out(T v); 
    template <typename T> void outl(T v);
    void outl();

    void outv(std::string k);
    void outvl(std::string k);

    void execute();
}

#endif //FLUENT_STATEMENT_H
