// copyright (c) 2017- mpab

#include "node.hpp"

#include <stdlib.h>

#include <iostream>

#include "logger.hpp"

namespace context {
using namespace node;
using namespace boost;

void track_node_create(const Node* n);
void track_node_delete(const Node* n);
}  // namespace context

namespace node {
using namespace std;

Node::Node(const Type type) : type(type) { context::track_node_create(this); }

Node::~Node() { context::track_node_delete(this); }

const char* Node::type_name() const { return node::type_name(type); }

const char* type_name(Type t) {
  const char* names[] = {"INSTRUCTION", "SYMBOL", "VARIABLE"};
  return names[t];
}

Symbol* create_symbol(const char* s) { return new Symbol(s); }

Variable* create_integer(const char* s) {
  int64_t i = atoi(s);
  return create_integer(i);
}

Variable* create_real(const char* s) {
  double r = atof(s);
  return create_real(r);
}

Variable* create_string(const char* s) { return new Variable(s); }

Variable* create_quoted_string(const char* s) {
  std::string qs(s);
  auto str = qs.substr(1, qs.length() - 2);
  return create_string(str.c_str());
}

Variable* create_integer(int64_t i) { return new Variable(i); }

Variable* create_real(double r) { return new Variable(r); }

/*
=======================================================================================================================
*/

ostream& operator<<(ostream& o, const Node& node) { return o << &node; }

ostream& operator<<(ostream& o, const Node* n) {
#ifdef DEBUG_BUILD
  if (!n) return o << "{nullptr}";
#else
  if (!n) return o;
#endif

  auto v = dynamic_cast<Variable*>(const_cast<Node*>(n));
  if (v) {
    o << v->value;
  }

  auto f = dynamic_cast<Instruction*>(const_cast<Node*>(n));
  if (f) {
    o << FMT8 << opcode_name(f->opcode);
  }

  auto s = dynamic_cast<Symbol*>(const_cast<Node*>(n));
  if (s) {
    o << s->name;
  }

  return o;
}

ostream& operator<<(ostream& o, const NodeInfo& n) {
  if (!n.n) {
    return o << FMT24 << reinterpret_cast<const void*>(n.n);
  }

  o << FMT24 << reinterpret_cast<const void*>(n.n) << FMT12 << n.n->type_name()
    << n.n;

  auto i = dynamic_cast<Instruction*>(const_cast<Node*>(n.n));

  if (i) {
    o << "[";
    std::vector<node::Node*>::size_type sz = 0;
    for (auto operand : i->operands) {
      o << reinterpret_cast<void*>(operand);
      if (++sz != i->operands.size()) {
        o << ", ";
      }
    }
    o << "]";
  }

  return o;
}
}  // namespace node
