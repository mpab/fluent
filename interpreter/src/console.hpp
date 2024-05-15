// copyright (c) 2017- mpab

#pragma once

#include "node.hpp"

namespace console {

bool repl();

void echo(node::Node* p);
void help();
void quit();
void copyright();
}  // namespace console
