//
// Created by mick on 13.10.17.
//

#ifndef FLUENT_PARSER_NODE_H
#define FLUENT_PARSER_NODE_H

#include <ostream>
#include <vector>
#include <boost/variant.hpp>

namespace node {
    using namespace std;
    using namespace boost;

    using Value = variant <long, double, std::string>;
    enum Type {FUN, SYM, VAR};

    struct Node {
        const Type type;
        const char* type_name() const;
        virtual ~Node();

    protected:
        Node(const Type type);
    };

      struct Function : Node {
        int const opcode;
        vector<Node*> operands;
        Function(int opcode, vector<Node*> operands) : Node(FUN), opcode(opcode), operands(operands) {}
    };

    struct Variable : public Node {
        Value value;

        Variable(long v) : Node(VAR), value(v) {}
        Variable(double v) : Node(VAR), value(v) {}
        Variable(string v) : Node(VAR), value(v) {}
        Variable(const Variable& v) : Node(VAR), value(v.value) {}
        Variable(const Value* v) : Node(VAR), value(*v) {}

        string chars() const { return get<string>(value); }
        
        long integer() const { return get<long>(value); }
        void integer(long n) { value = n; }
        
        double real() const { return get<double>(value); }
        void real(double d) { value = d; }
    };

    struct Symbol : public Node {
        string const name;
        Symbol(string name) : Node(SYM), name(name) {}
    };

    const char* type_name(Type t);
    
    // for lexer
    Symbol* create_symbol(const char* s);
    Variable* create_integer(const char* s);
    Variable* create_real(const char* s);
    Variable* create_quoted_string(char* s);

    // for copying
    Variable* create_integer(long i);
    Variable* create_real(double r);
  
    ostream& operator << (ostream& o, const Node& n);
    ostream& operator << (ostream& o, const Node* n);
    
    struct NodeInfo {
        const Node* n;
        NodeInfo(const Node* n) : n(n) {}
    };
    ostream& operator << (ostream& o, const NodeInfo& n);
}

#endif //FLUENT_PARSER_NODE_H
