#include <iostream>
#include "node.h"
#include "logger.h"
#include "y.tab.h"
#include "opcode_templates.h"

namespace context {
    using namespace node;
    Variable* get_val(Node* n);
    Node* assign(Node* k, Node* v);
}

namespace opcodes {
    using namespace node;
    using namespace std;

    Node* execute(Node* n);

    Node* print(Node* n) {
        cout << context::get_val(n);
        return nullptr;
    }

    Node* println(Node* n) {
        cout << context::get_val(n) << endl;
        return nullptr;
    }

    bool eval_condition(Node* n) {
        auto value = context::get_val(n);
        if (!value) {
            return false;
        }
        return op::conditional(value);
    }

    Node* execute(Instruction *n) {
        
        logger::debug() << "execute(Instruction*) " << NodeInfo(n) << endl;

        switch(n->opcode) {
            case '=':
            return context::assign(n->operands[0], n->operands[1]);

            case '+':
            return op::add(context::get_val(n->operands[0]), context::get_val(n->operands[1]));

            case '-':
            return op::sub(context::get_val(n->operands[0]), context::get_val(n->operands[1]));

            case '*':
            return op::mul(context::get_val(n->operands[0]), context::get_val(n->operands[1]));

            case '/':
            return op::div(context::get_val(n->operands[0]), context::get_val(n->operands[1]));
            
            case T_NEG:
            return op::negate(context::get_val(n->operands[0]));

            case T_OUT:
            return print(n->operands[0]);

            case T_OUTL:
            return println(n->operands[0]);

            case T_COND:             
            if (eval_condition(n->operands[0])) {
                execute(n->operands[1]);
            } else if (n->operands.size() > 2) {
                execute(n->operands[2]);
            }
            return nullptr;

            case T_WHILE:
            while(eval_condition(n->operands[0])) {
                execute(n->operands[1]); 
            }
            return nullptr;

            case ';':
            execute(n->operands[0]); 
            return execute(n->operands[1]); 

            case '>':
            //logger::info() << "Node* execute(Instruction *n): '>'" << endl;
            return op::gt(context::get_val(n->operands[0]), context::get_val(n->operands[1]));

            case T_GE:
            //logger::info() << "Node* execute(Instruction *n): '>='" << endl;
            return op::ge(context::get_val(n->operands[0]), context::get_val(n->operands[1]));

            case '<':
            //logger::info() << "Node* execute(Instruction *n): '<'" << endl;
            return op::lt(context::get_val(n->operands[0]), context::get_val(n->operands[1]));

            case T_LE:
            //logger::info() << "Node* execute(Instruction *n): '<='" << endl;
            return op::le(context::get_val(n->operands[0]), context::get_val(n->operands[1]));

            case T_EQ:
            //logger::info() << "Node* execute(Instruction *n): '=='" << endl;
            return op::eq(context::get_val(n->operands[0]), context::get_val(n->operands[1]));

            case T_NE:
            //logger::info() << "Node* execute(Instruction *n): '!='" << endl;
            return op::ne(context::get_val(n->operands[0]), context::get_val(n->operands[1]));

            case '^':
            //logger::info() << "Node* execute(Instruction *n): '^'" << endl;
            return op::exp(context::get_val(n->operands[0]), context::get_val(n->operands[1]));

            default:
            break;

        }

        logger::error() << "unhandled opcode: " << NodeInfo(n) << endl;
        return nullptr;
    }

    Node* execute(Node* n) {
        logger::debug() << "execute(Node*)" << NodeInfo(n) << endl;

        auto f = dynamic_cast<Instruction*>(n);
        if (f) {
            return execute(f);
        }

        logger::error() << "execute: unhandled node type - " << n;
        return nullptr;
    }
}