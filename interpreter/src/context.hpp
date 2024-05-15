// copyright (c) 2017- mpab

#pragma once

#include <string>

#include "node.hpp"

#ifndef PARSER_USES_VOID_NODE_DEFINITION
void exec(node::Node* n);
node::Node* addi(int opcode, int count, node::Node* n1,
                 node::Node* n2 = nullptr, node::Node* n3 = nullptr);
// required for parser testing only
node::Node* nop();
#else
/*
    these noddy wrappers and the shonky casts from void* to Node* are required
    if yacc/bison stuffs up the code generation...

    parser. y *should* contain this:

    %union {
        node::Node* node;
    };
*/
void exec(void* n);
void* addi(int opcode, int count, void* n1, void* n2 = nullptr,
           void* n3 = nullptr);
// required for parser testing only
void* nop();
#endif

namespace context {

node::Node* add_instruction(int opcode, int count, node::Node* n1,
                            node::Node* n2 = nullptr, node::Node* n3 = nullptr);

node::Node* get_binding(std::string symbol_name);
void free_unused_nodes();
void free_all_nodes();

void execute_blocks();
void execute_block(node::Node* n);

const node::Node* active_block();
const node::Node* last_created_node();

enum ExecuteMode { IMMEDIATE, BUFFERED };
void set_execute_mode(ExecuteMode mode);

void inspect();
}  // namespace context
