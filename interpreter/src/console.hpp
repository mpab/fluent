// copyright (c) 2017- mpab

#pragma once

#include "node.hpp"

namespace console {
using namespace node;

bool repl();

void echo(Node* p);
void help();
void quit();
void copyright();
}  // namespace console
