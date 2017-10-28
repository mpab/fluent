#include <iostream>
#include "node.h"
#include "logger.h"
#include "../gen/y.tab.h"
#include "opcode_templates.h"

namespace context {
    using namespace node;
    Variable* eval(Node* n);
    Node* assign(Node* k, Node* v);
}

namespace opcodes {
    using namespace node;

    Node* execute(Node* n);

    Node* print(Node* n) {
        cout << context::eval(n);
        return nullptr;
    }

    Node* println(Node* n) {
        cout << context::eval(n) << endl;
        return nullptr;
    }

    bool eval_condition(Node* n) {
        auto value = context::eval(n);
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
            return op::add(context::eval(n->operands[0]), context::eval(n->operands[1]));

            case '-':
            return op::sub(context::eval(n->operands[0]), context::eval(n->operands[1]));

            case '*':
            return op::mul(context::eval(n->operands[0]), context::eval(n->operands[1]));

            case '/':
            return op::div(context::eval(n->operands[0]), context::eval(n->operands[1]));
            
            case T_NEG:
            return op::negate(context::eval(n->operands[0]));

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
            return op::gt(context::eval(n->operands[0]), context::eval(n->operands[1]));

            case T_GE:
            return op::ge(context::eval(n->operands[0]), context::eval(n->operands[1]));

            case '<':
            return op::lt(context::eval(n->operands[0]), context::eval(n->operands[1]));

            case T_LE:
;
            return op::le(context::eval(n->operands[0]), context::eval(n->operands[1]));

            case T_EQ:
            return op::is_eq(context::eval(n->operands[0]), context::eval(n->operands[1]));

            case T_NE:
            return op::ne(context::eval(n->operands[0]), context::eval(n->operands[1]));

            case '^':
            return op::exp(context::eval(n->operands[0]), context::eval(n->operands[1]));

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