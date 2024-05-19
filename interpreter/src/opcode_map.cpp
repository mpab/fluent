// copyright (c) 2017- mpab

#include "parser.tab.hh"

const char* opcode_name(int id) {
  switch (id) {
    case T_CLOSURE:
      return "(){}";
    case T_WHILE:
      return "while";

    case T_COND:
      return "if";

    case T_OUT:
      return "out";

    case T_OUTL:
      return "outl";

    case T_UNDEFINED:
      return "undefined";

    case T_ABORT:
      return "abort";

    case ';':
      return ";";

    case '=':
      return "=";

    case T_NEG:
      return "(-)";

    case '+':
      return "+";

    case '-':
      return "-";

    case '*':
      return "*";

    case '/':
      return "/";

    case '<':
      return "<";

    case '>':
      return ">";

    case T_GE:
      return ">=";

    case T_LE:
      return "<=";

    case T_NE:
      return "!=";

    case T_EQ:
      return "==";

    case '^':
      return "^";

    case '%':
      return "%";
  }

  return ("{UNKNOWN}");
}
