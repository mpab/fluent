// copyright (c) 2017- mpab

#pragma once

#include <boost/variant.hpp>
#include <ostream>
#include <vector>

namespace node {

using Value = boost::variant<int64_t, double, std::string>;
enum Type { INSTRUCTION, SYMBOL, VARIABLE };

struct Node {
  const Type type;
  const char* type_name() const;
  virtual ~Node();

 protected:
  explicit Node(const Type type);
};

struct Instruction : Node {
  int const opcode;
  std::vector<Node*> operands;
  Instruction(int opcode, std::vector<Node*> operands)
      : Node(INSTRUCTION), opcode(opcode), operands(operands) {}
};

struct Variable : public Node {
  Value value;

  explicit Variable(int64_t v) : Node(VARIABLE), value(v) {}
  explicit Variable(double v) : Node(VARIABLE), value(v) {}
  explicit Variable(std::string v) : Node(VARIABLE), value(v) {}
  explicit Variable(const Variable& v) : Node(VARIABLE), value(v.value) {}
  explicit Variable(const Value* v) : Node(VARIABLE), value(*v) {}

  std::string chars() const { return get<std::string>(value); }

  int64_t integer() const { return get<int64_t>(value); }
  void integer(int64_t n) { value = n; }

  double real() const { return get<double>(value); }
  void real(double d) { value = d; }
};

struct Symbol : public Node {
  std::string const name;
  explicit Symbol(std::string name) : Node(SYMBOL), name(name) {}
};

const char* type_name(Type t);

// for lexer
Symbol* create_symbol(const char* s);
Variable* create_integer(const char* s);
Variable* create_real(const char* s);
Variable* create_quoted_string(const char* s);

// for copying
Variable* create_integer(int64_t i);
Variable* create_real(double r);

std::ostream& operator<<(std::ostream& o, const Node& n);
std::ostream& operator<<(std::ostream& o, const Node* n);

struct NodeInfo {
  const Node* n;
  explicit NodeInfo(const Node* n) : n(n) {}
};
std::ostream& operator<<(std::ostream& o, const NodeInfo& n);
}  // namespace node
