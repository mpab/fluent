// copyright (c) 2017- mpab

#include "parser.tab.hh"

const char* opcode_name(int id) {
  switch (id) {
    // case T_CLOSURE:
    //   return "(){}";
    case T_WHILE:
      return "while";

    case T_IF:
      return "if";

    case T_OUT:
      return "out";

    case T_OUTL:
      return "outl";

    case T_UNDEFINED:
      return "undefined";

    case T_EXIT:
      return "exit";

    // case T_STMT_TERM:
    //   return ";";
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
