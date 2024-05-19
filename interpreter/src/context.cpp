// copyright (c) 2017- mpab

#include <list>
#include <unordered_map>
#include <vector>

#include "logger.hpp"
#include "node.hpp"
#include "opcodes.hpp"
#include "parser.tab.hh"

// for debugging
#include "console.hpp"
#include "context.hpp"

#ifndef PARSER_USES_VOID_NODE_DEFINITION
void exec(node::Node* n) {
  if (!n) {  // for nop...
    logger::debug("exec(nullptr)");
    return;
  }

  context::execute_block(n);
}

node::Node* addi(int opcode, int count, node::Node* n1, node::Node* n2,
                 node::Node* n3) {
  return context::add_instruction(opcode, count, n1, n2, n3);
}

// required for parser testing
node::Node* nop() {
  logger::debug("nop()");
  return nullptr;
}
#else
/*
    these noddy wrappers and the shonky casts from void* to Node* are required
    if yacc/bison stuffs up the code generation...

    parser. y *should* contain this:

    %union {
        node::Node* node;
    };
*/

void exec(void* n) {
  if (!n) {  // for nop...
    logger::debug("exec(nullptr)");
    return;
  }

  context::execute_block((node::Node*)n);
}

void* addi(int opcode, int count, void* n1, void* n2, void* n3) {
  return context::add_instruction(opcode, count, (node::Node*)n1,
                                  (node::Node*)n2, (node::Node*)n3);
}

// required for parser testing
void* nop() {
  logger::debug("nop()");
  return nullptr;
}
#endif

namespace context {
using namespace std;
using namespace node;

Variable* eval(Node* n);

vector<Node*> blocks;
list<const Node*> tracked_nodes;
unordered_map<string, Node*> symbols;

const Node* this_active_block = nullptr;
const Node* active_block() { return this_active_block; }

ExecuteMode execute_mode = IMMEDIATE;
void set_execute_mode(ExecuteMode mode) { execute_mode = mode; }

const Node* this_last_created_node = nullptr;
const Node* last_created_node() { return this_last_created_node; }

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

bool is_not_an_instruction(Node* n) {
  if (n->type == INSTRUCTION) {
    return false;
  }

  if (n->type == VARIABLE) {
    console::echo(n);
  }

  if (n->type == SYMBOL) {
    auto v = eval(n);
    console::echo(v);
    delete v;  // because of unaliasing...
  }

  delete n;

  return true;
}

void execute_block(Node* n) {
#ifdef DEBUG_BUILD
  logger::info() << "void execute_block: " << NodeInfo(n) << endl;
#endif

  // print out raw variables or symbols in repl, and don't store them
  if (is_not_an_instruction(n)) {
    return;
  }

  blocks.push_back(n);
  if (execute_mode == IMMEDIATE) {
    this_active_block = n;

    Node* v_out = opcodes::execute(n);

    if (v_out && console::repl()) {
      auto v = eval(v_out);
      console::echo(v);
    }

    free_unused_nodes();
  }

#ifdef DEBUG_BUILD
  inspect();
#endif
}

Node* add_instruction(int opcode, int count, Node* n1, Node* n2, Node* n3) {
  auto v = vector<Node*>();

  switch (opcode) {
    case T_OUTL:
    case T_NEG:
    case T_UNDEFINED:
    case T_ABORT:
      v.push_back(n1);
      break;

    case T_COND:
      v.push_back(n1);
      v.push_back(n2);
      if (count > 2) {
        v.push_back(n3);
      }
      break;

    case T_COND_UNDEFINED:
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

#ifdef DEBUG_BUILD
  // logger::info() << "add_instruction:[" << opcode_name(opcode) << "](" <<
  // count << ")" << "[" << (void*)n1 << "][" << (void*)n2 << "][" << (void*)n3
  // << "]" << endl; logger::info() << NodeInfo(n) << endl; if (opcode ==
  // T_COND) { console::inspect(true); }
#endif

  return n;
}

bool check_binding(string symbol_name) {
  auto v = symbols.find(symbol_name);
  if (v == symbols.end()) {
    return false;
  }
  return true;
}

Node* get_binding(string symbol_name) {
  auto v = symbols.find(symbol_name);

  if (v == symbols.end()) {
    string warning = "symbol " + symbol_name + " is not defined";
    logger::warn() << warning << endl;
    return nullptr;
  }
  return v->second;
}

Variable* var_copy_cast(Node* n) {
  auto variable = dynamic_cast<Variable*>(n);
  if (variable) {
    // return a copy to prevent symbol aliasing...
    return new Variable(*variable);
  }
  return nullptr;
}

Variable* eval(Node* n) {
  logger::debug() << "Variable* eval(Node*) " << reinterpret_cast<void*>(n)
                  << endl;

  if (!n) {
    logger::warn("eval: Operand is null");
    return nullptr;
  }

  logger::debug("eval: trying variable");
  auto variable = dynamic_cast<Variable*>(n);
  if (variable) {
    return variable;
  }

  logger::debug("eval: trying symbol");
  auto symbol = dynamic_cast<Symbol*>(n);
  if (symbol) {
    auto node = get_binding(symbol->name);
    variable = var_copy_cast(node);
    if (variable) {
      return variable;
    }
  }

  logger::debug("eval: trying instruction");
  auto instruction = dynamic_cast<Instruction*>(n);
  if (instruction) {
    variable = var_copy_cast(opcodes::execute(instruction));
    if (variable) {
      return variable;
    }
  }

  logger::warn_if(!variable, "eval: bad value");
  return nullptr;
}

Node* bind(Symbol* k, Variable* v) {
  logger::debug() << "Node* bind(Symbol*, Variable*) "
                  << reinterpret_cast<void*>(k) << " "
                  << reinterpret_cast<void*>(v) << endl;

  if (!k || !v) {
    logger::warn() << "bind(s, v): bad symbol or rval" << endl;
    return nullptr;
  }

  symbols[k->name] = v;
  return v;
}

Node* bind(Node* k, Node* v) {
  logger::debug() << "Node* bind(Node*, Node*) " << reinterpret_cast<void*>(k)
                  << " " << reinterpret_cast<void*>(v) << endl;

  if (!k || !v) {
    logger::warn("bind (k, v): bad lval or rval");
    return nullptr;
  }
  return bind(dynamic_cast<Symbol*>(k), eval(v));
}

void free_all_nodes() {
  symbols.clear();
  free_unused_nodes();
}

void free_unused_nodes() {
  auto unused_nodes = tracked_nodes;

  for (auto it : symbols) {
    auto n = get_binding(it.first);
    logger::debug() << "free_unused_nodes() - keeping: " << n << endl;
    unused_nodes.remove(n);
  }

  for (auto n : unused_nodes) {
    logger::debug() << "free_unused_nodes() - deleting: " << n << endl;
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
  OUT << "================================= SYMBOLS "
         "====================================="
      << endl;
  OUT << "Symbols: " << context::symbols.size() << endl;

  for (auto it : context::symbols) {
    auto n = context::get_binding(it.first);
    OUT << it.first << " [" << NodeInfo(n) << "]" << endl;
  }

  OUT << "================================= BLOCKS "
         "======================================"
      << endl;
  OUT << "Blocks: " << context::blocks.size() << endl;

  for (auto n : context::blocks) {
    if (n == context::active_block()) {
      OUT << ANSI_COLOR_MAGENTA << NodeInfo(n) << endl;
    } else {
      OUT << NodeInfo(n) << endl;
    }
  }

  OUT << "================================= NODES "
         "======================================="
      << endl;
  OUT << "Nodes: " << context::tracked_nodes.size() << endl;

  for (auto n : context::tracked_nodes) {
    OUT << NodeInfo(n) << endl;
  }

  OUT << "====================================================================="
         "=========="
      << endl;
}
}  // namespace context
