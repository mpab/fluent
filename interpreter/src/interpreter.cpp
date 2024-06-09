// copyright (c) 2017- mpab

#include <stdio.h>
#include <stdlib.h>

#include <iostream>

#include "console.hpp"
#include "lexer.hh"
#include "parser.tab.hh"

int __column__ = 1;
extern char *_src_filename;

using namespace std;

const char *args_file = "args.fl";

void create_args_file(const char *file, int argc, char **argv) {
  FILE *fh;
#ifdef _MSC_VER
  fopen_s(&fh, file, "w");
#else
  fh = fopen(file, "w");
#endif
  int idx = 0;
  while (argc--) {
    char *err;

    strtol(argv[0], &err, 10);
    if (!*err) {
      fprintf(fh, "args_%d=%s;\n", idx++, argv[0]);
      ++argv;
      continue;
    }

    strtof(argv[0], &err);
    if (!*err) {
      fprintf(fh, "args_%d=%s;\n", idx++, argv[0]);
      ++argv;
      continue;
    }

    fprintf(fh, "args_%d='%s';\n", idx++, argv[0]);
    ++argv;
  }
  fclose(fh);
}

void set_yyin_file(const char *src) {
#ifdef _MSC_VER
  fopen_s(&yyin, src, "r");
#else
  yyin = fopen(src, "r");
#endif
  if (!yyin) {
    cout << "file: " << _src_filename << " not found.\n";
    // logger::error() << "file: " << src << " not found.\n";
  }
}

int parse_file(int argc, char **argv) {
  create_args_file(args_file, argc, argv);
  set_yyin_file(args_file);
  yyparse();
  fclose(yyin);
  return yyin != 0;
}

int mode_args = 1;
int yywrap() {
  if (mode_args) {
    fclose(yyin);
    remove(args_file);
    set_yyin_file(_src_filename);
    mode_args = 0;
    return 0;
  }
  return 1;
}

int main(int argc, char **argv) {
  ++argv, --argc; /* skip over program name */
  _src_filename = argv[0];
  if (argc > 0) {
    return parse_file(argc, argv);
  }
  yyin = stdin;
  console::copyright();
  yyparse();
}
