#include <unordered_map>
#include <list>
#include <vector>
#include "node.h"
#include "logger.h"
#include "functions.h"
#include "y.tab.h"

// for debugging
#include "console.h"

#include "context.h"

namespace context {
    using namespace std;
    using namespace node;

    Node* this_active_frame = nullptr;
    Node* active_frame() {
        return this_active_frame;
    }

    ExecuteMode execute_mode = IMMEDIATE;
    void set_execute_mode(ExecuteMode mode) {
        execute_mode = mode;
    }

    vector<Node*> frames;
    
    void execute(Node *n) { 

#       ifdef  DEBUG_BUILD
        logger::info() << "void execute: " << NodeInfo(n) << endl;
#       endif

        frames.push_back(n);
        if (execute_mode == IMMEDIATE) {
            this_active_frame = n;
            functions::execute(n);
            free_unused_nodes();
        }

#   ifdef  DEBUG_BUILD
        console::inspect(true);
#   endif

    }

    list <const Node*> tracked_nodes;

    unordered_map<string, Node*> symbols;

    void track_node_create(const Node* n) {
        logger::debug() << NodeInfo(n) << "created" << endl;
        tracked_nodes.push_back(n);
    }

    void track_node_delete(const Node* n) {
        logger::debug() << NodeInfo(n) << "deleted" << endl;
        tracked_nodes.remove(n);
    }

    Node* func(int opcode, int count, Node* n1, Node* n2, Node* n3) {
           
        auto v = vector<Node*>();

        switch (opcode) {
            case T_OUTL:
            case T_NEG:
                v.push_back(n1);
                break;

            case T_COND:
                v.push_back(n1);
                v.push_back(n2);
                if (count > 2) {
                    v.push_back(n3);
                }
            break;

            default:
            v.push_back(n1);
            v.push_back(n2);
            break;
        
        }

        auto n = new Function(opcode, v);
        logger::debug() << NodeInfo(n) << endl;

#       ifdef  DEBUG_BUILD
        //logger::info() << "func:[" << opcode_name(opcode) << "](" << count << ")" << "[" << (void*)n1 << "][" << (void*)n2 << "][" << (void*)n3 << "]" << endl;
        //logger::info() << NodeInfo(n) << endl;
        //if (opcode == T_COND) { console::inspect(true); }
#       endif

        return n;
    }

    void warn_undefined(string n) {
        string s = "symbol " + n + " is not defined";
        logger::warn(s.c_str());
    }

    Node* get_node(string k, bool warn_if_not_defined) {
        auto v = symbols.find(k);

        if (v == symbols.end()) {
            if (warn_if_not_defined) {
                warn_undefined(k);
            }
            return nullptr;
        }
        return v->second;
    }

    Variable* copy_dynamic_cast(Node* n) {
        auto variable = dynamic_cast<Variable*>(n);
        if (variable) {
            //return a copy to prevent symbol aliasing...
            return new Variable(*variable);
        }
        return nullptr;
    }

    Variable* get_val(Node* n) {

        logger::debug() << "Variable* get_val(Node*) " << (void*)n << endl;
        
        if (!n) {
            logger::error("get_val: Operand is null");
            return nullptr;
        }

        logger::debug("get_val: trying variable");
        auto variable = dynamic_cast<Variable*>(n);
        if (variable) {
            return variable;
        }

        logger::debug("get_val: trying symbol");
        auto symbol = dynamic_cast<Symbol*>(n);
        if (symbol) {
            auto node = get_node(symbol->name);
            variable = copy_dynamic_cast(node);
            if (variable) {
                return variable;
            }
        }

        logger::debug("get_val: trying instruction");
        auto instruction = dynamic_cast<Function*>(n);
        if (instruction) {
            variable = copy_dynamic_cast(functions::execute(instruction));
            if (variable) {
                return variable;
            }
         }

        logger::error_if(!variable, "get_val: failed");
        return nullptr;
    }

    Node* assign(Symbol* k, Variable* v) {
        
        logger::debug() << "Node* assign(Symbol*, Variable*) " << (void*)k << " " << (void*)v << endl;

        if (!k || !v) {
            logger::error("assign: null LHS or RHS");
            return nullptr;
        }

        symbols[k->name] = v;
        return v;
    }

    Node* assign(Node* k, Node* v) {
        
        logger::debug() << "Node* assign(Node*, Node*) " << (void*)k << " " << (void*)v << endl;

        if (!k || !v) {
            logger::error("assign: null LHS or RHS");
            return nullptr;
        }
        return assign(dynamic_cast<Symbol*>(k), get_val(v));
    }

    void free_all_nodes() {
        symbols.clear();
        free_unused_nodes();
    }

    void free_unused_nodes() {
        
        auto unused_nodes = tracked_nodes;

        for (auto it : symbols) {    
            auto n = context::get_node(it.first);
            logger::debug() << "free_unused_nodes() - keeping: " << (void*)n << endl;
            unused_nodes.remove(n);
        }

        for (auto n : unused_nodes) {
            logger::debug() << "free_unused_nodes() - deleting: " << (void*)n << endl;
            frames.erase(remove(frames.begin(), frames.end(), n), frames.end());
            delete n;
        }       
    }

    void execute() {
        for (auto f : frames) {
            this_active_frame = f;
            functions::execute(f);
        }
    }
}
