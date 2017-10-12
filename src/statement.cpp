//
// Created by mick on 11.10.17.
//

#include <math.h>
#include <string>
#include <iostream>

#include "statement.h"

namespace var {
    void stream(std::string k);
}

namespace statement {

    using namespace std;

    template long negate<long>(long n);
    template double negate<double>(double n);
    template <typename T> T negate(T n) {
        return -n;
    }

    template long add(long n, long m);
    template double add(double n, double m);
    template <typename T> T add(T n, T m) {
        return n + m;
    }

    template long subtract(long n, long m);
    template double subtract(double n, double m);
    template <typename T> T subtract(T n, T m) {
        return n - m;
    }

    template long multiply(long n, long m);
    template double multiply(double n, double m);
    template <typename T> T multiply(T n, T m) {
        return n * m;
    }

    template long divide(long n, long m);
    template double divide(double n, double m);
    template <typename T> T divide(T n, T m) {
        return n / m;
    }

    template long power(long n, long m);
    template double power(double n, double m);
    template <typename T> T power(T n, T m) {
        return pow(n, m);
    }

    template void out(long);
    template void out(double);
    template void out(char*);
    template <typename T> void out(T v) {
        cout << v;
    }

    template void outl(long);
    template void outl(double);
    template void outl(char*);
    template <typename T> void outl(T v) {
        out(v);
        cout << endl;
    }

    void outl() {
        cout << endl;
    }

    void outv(string k) {
        var::stream(k);
    }

    void outvl(string k) {
        var::stream(k);
        cout << endl;
    }

    void execute() {
    }
}
