#include <unordered_map>
#include <list>
#include <vector>
#include "node.h"
#include "logger.h"
#include "opcodes.h"
#include "y.tab.h"

// for debugging
#include "console.h"

#include "context.h"

namespace context {
    using namespace std;
    using namespace node;

    Variable* var(Node* n);

    vector<Node*> blocks;
    list <const Node*> tracked_nodes;
    unordered_map<string, Node*> symbols;
    
    const Node* this_active_block = nullptr;
    const Node* active_block() {
        return this_active_block;
    }

    ExecuteMode execute_mode = IMMEDIATE;
    void set_execute_mode(ExecuteMode mode) {
        execute_mode = mode;
    }

    const Node* this_last_created_node = nullptr;
    const Node* last_created_node() {
        return this_last_created_node;
    }

    void track_node_create(const Node* n) {
        if (n) {
            this_last_created_node = n;
            logger::debug() << NodeInfo(n) << "created" << endl;
            tracked_nodes.push_back(n);
        }
    }

    void track_node_delete(const Node* n) {
        if (n) {
            logger::debug() << NodeInfo(n) << "deleted" << endl;
            tracked_nodes.remove(n);
        }
    }

    bool is_not_an_instruction(Node *n) { 
        if (n->type == INSTRUCTION) {
            return false;
        }

        if (n->type == VARIABLE) {
            console::echo(n); 
        }

        if (n->type == SYMBOL) {
            auto v = var(n);
            console::echo(v);
            delete v; // because of unaliasing...
        }

        delete n;

        return true;
    }
     
    void execute_block(Node *n) { 
#       ifdef  DEBUG_BUILD
        logger::info() << "void execute_block: " << NodeInfo(n) << endl;
#       endif

        // print out raw variables or symbols in repl, and don't store them
        if (is_not_an_instruction(n)) {
            return;
        }

        blocks.push_back(n);
        if (execute_mode == IMMEDIATE) {
            this_active_block = n;

            Node* repl_out = opcodes::execute(n);

            if (console::repl()) {
                console::echo(var(repl_out));
            }

            free_unused_nodes();
        }

#   ifdef  DEBUG_BUILD
        inspect();
#   endif
    }

    Node* add_instruction(int opcode, int count, Node* n1, Node* n2, Node* n3) {
           
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

        auto n = new Instruction(opcode, v);
        logger::debug() << NodeInfo(n) << endl;

#       ifdef  DEBUG_BUILD
        //logger::info() << "add_instruction:[" << opcode_name(opcode) << "](" << count << ")" << "[" << (void*)n1 << "][" << (void*)n2 << "][" << (void*)n3 << "]" << endl;
        //logger::info() << NodeInfo(n) << endl;
        //if (opcode == T_COND) { console::inspect(true); }
#       endif

        return n;
    }

    Node* get_symbolic_node(string symbol_name) {
        auto v = symbols.find(symbol_name);

        if (v == symbols.end()) {
            string warning = "symbol " + symbol_name + " is not defined";
            logger::warn(warning.c_str());
            return nullptr;
        }
        return v->second;
    }

    Variable* var_copy_cast(Node* n) {
        auto variable = dynamic_cast<Variable*>(n);
        if (variable) {
            //return a copy to prevent symbol aliasing...
            return new Variable(*variable);
        }
        return nullptr;
    }

    Variable* var(Node* n) {

        logger::debug() << "Variable* var(Node*) " << (void*)n << endl;
        
        if (!n) {
            logger::error("var: Operand is null");
            return nullptr;
        }

        logger::debug("var: trying variable");
        auto variable = dynamic_cast<Variable*>(n);
        if (variable) {
            return variable;
        }

        logger::debug("var: trying symbol");
        auto symbol = dynamic_cast<Symbol*>(n);
        if (symbol) {
            auto node = get_symbolic_node(symbol->name);
            variable = var_copy_cast(node);
            if (variable) {
                return variable;
            }
        }

        logger::debug("var: trying instruction");
        auto instruction = dynamic_cast<Instruction*>(n);
        if (instruction) {
            variable = var_copy_cast(opcodes::execute(instruction));
            if (variable) {
                return variable;
            }
         }

        logger::warn_if(!variable, "var: failed to get variable");
        return nullptr;
    }

    Node* assign(Symbol* k, Variable* v) {
        
        logger::debug() << "Node* assign(Symbol*, Variable*) " << (void*)k << " " << (void*)v << endl;

        if (!k || !v) {
            logger::error("assign: null Symbol or Variable");
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
        return assign(dynamic_cast<Symbol*>(k), var(v));
    }

    void free_all_nodes() {
        symbols.clear();
        free_unused_nodes();
    }

    void free_unused_nodes() {
        
        auto unused_nodes = tracked_nodes;

        for (auto it : symbols) {    
            auto n = get_symbolic_node(it.first);
            logger::debug() << "free_unused_nodes() - keeping: " << (void*)n << endl;
            unused_nodes.remove(n);
        }

        for (auto n : unused_nodes) {
            logger::debug() << "free_unused_nodes() - deleting: " << (void*)n << endl;
            blocks.erase(remove(blocks.begin(), blocks.end(), n), blocks.end());
            delete n;
        }       
    }

    void execute_blocks() {
        for (auto b : blocks) {
            this_active_block = b;
            opcodes::execute(b);
        }
    }

/*
=======================================================================================================================
*/

#define OUT logger::info()

void inspect() {
    OUT << "================================= SYMBOLS =====================================" << endl;
    OUT << "Symbols: " << context::symbols.size() << endl;

    for (auto it : context::symbols) {
        auto n = context::get_symbolic_node(it.first);
        OUT << it.first << " [" << NodeInfo(n) << "]" << endl;
    }

    OUT << "================================= BLOCKS ======================================" << endl;
    OUT << "Blocks: " << context::blocks.size() << endl;

    for (auto n : context::blocks) {
        if (n == context::active_block()) {
            OUT << ANSI_COLOR_MAGENTA;
        }
        OUT << NodeInfo(n) << ANSI_COLOR_RESET << endl;
    }
    
    OUT << "================================= NODES =======================================" << endl;
    OUT << "Nodes: " << context::tracked_nodes.size() << endl;

    for (auto n : context::tracked_nodes) {
        OUT << NodeInfo(n) << endl;
    }

    OUT << "===============================================================================" << endl;
}
}
