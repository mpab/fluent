//
// Created by mick on 03.10.17.
//

#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <stack>
#include <unordered_map>
#include <iostream>
#include "logger.h"

#include "var.h"

namespace var {
    using namespace std;

    struct IVar {
        const type_index type;
        IVar(type_index type) : type(type) {}
        virtual ~IVar(){}
        virtual void stream(ostream& o) = 0;
        virtual void info(ostream& o) = 0;
    };

    template <typename T> struct Var : public IVar {
        
        Var(T v) : IVar(typeid(T)), v(v) {}
        void stream(ostream& o) { o << v; }
        void info(ostream& o) { o << v; }

        T value() { return v; }
    protected:
        T v;
    };

    unordered_map<string, IVar*> var_map;

    IVar* get_var(string k) {
        auto v = var_map.find(k);
        return v != var_map.end() ? v->second : nullptr;
    }

    void add_var(string k, IVar *pv) {
        delete get_var(k);
        var_map[k] = pv;
    }

    void warn_undefined(string n) {
        string s = "symbol " + n + " is not defined";
        logger::warn(s.c_str());
    }

    void stream(string k) {
        auto var = get_var(k);
        if (!var) {
            warn_undefined(k);
            return;
        }

        var->stream(cout);
    }

    void copy(string k1, string k2) {
        auto var = get_var(k2);
        if (!var) {
            warn_undefined(k2);
            return;
        }

        add_var(k1, var);
    }

    template void create<long>(string k, long v);
    template void create<double>(string k, double v);
    template void create<char*>(string k, char* v);
    template <typename T> void create(string k, T v) {
        add_var(k, new Var<T>(v));
    }

    template long value<long>(string k, ostream& err);
    template double value<double>(string k, ostream& err);
    template string value<string>(string k, ostream& err);
    template <typename T> T value(string k, ostream& err) {

        auto pivar = get_var(k);
        if (!pivar) {
            err << "symbol " << k << " is not defined";
            return T();
        }

        Var<T>* pvar = dynamic_cast< Var<T> * >(pivar);
        if (!pvar) {
            err << "symbol " << k << " is not of type: " << typeid(T).name() << endl;
            return T();
        }
        
        return pvar->value();
    }

    struct Sequence : public IVar {
        Sequence(long start, long end) : IVar(typeid(Sequence)), start(start), end(end), v(start) {}
        Sequence() : IVar(typeid(Sequence)), start(0), end(0) {}
        void stream(ostream& o) { o << value(); }
        void info(ostream& o) { o << v << "[" << start << ",...," << end << "]"; }

        long value() {
            auto r = v;

            if (v != end ) {
                ++v;
            }

            return r;
        }

    protected:
        const long start;
        const long end;
        long v;
    };

    stack<Sequence*> seq_stack;

    void push_seq(long start, long end) {
        seq_stack.push(new Sequence(start, end));
    }

    void create_seq(string k) {
        auto var = seq_stack.top();
        seq_stack.pop();
        add_var(k, var);
    }
}
