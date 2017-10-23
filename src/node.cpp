#include <stdlib.h>
#include <iostream>
#include <list>
#include <unordered_map>
#include <string.h>
#include "logger.h"

#include "node.h"

namespace context {
    using namespace node;
    using namespace boost;

    void track_node_create(const Node* n);
    void track_node_delete(const Node* n);
}

namespace node {
    using namespace std;

    Node::Node(const Type type) : type(type) {
        context::track_node_create(this);
    }

    Node::~Node() {
        context::track_node_delete(this);
    }

    const char* Node::type_name() const {
        return node::type_name(type);
    }

    const char* type_name(Type t) {
        const char* names[] = {
            "INSTRUCTION", "SYMBOL", "VARIABLE"
        };
        return names[t];
    }

    Symbol* create_symbol(const char* s) {
        return new Symbol(s);
    }

    Variable* create_integer(const char* s) {
        long i = atoi(s);
        return create_integer(i);
    }

    Variable* create_real(const char* s) {
        double r = atof(s);
        return create_real(r);
    }

    Variable* create_string(const char* s) {
        return new Variable(s);
    }

    Variable* create_quoted_string(char* s) {
        int len = strlen(s);
        s[len-1] = 0;
        return create_string(&s[1]);
    }

    Variable* create_integer(long i) {
        return new Variable(i);
    }

    Variable* create_real(double r) {
        return new Variable(r);
    }

/*
=======================================================================================================================
*/

    ostream& operator << (ostream& o, const Node& node) {
        return o << &node;
    }

    ostream& operator << (ostream& o, const Node* n) {

        if (!n) {
            return o << "{nullptr}";
        }

        auto v = dynamic_cast<Variable*>(const_cast<Node*>(n));
        if (v) {
            o << v->value;
        }

        auto f = dynamic_cast<Instruction*>(const_cast<Node*>(n));
        if (f) {
            o << opcode_name(f->opcode) << " ";
        }

        auto s = dynamic_cast<Symbol*>(const_cast<Node*>(n));
        if (s) {
            o << s->name;
        }

        return o;
    }

    ostream& operator << (ostream& o, const NodeInfo& n) {
        
        o << FMT16 << (void*)n.n << FMT12 << n.n->type_name() << n.n;
        
        auto i = dynamic_cast<Instruction*>(const_cast<Node*>(n.n));
        
        if (i) {
            o << "[";
            int n = 0;
            for (auto operand : i->operands) {
                o << (void*)operand;
                if (++n != i->operands.size()) {
                    o << ", ";
                }
            }
            o << "]";
        }

        return o;
    }
}
